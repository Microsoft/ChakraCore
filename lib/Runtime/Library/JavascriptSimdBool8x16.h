//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class JavascriptSIMDBool8x16 sealed : public RecyclableObject
    {
    private:
        SIMDValue value;

        DEFINE_VTABLE_CTOR(JavascriptSIMDBool8x16, RecyclableObject);

    public:
        class EntryInfo
        {
        public:
            static FunctionInfo ToString;

        };
        JavascriptSIMDBool8x16(StaticType *type);
        JavascriptSIMDBool8x16(SIMDValue *val, StaticType *type);

        static JavascriptSIMDBool8x16* AllocUninitialized(ScriptContext* requestContext);
        static JavascriptSIMDBool8x16* New(SIMDValue *val, ScriptContext* requestContext);
        static bool Is(Var instance);
        static JavascriptSIMDBool8x16* FromVar(Var aValue);

        __inline SIMDValue GetValue() { return value; }

        virtual BOOL GetPropertyReference(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        virtual BOOL GetProperty(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        virtual BOOL GetProperty(Var originalInstance, JavascriptString* propertyNameString, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        virtual RecyclableObject * CloneToScriptContext(ScriptContext* requestContext) override;

        static size_t GetOffsetOfValue() { return offsetof(JavascriptSIMDBool8x16, value); }


        // Entry Points
        /*
        There is one toString per SIMD type. The code is entrant from value objects (e.g. a.toString()) or on arithmetic operations.
        It will also be a property of SIMD.*.prototype for SIMD dynamic objects.
        */
        static Var EntryToString(RecyclableObject* function, CallInfo callInfo, ...);
        // End Entry Points

        static void ToStringBuffer(SIMDValue& value, __out_ecount(countBuffer) char16* stringBuffer, size_t countBuffer, ScriptContext* scriptContext = nullptr)
        {
            swprintf_s(stringBuffer, countBuffer, _u("SIMD.Bool8x16(%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)"), \
                value.i8[0] ? _u("true") : _u("false"), value.i8[1] ? _u("true") : _u("false"), value.i8[2] ? _u("true") : _u("false"), value.i8[3] ? _u("true") : _u("false"), \
                value.i8[4] ? _u("true") : _u("false"), value.i8[5] ? _u("true") : _u("false"), value.i8[6] ? _u("true") : _u("false"), value.i8[7] ? _u("true") : _u("false"), \
                value.i8[8] ? _u("true") : _u("false"), value.i8[9] ? _u("true") : _u("false"), value.i8[10] ? _u("true") : _u("false"), value.i8[11] ? _u("true") : _u("false"), \
                value.i8[12] ? _u("true") : _u("false"), value.i8[13] ? _u("true") : _u("false"), value.i8[14] ? _u("true") : _u("false"), value.i8[15] ? _u("true") : _u("false")
                );
        }

        Var  Copy(ScriptContext* requestContext);

    private:
        bool GetPropertyBuiltIns(PropertyId propertyId, Var* value, ScriptContext* requestContext);
    };
}

