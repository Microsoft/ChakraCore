//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class JavascriptSIMDUint32x4;

    class JavascriptSIMDInt32x4 sealed : public RecyclableObject
    {
    private:
        SIMDValue value;

        DEFINE_VTABLE_CTOR(JavascriptSIMDInt32x4, RecyclableObject);

    public:
        class EntryInfo
        {
        public:
            static FunctionInfo ToString;
            static FunctionInfo Bool;
        };

        JavascriptSIMDInt32x4(StaticType *type);
        JavascriptSIMDInt32x4(SIMDValue *val, StaticType *type);

        static JavascriptSIMDInt32x4* AllocUninitialized(ScriptContext* requestContext);
        static JavascriptSIMDInt32x4* New(SIMDValue *val, ScriptContext* requestContext);
        static bool Is(Var instance);
        static JavascriptSIMDInt32x4* FromVar(Var aValue);

        static JavascriptSIMDInt32x4* FromBool(SIMDValue *val, ScriptContext* requestContext);
        static JavascriptSIMDInt32x4* FromFloat64x2(JavascriptSIMDFloat64x2 *instance, ScriptContext* requestContext);

        __inline SIMDValue GetValue() { return value; }

        virtual BOOL GetPropertyReference(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        virtual BOOL GetProperty(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        virtual BOOL GetProperty(Var originalInstance, JavascriptString* propertyNameString, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        virtual RecyclableObject * CloneToScriptContext(ScriptContext* requestContext) override;
        static size_t GetOffsetOfValue() { return offsetof(JavascriptSIMDInt32x4, value); }

        // Entry Points
        /*
        There is one toString per SIMD type. The code is entrant from value objects explicitly (e.g. a.toString()) or on overloaded operations.
        It will also be a property of SIMD.int32x4.prototype for SIMD dynamic objects.
        */
        static Var EntryToString(RecyclableObject* function, CallInfo callInfo, ...);
        // End Entry Points

        static void ToStringBuffer(SIMDValue& value, __out_ecount(countBuffer) char16* stringBuffer, size_t countBuffer, ScriptContext* scriptContext = nullptr)
        {
            swprintf_s(stringBuffer, countBuffer, _u("SIMD.Int32x4(%d, %d, %d, %d)"), value.i32[SIMD_X], value.i32[SIMD_Y], value.i32[SIMD_Z], value.i32[SIMD_W]);
        }

        Var  Copy(ScriptContext* requestContext);
        Var  CopyAndSetLaneFlag(uint index, BOOL value, ScriptContext* requestContext);

    private:
        bool GetPropertyBuiltIns(PropertyId propertyId, Var* value, ScriptContext* requestContext);
        Var  GetLaneAsFlag(uint index, ScriptContext* requestContext);
        Var  GetSignMask();
    };
}
