//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

namespace Js
{
    class WebAssemblyTable : public DynamicObject
    {
#ifdef ENABLE_WASM
    public:
        class EntryInfo
        {
        public:
            static FunctionInfo NewInstance;
            static FunctionInfo GetterLength;
            static FunctionInfo Grow;
            static FunctionInfo Get;
            static FunctionInfo Set;
        };
        WebAssemblyTable(Var * values, uint32 length, uint32 maxLength, DynamicType * type);
        static Var NewInstance(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryGetterLength(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryGrow(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryGet(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntrySet(RecyclableObject* function, CallInfo callInfo, ...);

        static bool Is(Var aValue);
        static WebAssemblyTable * FromVar(Var aValue);

        static WebAssemblyTable * Create(uint32 initial, uint32 maximum, ScriptContext * scriptContext);
    private:
        uint32 m_maxLength;
        uint32 m_length;
        Var * m_values;
#endif
    };
}
