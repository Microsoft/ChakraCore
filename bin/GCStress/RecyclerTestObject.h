//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "stdafx.h"

#include "core/RecyclerHeapMarkingContext.h"

class RecyclerTestObject : public IRecyclerVisitedObject
{
protected:
    RecyclerTestObject()
    {
        generation = currentGeneration;
    }

public:
    // IRecyclerVisitedObject implementation. We don't use FinalizableObject here as
    // RecyclerVisitedObjects need to have Trace called on them, which is not allowed for
    // FinalizableObject.
    virtual void __stdcall Finalize(bool isShutdown) override { VerifyCondition(false); };
    virtual void __stdcall Dispose(bool isShutdown) override { VerifyCondition(false); };
    virtual void __stdcall OnMark() override {}
    virtual void __stdcall Mark(RecyclerHeapHandle recycler) override { Mark(static_cast<Recycler*>(recycler)); };
    virtual void __stdcall Trace(IRecyclerHeapMarkingContext* markContext) override { VerifyCondition(false); };

    virtual void __stdcall Mark(Recycler * recycler) { VerifyCondition(false); };

public:
    static void BeginWalk()
    {
        currentGeneration++;

        walkObjectCount = 0;
        walkScannedByteCount = 0;
        walkBarrierByteCount = 0;
        walkTrackedByteCount = 0;
        walkRecyclerVisitedByteCount = 0;
        walkLeafByteCount = 0;
        maxWalkDepth = 0;

        currentWalkDepth = 0;

        wprintf(_u("-------------------------------------------\n"));
        wprintf(_u("Full heap walk starting\n"));
    }

    static void WalkReference(RecyclerTestObject * object)
    {
        if (object != nullptr)
        {
            // See if we've already seen the location in this traversal.
            if (object->generation != currentGeneration)
            {
                // Haven't see it yet.  Must be from the previous generation; increment generation and validate that.
                // Update to current generation to indicate we've seen it
                object->generation++;
                VerifyCondition(object->generation == currentGeneration);

                walkObjectCount++;

                currentWalkDepth++;
                maxWalkDepth = max(currentWalkDepth, maxWalkDepth);

                // Call virtual for object-specific behavior
                object->DoWalkObject();

                currentWalkDepth--;
            }
        }
    }

    static void EndWalk()
    {
        VerifyCondition(currentWalkDepth == 0);

        wprintf(_u("Full heap walk finished\n"));
        wprintf(_u("Object Count:           %12llu\n"), (unsigned long long) walkObjectCount);
        wprintf(_u("Scanned Bytes:          %12llu\n"), (unsigned long long) walkScannedByteCount);
        wprintf(_u("Barrier Bytes:          %12llu\n"), (unsigned long long) walkBarrierByteCount);
        wprintf(_u("Tracked Bytes:          %12llu\n"), (unsigned long long) walkTrackedByteCount);
        wprintf(_u("RecyclerVisited Bytes:  %12llu\n"), (unsigned long long) walkRecyclerVisitedByteCount);
        wprintf(_u("Leaf Bytes:             %12llu\n"), (unsigned long long) walkLeafByteCount);
        wprintf(_u("Total Bytes:            %12llu\n"), (unsigned long long) (walkScannedByteCount + walkBarrierByteCount + walkTrackedByteCount + walkLeafByteCount + walkRecyclerVisitedByteCount));
        wprintf(_u("Max Depth:              %12llu\n"), (unsigned long long) maxWalkDepth);
    }

    // Virtual methods
    virtual bool TryGetRandomLocation(Location * location)
    {
        // Return false to indicate no internal locations
        // Subclasses can override this to handle their internal locations as appropriate
        return false;
    }

    virtual void DoWalkObject() = 0;

protected:
    // Global variables

    // This global variable contains the "generation" of GC objects
    // It is used to validate the correctness of GC objects
    // It is assigned initially during object creation, and
    // updated when we walk the entire object graph in TraverseAllObjects
    static size_t currentGeneration;

    // These globals contain statistical data generated by WalkAllReferences
    static size_t walkObjectCount;
    static size_t walkScannedByteCount;
    static size_t walkLeafByteCount;
    static size_t walkBarrierByteCount;
    static size_t walkTrackedByteCount;
    static size_t walkRecyclerVisitedByteCount;
    static size_t currentWalkDepth;
    static size_t maxWalkDepth;

private:
    // Instance variables

    // See comments above re currentGeneration
    Field(size_t) generation;
};

template <unsigned int minCount, unsigned int maxCount>
class LeafObject : public RecyclerTestObject
{
private:
    LeafObject(unsigned int count) :
        count(count)
    {
        for (unsigned int i = 0; i < count; i++)
        {
            data[i] = i;
        }
    }

public:
    static RecyclerTestObject * New()
    {
        unsigned int count = minCount + GetRandomInteger(maxCount - minCount + 1);

        return RecyclerNewPlusLeaf(recyclerInstance, sizeof(size_t) * count, LeafObject, count);
    }

protected:
    virtual void DoWalkObject() override
    {
        walkLeafByteCount += sizeof(LeafObject) + count * sizeof(size_t);
    }

private:
    Field(unsigned int) count;
    Field(size_t ) data[0];
};

template <unsigned int minCount, unsigned int maxCount>
class ScannedObject : public RecyclerTestObject
{
private:
    ScannedObject(unsigned int count) :
        count(count)
    {
        for (unsigned int i = 0; i < count; i++)
        {
            references[i] = nullptr;
        }
    }

public:
    static RecyclerTestObject * New()
    {
        unsigned int count = minCount + GetRandomInteger(maxCount - minCount + 1);

        return RecyclerNewPlus(recyclerInstance, sizeof(RecyclerTestObject *) * count, ScannedObject, count);
    }

    virtual bool TryGetRandomLocation(Location * location) override
    {
        // Get a random slot and construct a Location for it
        *location = Location::Scanned(&references[GetRandomInteger(count)]);

        return true;
    }

protected:
    virtual void DoWalkObject() override
    {
        walkScannedByteCount += sizeof(ScannedObject) + count * sizeof(RecyclerTestObject *);

        for (unsigned int i = 0; i < count; i++)
        {
            RecyclerTestObject::WalkReference(references[i]);
        }
    }

private:
    Field(unsigned int) count;
    FieldNoBarrier(RecyclerTestObject *) references[0];  // SWB-TODO: is this correct?
};

template <unsigned int minCount, unsigned int maxCount>
class BarrierObject : public RecyclerTestObject
{
private:
    BarrierObject(unsigned int count) :
        count(count)
    {
        for (unsigned int i = 0; i < count; i++)
        {
            references[i] = nullptr;
        }
    }

public:
    static RecyclerTestObject * New()
    {
        unsigned int count = minCount + GetRandomInteger(maxCount - minCount + 1);

        return RecyclerNewWithBarrierPlus(recyclerInstance, sizeof(RecyclerTestObject *) * count, BarrierObject, count);
    }

    virtual bool TryGetRandomLocation(Location * location) override
    {
        // Get a random slot and construct a Location for it
        *location = Location::Barrier(&references[GetRandomInteger(count)]);

        return true;
    }

protected:
    virtual void DoWalkObject() override
    {
        walkBarrierByteCount += sizeof(BarrierObject) + count * sizeof(RecyclerTestObject *);

        for (unsigned int i = 0; i < count; i++)
        {
            RecyclerTestObject::WalkReference(references[i]);
        }
    }

private:
    Field(unsigned int) count;
    FieldNoBarrier(RecyclerTestObject *) references[0];  // SWB-TODO: is this correct?
};

// TrackedObject must be a FinalizableObject (in order to be 'new'ed with RecyclerNewTrackedLeafPlusZ)
// but it also must be a RecyclerTestObject to participate in GCStress. It must inherit from RecyclerTestObject
// first so that the algined pointer is returned from New.
// Fortunately, the v-tables for RecyclerTestObject and FinalizableObject line up, so the 
// IRecyclerVisitedObject/FinalizableObject calls end up in the right place.
template <unsigned int minCount, unsigned int maxCount>
class TrackedObject : public RecyclerTestObject, public FinalizableObject
{
private:
    TrackedObject(unsigned int count) :
        count(count)
    {
        for (unsigned int i = 0; i < count; i++)
        {
            references[i] = nullptr;
        }
    }

public:
    static RecyclerTestObject * New()
    {
        unsigned int count = minCount + GetRandomInteger(maxCount - minCount + 1);

        return RecyclerNewTrackedLeafPlusZ(recyclerInstance, sizeof(RecyclerTestObject *) * count, TrackedObject, count);
    }

    virtual bool TryGetRandomLocation(Location * location) override
    {
        // Get a random slot and construct a Location for it
        *location = Location::Tagged(&references[GetRandomInteger(count)]);

        return true;
    }

    // Tracked object implementation
    virtual void __stdcall Mark(Recycler * recycler) override
    {
        for (unsigned int i = 0; i < count; i++)
        {
            RecyclerTestObject * object = Location::Untag(references[i]);
            if (object != nullptr)
            {
                recycler->TryMarkNonInterior(object);
            }
        }
    };

    // Tracked objects are always finalize as well. Just do nothing.
    virtual void __stdcall Finalize(bool isShutdown) override { }
    virtual void __stdcall Dispose(bool isShutdown) override { };


protected:
    virtual void DoWalkObject() override
    {
        walkTrackedByteCount += sizeof(TrackedObject) + count * sizeof(RecyclerTestObject *);

        for (unsigned int i = 0; i < count; i++)
        {
            RecyclerTestObject::WalkReference(Location::Untag(references[i]));
        }
    }

private:
    Field(unsigned int) count;
    FieldNoBarrier(RecyclerTestObject *) references[0];  // SWB-TODO: is this correct?
};

template <unsigned int minCount, unsigned int maxCount>
class RecyclerVisitedObject : public RecyclerTestObject
{
public:
    static RecyclerTestObject * New()
    {
        // Determine a random amount of RecyclerTestObject* references to influence the size of this object.
        const unsigned int count = minCount + GetRandomInteger(maxCount - minCount + 1);

        void* mem = nullptr;
        const size_t size = sizeof(RecyclerVisitedObject) + (sizeof(RecyclerTestObject*) * count);

        // Randomly select the type of object to create
        AllocationType allocType = static_cast<AllocationType>(GetRandomInteger(static_cast<unsigned int>(AllocationType::Count)));
        switch (allocType)
        {
        case AllocationType::TraceAndFinalized:
            mem = RecyclerAllocVisitedHostTracedAndFinalizedZero(recyclerInstance, size);
            break;
        case AllocationType::TraceOnly:
            mem = RecyclerAllocVisitedHostTracedZero(recyclerInstance, size);
            break;
        case AllocationType::FinalizeLeaf:
            mem = RecyclerAllocVisitedHostFinalizedZero(recyclerInstance, size);
            break;
        case AllocationType::Leaf:
            mem = RecyclerAllocVisitedHostLeafZero(recyclerInstance, size);
            break;

        default:
            Assert(false);
        }

        // Construct the v-table, allocType, and count information for the new object.
        RecyclerVisitedObject* obj = new (mem) RecyclerVisitedObject(allocType, count);
        return obj;
    }

    virtual bool TryGetRandomLocation(Location * location) override
    {
        // Leaf types should not return a location
        if (type == AllocationType::Leaf || type == AllocationType::FinalizeLeaf)
        {
            return false;
        }

        // Get a random slot and construct a Location for it
        // Make this a Tagged location so that we won't inadvertently keep objects alive
        // in the case where this object gets put on the wrong mark stack.
        *location = Location::Tagged(&references[GetRandomInteger(count)]);

        return true;
    }

    virtual void __stdcall Trace(IRecyclerHeapMarkingContext* markContext) override
    {
        VerifyCondition(type == AllocationType::TraceAndFinalized || type == AllocationType::TraceOnly);
        // Note that the pointers in the references arrary are technically tagged. However, this is ok
        // as the Mark that we're performing is an interior mark, which gets us to the right object(s).
        markContext->MarkObjects(reinterpret_cast<void**>(&references[0]), count, this);
    }

    virtual void __stdcall Finalize(bool isShutdown) override
    {
        // Only types that request finalization should have Finalize called
        VerifyCondition(IsFinalizable());
    }
    virtual void __stdcall Dispose(bool isShutdown) override
    { 
        // Only types that request finalization should have Finalize called
        VerifyCondition(IsFinalizable());
        VerifyCondition(unmanagedResource != nullptr);
        BOOL success = ::HeapFree(GetProcessHeap(), 0, unmanagedResource);
        VerifyCondition(success != FALSE);
        unmanagedResource = nullptr;
    }


protected:
    virtual void DoWalkObject() override
    {
        walkRecyclerVisitedByteCount += sizeof(RecyclerVisitedObject) + count * sizeof(RecyclerTestObject *);

        for (unsigned int i = 0; i < count; i++)
        {
            RecyclerTestObject::WalkReference(Location::Untag(references[i]));
        }
    }

private:
    enum class AllocationType : unsigned int
    {
        TraceAndFinalized = 0,
        TraceOnly,
        FinalizeLeaf,
        Leaf,
        Count,
    };

    bool IsFinalizable() const { return type == AllocationType::TraceAndFinalized || type == AllocationType::FinalizeLeaf; }
    RecyclerVisitedObject(AllocationType allocType, unsigned int count) :
        count(count),
        type(allocType)
    {
        for (unsigned int i = 0; i < count; i++)
        {
            references[i] = nullptr;
        }
        if (IsFinalizable())
        {
            unmanagedResource = ::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, GetRandomInteger(1024));
            VerifyCondition(unmanagedResource != nullptr);
        }
    }


    Field(AllocationType) type;
    Field(void*) unmanagedResource;
    Field(unsigned int) count;
    FieldNoBarrier(RecyclerTestObject *) references[0];  // SWB-TODO: is this correct? (copied from TrackedObject)
};
