//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

class JavascriptSIMDFloat32x4;
class JavascriptSIMDInt32x4;
class JavascriptSIMDInt16x8;

namespace Js
{
    class JavascriptSIMDInt8x16 sealed : public JavascriptSIMDType
    {
    private:
        SIMDValue value;

        DEFINE_VTABLE_CTOR(JavascriptSIMDInt8x16, JavascriptSIMDType);

    public:
        class EntryInfo
        {
        public:
            static FunctionInfo ToString;
            static FunctionInfo ToLocaleString;
            static FunctionInfo ValueOf;
            static FunctionInfo SymbolToPrimitive;
        };

        JavascriptSIMDInt8x16(SIMDValue *val, StaticType *type);
        static JavascriptSIMDInt8x16* New(SIMDValue *val, ScriptContext* requestContext);
        static bool Is(Var instance);
        static JavascriptSIMDInt8x16* FromVar(Var aValue);
        static const wchar_t* GetFullBuiltinName(wchar_t** aBuffer, const wchar_t* name);
        static Var CallToLocaleString(RecyclableObject& obj, ScriptContext& requestContext, SIMDValue simdValue, const Var* args, uint numArgs, CallInfo callInfo);

        static void ToStringBuffer(SIMDValue& value, __out_ecount(countBuffer) wchar_t* stringBuffer, size_t countBuffer, ScriptContext* scriptContext = nullptr)
        {
            swprintf_s(stringBuffer, countBuffer, L"SIMD.Int8x16(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
                value.i8[0], value.i8[1], value.i8[2], value.i8[3], value.i8[4], value.i8[5], value.i8[6], value.i8[7],
                value.i8[8], value.i8[9], value.i8[10], value.i8[11], value.i8[12], value.i8[13], value.i8[14], value.i8[15]);
        }
        
        virtual RecyclableObject * CloneToScriptContext(ScriptContext* requestContext) override;

        __inline SIMDValue GetValue() { return value; }
        Var  Copy(ScriptContext* requestContext);
    };
}
