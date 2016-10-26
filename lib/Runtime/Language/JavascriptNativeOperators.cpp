//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeLanguagePch.h"
#include "JavascriptNativeOperators.h"

namespace Js
{
#if ENABLE_NATIVE_CODEGEN
    void * JavascriptNativeOperators::Op_SwitchStringLookUp_OOP(JavascriptString* str, Js::BranchDictionaryWrapper<JavascriptString*, OOPJITBranchDictAllocator>* branchTargets, uintptr_t funcStart, uintptr_t funcEnd)
    {
        void* defaultTarget = branchTargets->defaultTarget;
        Js::BranchDictionaryWrapper<JavascriptString*, OOPJITBranchDictAllocator>::BranchDictionary& stringDictionary = branchTargets->dictionary;
        void* target = stringDictionary.Lookup(str, defaultTarget);
        uintptr_t utarget = (uintptr_t)target;

        if ((utarget - funcStart) > (funcEnd - funcStart))
        {
            AssertMsg(false, "Switch string dictionary jump target outside of function");
            Throw::FatalInternalError();
        }
        return target;
    }

    void * JavascriptNativeOperators::Op_SwitchStringLookUp(JavascriptString* str, Js::BranchDictionaryWrapper<JavascriptString*, NativeCodeDataNoFixup::Allocator>* branchTargets, uintptr_t funcStart, uintptr_t funcEnd)
    {
        void* defaultTarget = branchTargets->defaultTarget;
        Js::BranchDictionaryWrapper<JavascriptString*, NativeCodeDataNoFixup::Allocator>::BranchDictionary& stringDictionary = branchTargets->dictionary;
        void* target = stringDictionary.Lookup(str, defaultTarget);
        uintptr_t utarget = (uintptr_t)target;

        if ((utarget - funcStart) > (funcEnd - funcStart))
        {
            AssertMsg(false, "Switch string dictionary jump target outside of function");
            Throw::FatalInternalError();
        }
        return target;
    }
#endif
};
