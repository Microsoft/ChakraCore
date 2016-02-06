//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

namespace Js
{
    // given bucket size, calculate how many pointers can be hold in AuxPtrFix structure
    constexpr uint8 CalcMaxCount(const uint8 size)
    {
        return (size - 1) / (1 + sizeof(void*));
    }
    // Use fixed size structure to save pointers 
    template<typename FieldsEnum, uint8 size, uint8 _MaxCount = CalcMaxCount(size)>
    struct AuxPtrsFix
    {
        static const uint8 MaxCount = _MaxCount;
        uint8 count;                            // always saving maxCount
        FieldsEnum type[MaxCount];                  // save instantiated pointer enum
        WriteBarrierPtr<void> ptr[MaxCount];    // save instantiated pointer address
        AuxPtrsFix();
        AuxPtrsFix(AuxPtrsFix<FieldsEnum, 16>* ptr16); // called when promoting from AuxPtrs16 to AuxPtrs32
        void* Get(FieldsEnum e);
        bool Set(FieldsEnum e, void* p);
    };

    // Use flexible size structure to save pointers. when pointer count exceeds AuxPtrsFix<FieldsEnum, 32>::MaxCount, 
    // it will promote to this structure to save the pointers
    template<class T, typename FieldsEnum>
    struct AuxPtrs
    {
        typedef AuxPtrsFix<FieldsEnum, 16> AuxPtrs16;
        typedef AuxPtrsFix<FieldsEnum, 32> AuxPtrs32;
        typedef AuxPtrs<T, FieldsEnum> AuxPtrsT;
        uint8 count;                            // save instantiated pointers count
        uint8 capacity;                         // save number of pointers can be hold in current instance of AuxPtrs
        FieldsEnum offsets[FieldsEnum::Max];       // save position of each instantiated pointers, if not instantiate, it's invalid
        WriteBarrierPtr<void> ptrs[1];          // instantiated pointer addresses
        AuxPtrs(uint8 capacity, AuxPtrs32* ptr32);  // called when promoting from AuxPtrs32 to AuxPtrs
        AuxPtrs(uint8 capacity, AuxPtrs* ptr);      // called when expanding (i.e. promoting from AuxPtrs to bigger AuxPtrs)
        void* Get(FieldsEnum e);
        bool Set(FieldsEnum e, void* p);
        static void AllocAuxPtrFix(T* _this, uint8 size, Recycler* recycler);
        static void AllocAuxPtr(T* _this, uint8 count, Recycler* recycler);
        static void* GetAuxPtr(const T* _this, FieldsEnum e);
        static void SetAuxPtr(T* _this, FieldsEnum e, void* ptr, Recycler* recycler);
    };


    template<typename FieldsEnum, uint8 size, uint8 _MaxCount>
    AuxPtrsFix<FieldsEnum, size, _MaxCount>::AuxPtrsFix()
    {
        static_assert(_MaxCount == AuxPtrsFix<FieldsEnum, 16>::MaxCount, "Should only be called on AuxPtrsFix<FieldsEnum, 16>");
        this->count = AuxPtrsFix<FieldsEnum, 16>::MaxCount;
        for (uint8 i = 0; i < count; i++)
        {
            this->type[i] = FieldsEnum::Invalid;
        }
    }

    template<typename FieldsEnum, uint8 size, uint8 _MaxCount>
    AuxPtrsFix<FieldsEnum, size, _MaxCount>::AuxPtrsFix(AuxPtrsFix<FieldsEnum, 16>* ptr16)
    {
        static_assert(_MaxCount == AuxPtrsFix<FieldsEnum, 32>::MaxCount, "Should only be called on AuxPtrsFix<FieldsEnum, 32>");
        this->count = AuxPtrsFix<FieldsEnum, 32>::MaxCount;
        for (uint8 i = 0; i < AuxPtrsFix<FieldsEnum, 16>::MaxCount; i++)
        {
            this->type[i] = ptr16->type[i];
            this->ptr[i] = ptr16->ptr[i];
        }
        for (uint8 i = AuxPtrsFix<FieldsEnum, 16>::MaxCount; i < count; i++)
        {
            this->type[i] = FieldsEnum::Invalid;
        }
    }
    template<typename FieldsEnum, uint8 size, uint8 _MaxCount>
    inline void* AuxPtrsFix<FieldsEnum, size, _MaxCount>::Get(FieldsEnum e)
    {
        Assert(count == _MaxCount);
        for (uint8 i = 0; i < _MaxCount; i++) // using _MaxCount instead of count so compiler can optimize in case _MaxCount is 1.
        {
            if (type[i] == e)
            {
                return ptr[i];
            }
        }
        return nullptr;
    }
    template<typename FieldsEnum, uint8 size, uint8 _MaxCount>
    inline bool AuxPtrsFix<FieldsEnum, size, _MaxCount>::Set(FieldsEnum e, void* p)
    {
        Assert(count == _MaxCount);
        for (uint8 i = 0; i < _MaxCount; i++)
        {
            if (type[i] == e || type[i] == FieldsEnum::Invalid)
            {
                ptr[i] = p;
                type[i] = e;
                return true;
            }
        }
        return false;
    }
    template<class T, typename FieldsEnum>
    AuxPtrs<T, FieldsEnum>::AuxPtrs(uint8 capacity, AuxPtrs32* ptr32)
    {
        Assert(ptr32->count >= AuxPtrs32::MaxCount);
        this->count = ptr32->count;
        this->capacity = capacity;
        memset(offsets, (uint8)FieldsEnum::Invalid, (uint8)FieldsEnum::Max);
        for (uint8 i = 0; i < ptr32->count; i++)
        {
            offsets[(uint8)ptr32->type[i]] = (FieldsEnum)i;
            ptrs[i] = ptr32->ptr[i];
        }
    }
    template<class T, typename FieldsEnum>
    AuxPtrs<T, FieldsEnum>::AuxPtrs(uint8 capacity, AuxPtrs* ptr)
    {
        memcpy(this, ptr, sizeof(AuxPtrs) + (ptr->count - 1)*sizeof(void*));
        this->capacity = capacity;
    }
    template<class T, typename FieldsEnum>
    inline void* AuxPtrs<T, FieldsEnum>::Get(FieldsEnum e)
    {
        uint8 u = (uint8)e;
        return offsets[u] == FieldsEnum::Invalid ? nullptr : ptrs[(uint8)offsets[u]];
    }
    template<class T, typename FieldsEnum>
    inline bool AuxPtrs<T, FieldsEnum>::Set(FieldsEnum e, void* p)
    {
        uint8 u = (uint8)e;
        if (offsets[u] != FieldsEnum::Invalid)
        {
            ptrs[(uint8)offsets[u]] = p;
            return true;
        }
        else
        {
            if (count == capacity)
            {
                // need to expand
                return false;
            }
            else
            {
                offsets[u] = (FieldsEnum)count++;
                ptrs[(uint8)offsets[u]] = p;
                return true;
            }
        }
    }

    template<class T, typename FieldsEnum>
    void AuxPtrs<T, FieldsEnum>::AllocAuxPtrFix(T* _this, uint8 size, Recycler* recycler)
    {
        if (size == 16)
        {
            _this->auxPtrs = (AuxPtrs<T, FieldsEnum>*)RecyclerNewWithBarrierStructZ(recycler, AuxPtrs16);
        }
        else if (size == 32)
        {
            _this->auxPtrs = (AuxPtrs<T, FieldsEnum>*)RecyclerNewWithBarrierPlusZ(recycler, 0, AuxPtrs32, (AuxPtrs16*)(void*)_this->auxPtrs);
        }
        else
        {
            Assert(false);
        }
    }

    template<class T, typename FieldsEnum>
    void AuxPtrs<T, FieldsEnum>::AllocAuxPtr(T* _this, uint8 count, Recycler* recycler)
    {
        Assert(count >= AuxPtrs32::MaxCount);
        auto requestSize = sizeof(AuxPtrs<T, FieldsEnum>) + (count - 1)*sizeof(void*);
        auto allocSize = ::Math::Align<uint8>((uint8)requestSize, 16);
        auto capacity = (uint8)((allocSize - offsetof(AuxPtrsT, ptrs)) / sizeof(void*));

        if (_this->auxPtrs->count != AuxPtrs32::MaxCount) // expanding
        {
            _this->auxPtrs = RecyclerNewWithBarrierPlusZ(recycler, allocSize - sizeof(AuxPtrsT), AuxPtrsT, capacity, _this->auxPtrs);
        }
        else // promoting from AuxPtrs32
        {
            _this->auxPtrs = RecyclerNewWithBarrierPlusZ(recycler, allocSize - sizeof(AuxPtrsT), AuxPtrsT, capacity, (AuxPtrs32*)(void*)_this->auxPtrs);
        }
    }

    template<class T, typename FieldsEnum>
    inline void* AuxPtrs<T, FieldsEnum>::GetAuxPtr(const T* _this, FieldsEnum e)
    {
        if (_this->auxPtrs == nullptr)
        {
            return nullptr;
        }
        if (_this->auxPtrs->count == AuxPtrs16::MaxCount)
        {
            return ((AuxPtrs16*)(void*)_this->auxPtrs)->Get(e);
        }
        if (_this->auxPtrs->count == AuxPtrs32::MaxCount)
        {
            return ((AuxPtrs32*)(void*)_this->auxPtrs)->Get(e);
        }
        return _this->auxPtrs->Get(e);
    }
    template<class T, typename FieldsEnum>
    void AuxPtrs<T, FieldsEnum>::SetAuxPtr(T* _this, FieldsEnum e, void* ptr, Recycler* recycler)
    {
        if (ptr == nullptr && GetAuxPtr(_this, e) == nullptr)
        {
            return;
        }
        if (_this->auxPtrs == nullptr)
        {
            AuxPtrs<FunctionProxy, FieldsEnum>::AllocAuxPtrFix(_this, 16, recycler);
            bool ret = ((AuxPtrs16*)(void*)_this->auxPtrs)->Set(e, ptr);
            Assert(ret);
            return;
        }
        if (_this->auxPtrs->count == AuxPtrs16::MaxCount)
        {
            bool ret = ((AuxPtrs16*)(void*)_this->auxPtrs)->Set(e, ptr);
            if (ret)
            {
                return;
            }
            else
            {
                AuxPtrs<FunctionProxy, FieldsEnum>::AllocAuxPtrFix(_this, 32, recycler);
            }
        }
        if (_this->auxPtrs->count == AuxPtrs32::MaxCount)
        {
            bool ret = ((AuxPtrs32*)(void*)_this->auxPtrs)->Set(e, ptr);
            if (ret)
            {
                return;
            }
            else
            {
                AuxPtrs<FunctionProxy, FieldsEnum>::AllocAuxPtr(_this, AuxPtrs32::MaxCount + 1, recycler);
            }
        }

        bool ret = _this->auxPtrs->Set(e, ptr);
        if (!ret)
        {
            AuxPtrs<FunctionProxy, FieldsEnum>::AllocAuxPtr(_this, _this->auxPtrs->count + 1, recycler);
            ret = _this->auxPtrs->Set(e, ptr);
            Assert(ret);
        }
    }
}
