//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

interface IRecyclerHeapMarkingContext;
typedef void* RecyclerHeapHandle;

interface IRecyclerVisitedObject
{
    // Called right after finish marking and this object is determined to be dead.
    // Should contain only simple clean up code.
    // Can't run another script
    // Can't cause a re-entrant collection
    STDMETHOD_(void, Finalize)(bool isShutdown) = 0;

    // Call after sweeping is done.
    // Can call other script or cause another collection.
    STDMETHOD_(void, Dispose)(bool isShutdown) = 0;

    // Used only by TrackableObjects (created with TrackedBit on by RecyclerNew*Tracked)
    STDMETHOD_(void, Mark)(RecyclerHeapHandle recycler) = 0;

    // Special behavior on certain GC's
    STDMETHOD_(void, OnMark)() = 0;

    // Used only by RecyclerVisitedHost objects (created with RecyclerAllocVistedHost_Traced*)
    STDMETHOD_(void, Trace)(IRecyclerHeapMarkingContext* markingContext) = 0;
};

