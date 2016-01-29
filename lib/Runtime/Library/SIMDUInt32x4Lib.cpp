//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeLibraryPch.h"

#if ENABLE_NATIVE_CODEGEN
namespace Js
{

    // Q: Are we allowed to call this as a constructor ?
    Var SIMDUint32x4Lib::EntryUint32x4(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();
        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        Var undefinedVar = scriptContext->GetLibrary()->GetUndefined();

        unsigned int intSIMDX = JavascriptConversion::ToUInt32(args.Info.Count >= 2 ? args[1] : undefinedVar, scriptContext);
        unsigned int intSIMDY = JavascriptConversion::ToUInt32(args.Info.Count >= 3 ? args[2] : undefinedVar, scriptContext);
        unsigned int intSIMDZ = JavascriptConversion::ToUInt32(args.Info.Count >= 4 ? args[3] : undefinedVar, scriptContext);
        unsigned int intSIMDW = JavascriptConversion::ToUInt32(args.Info.Count >= 5 ? args[4] : undefinedVar, scriptContext);

        SIMDValue lanes = SIMDUint32x4Operation::OpUInt32x4(intSIMDX, intSIMDY, intSIMDZ, intSIMDW);

        return JavascriptSIMDUint32x4::New(&lanes, scriptContext);
    }

    Var SIMDUint32x4Lib::EntryCheck(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();
        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        if (args.Info.Count >= 2 && JavascriptSIMDUint32x4::Is(args[1]))
        {
            return args[1];
        }
        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"UInt32x4");
    }

    Var SIMDUint32x4Lib::EntrySplat(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();
        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        Var undefinedVar = scriptContext->GetLibrary()->GetUndefined();
        int value = JavascriptConversion::ToUInt32(args.Info.Count >= 2 ? args[1] : undefinedVar, scriptContext);

        SIMDValue lanes = SIMDUint32x4Operation::OpSplat(value);

        return JavascriptSIMDUint32x4::New(&lanes, scriptContext);
    }

    Var SIMDUint32x4Lib::EntryFromFloat32x4(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);
        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        if (args.Info.Count >= 2 && JavascriptSIMDFloat32x4::Is(args[1]))
        {
            JavascriptSIMDFloat32x4 *instance = JavascriptSIMDFloat32x4::FromVar(args[1]);
            Assert(instance);

            SIMDValue input = instance->GetValue();
            bool throws = false;
            SIMDValue result = SIMDUint32x4Operation::OpFromFloat32x4(input, throws);

            // out of range
            if (throws)
            {
                JavascriptError::ThrowRangeError(scriptContext, JSERR_ArgumentOutOfRange, L"SIMD.UInt32x4.FromFloat32x4");
            }
            return JavascriptSIMDUint32x4::New(&result, scriptContext);

        }
        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"fromFloat32x4");
    }

    Var SIMDUint32x4Lib::EntryFromFloat32x4Bits(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);
        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        if (args.Info.Count >= 2 && JavascriptSIMDFloat32x4::Is(args[1]))
        {
            JavascriptSIMDFloat32x4 *instance = JavascriptSIMDFloat32x4::FromVar(args[1]);
            Assert(instance);
            return SIMDConvertTypeFromBits<JavascriptSIMDFloat32x4, JavascriptSIMDUint32x4>(instance, scriptContext);

        }
        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdFloat32x4TypeMismatch, L"fromFloat32x4Bits");
    }

    Var SIMDUint32x4Lib::EntryFromInt32x4Bits(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);
        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        if (args.Info.Count >= 2 && JavascriptSIMDInt32x4::Is(args[1]))
        {
            JavascriptSIMDInt32x4 *instance = JavascriptSIMDInt32x4::FromVar(args[1]);
            Assert(instance);

            return  SIMDConvertTypeFromBits<JavascriptSIMDInt32x4, JavascriptSIMDUint32x4>(instance, scriptContext);
        }
        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdInt32x4TypeMismatch, L"fromInt32x4Bits");
    }

    Var SIMDUint32x4Lib::EntryFromInt16x8Bits(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);
        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        if (args.Info.Count >= 2 && JavascriptSIMDInt16x8::Is(args[1]))
        {
            JavascriptSIMDInt16x8 *instance = JavascriptSIMDInt16x8::FromVar(args[1]);
            Assert(instance);

            return  SIMDConvertTypeFromBits<JavascriptSIMDInt16x8, JavascriptSIMDUint32x4>(instance, scriptContext);
        }
        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdInt32x4TypeMismatch, L"fromInt16x8Bits");
    }

    Var SIMDUint32x4Lib::EntryFromInt8x16Bits(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);
        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        if (args.Info.Count >= 2 && JavascriptSIMDInt8x16::Is(args[1]))
        {
            JavascriptSIMDInt8x16 *instance = JavascriptSIMDInt8x16::FromVar(args[1]);
            Assert(instance);

            return  SIMDConvertTypeFromBits<JavascriptSIMDInt8x16, JavascriptSIMDUint32x4>(instance, scriptContext);
        }
        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdInt32x4TypeMismatch, L"fromInt8x16Bits");
    }
    // Enable with UInt16x8 and UInt8x16 implementation

    Var SIMDUint32x4Lib::EntryFromUint16x8Bits(RecyclableObject* function, CallInfo callInfo, ...)
    {
#if 0
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);
        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        if (args.Info.Count >= 2 && JavascriptSIMDUInt16x8::Is(args[1]))
        {
            JavascriptSIMDUInt16x8 *instance = JavascriptSIMDUInt16x8::FromVar(args[1]);
            Assert(instance);

            return SIMDConvertTypeFromBits<JavascriptSIMDInt16x8, JavascriptSIMDUint32x4>(instance, scriptContext);
        }
        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdInt32x4TypeMismatch, L"fromUInt16x8Bits");
#endif
        return NULL;
    }

    Var SIMDUint32x4Lib::EntryFromUint8x16Bits(RecyclableObject* function, CallInfo callInfo, ...)
    {
#if 0
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);
        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        if (args.Info.Count >= 2 && JavascriptSIMDUInt8x16::Is(args[1]))
        {
            JavascriptSIMDUInt8x16 *instance = JavascriptSIMDUInt8x16::FromVar(args[1]);
            Assert(instance);

            return SIMDConvertTypeFromBits<JavascriptSIMDInt8x16, JavascriptSIMDUint32x4>(instance, scriptContext);
        }
        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdInt32x4TypeMismatch, L"fromUInt8x16Bits");
#endif
        return NULL;
    }


    //Lane Access
    Var SIMDUint32x4Lib::EntryExtractLane(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        // first arg has to be of type UInt32x4, so cannot be missing.
        if (args.Info.Count >= 3 && JavascriptSIMDUint32x4::Is(args[1]))
        {
            // if value arg is missing, then it is undefined.
            Var laneVar = args.Info.Count >= 3 ? args[2] : scriptContext->GetLibrary()->GetUndefined();
            uint result = SIMD128ExtractLane<JavascriptSIMDUint32x4, 4, uint>(args[1], laneVar, scriptContext);

            return JavascriptNumber::ToVarNoCheck((double)result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"ExtractLane");
    }

    Var SIMDUint32x4Lib::EntryReplaceLane(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        // first arg has to be of type UInt32x4, so cannot be missing.
        if (args.Info.Count >= 4 && JavascriptSIMDUint32x4::Is(args[1]))
        {
            // if value arg is missing, then it is undefined.
            Var laneVar = args.Info.Count >= 4 ? args[2] : scriptContext->GetLibrary()->GetUndefined();
            Var argVal = args.Info.Count >= 4 ? args[3] : scriptContext->GetLibrary()->GetUndefined();
            uint value = JavascriptConversion::ToUInt32(argVal, scriptContext);

            SIMDValue result = SIMD128ReplaceLane<JavascriptSIMDUint32x4, 4, uint>(args[1], laneVar, value, scriptContext);

            return JavascriptSIMDUint32x4::New(&result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"ReplaceLane");
    }

    Var SIMDUint32x4Lib::EntryNot(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        if (args.Info.Count >= 2 && JavascriptSIMDUint32x4::Is(args[1]))
        {
            JavascriptSIMDUint32x4 *a = JavascriptSIMDUint32x4::FromVar(args[1]);
            Assert(a);

            SIMDValue value, result;

            value = a->GetValue();
            result = SIMDInt32x4Operation::OpNot(value);

            return JavascriptSIMDUint32x4::New(&result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"not");
    }

    Var SIMDUint32x4Lib::EntryAdd(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        // If any of the args are missing, then it is Undefined type which causes TypeError exception.
        if (args.Info.Count >= 3 && JavascriptSIMDUint32x4::Is(args[1]) && JavascriptSIMDUint32x4::Is(args[2]))
        {
            JavascriptSIMDUint32x4 *a = JavascriptSIMDUint32x4::FromVar(args[1]);
            JavascriptSIMDUint32x4 *b = JavascriptSIMDUint32x4::FromVar(args[2]);
            Assert(a && b);

            SIMDValue result, aValue, bValue;

            aValue = a->GetValue();
            bValue = b->GetValue();
            result = SIMDInt32x4Operation::OpAdd(aValue, bValue);

            return JavascriptSIMDUint32x4::New(&result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"add");
    }

    Var SIMDUint32x4Lib::EntrySub(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        // If any of the args are missing, then it is Undefined type which causes TypeError exception.
        if (args.Info.Count >= 3 && JavascriptSIMDUint32x4::Is(args[1]) && JavascriptSIMDUint32x4::Is(args[2]))
        {
            JavascriptSIMDUint32x4 *a = JavascriptSIMDUint32x4::FromVar(args[1]);
            JavascriptSIMDUint32x4 *b = JavascriptSIMDUint32x4::FromVar(args[2]);
            Assert(a && b);

            SIMDValue result, aValue, bValue;

            aValue = a->GetValue();
            bValue = b->GetValue();
            result = SIMDInt32x4Operation::OpSub(aValue, bValue);

            return JavascriptSIMDUint32x4::New(&result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"sub");
    }

    Var SIMDUint32x4Lib::EntryMul(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        // If any of the args are missing, then it is Undefined type which causes TypeError exception.
        if (args.Info.Count >= 3 && JavascriptSIMDUint32x4::Is(args[1]) && JavascriptSIMDUint32x4::Is(args[2]))
        {
            JavascriptSIMDUint32x4 *a = JavascriptSIMDUint32x4::FromVar(args[1]);
            JavascriptSIMDUint32x4 *b = JavascriptSIMDUint32x4::FromVar(args[2]);
            Assert(a && b);

            SIMDValue result, aValue, bValue;

            aValue = a->GetValue();
            bValue = b->GetValue();
            result = SIMDInt32x4Operation::OpMul(aValue, bValue);

            return JavascriptSIMDUint32x4::New(&result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"mul");
    }

    Var SIMDUint32x4Lib::EntryAnd(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        // If any of the args are missing, then it is Undefined type which causes TypeError exception.
        if (args.Info.Count >= 3 && JavascriptSIMDUint32x4::Is(args[1]) && JavascriptSIMDUint32x4::Is(args[2]))
        {
            JavascriptSIMDUint32x4 *a = JavascriptSIMDUint32x4::FromVar(args[1]);
            JavascriptSIMDUint32x4 *b = JavascriptSIMDUint32x4::FromVar(args[2]);
            Assert(a && b);

            SIMDValue result, aValue, bValue;

            aValue = a->GetValue();
            bValue = b->GetValue();
            result = SIMDInt32x4Operation::OpAnd(aValue, bValue);

            return JavascriptSIMDUint32x4::New(&result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"and");
    }

    Var SIMDUint32x4Lib::EntryOr(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        // If any of the args are missing, then it is Undefined type which causes TypeError exception.
        if (args.Info.Count >= 3 && JavascriptSIMDUint32x4::Is(args[1]) && JavascriptSIMDUint32x4::Is(args[2]))
        {
            JavascriptSIMDUint32x4 *a = JavascriptSIMDUint32x4::FromVar(args[1]);
            JavascriptSIMDUint32x4 *b = JavascriptSIMDUint32x4::FromVar(args[2]);
            Assert(a && b);

            SIMDValue result, aValue, bValue;

            aValue = a->GetValue();
            bValue = b->GetValue();
            result = SIMDInt32x4Operation::OpOr(aValue, bValue);

            return JavascriptSIMDUint32x4::New(&result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"or");
    }

    Var SIMDUint32x4Lib::EntryXor(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        // If any of the args are missing, then it is Undefined type which causes TypeError exception.
        if (args.Info.Count >= 3 && JavascriptSIMDUint32x4::Is(args[1]) && JavascriptSIMDUint32x4::Is(args[2]))
        {
            JavascriptSIMDUint32x4 *a = JavascriptSIMDUint32x4::FromVar(args[1]);
            JavascriptSIMDUint32x4 *b = JavascriptSIMDUint32x4::FromVar(args[2]);
            Assert(a && b);

            SIMDValue result, aValue, bValue;

            aValue = a->GetValue();
            bValue = b->GetValue();
            result = SIMDInt32x4Operation::OpXor(aValue, bValue);

            return JavascriptSIMDUint32x4::New(&result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"xor");
    }

    Var SIMDUint32x4Lib::EntryMin(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        // If any of the args are missing, then it is Undefined type which causes TypeError exception.
        // strict type on both operands
        if (args.Info.Count >= 3 && JavascriptSIMDUint32x4::Is(args[1]) && JavascriptSIMDUint32x4::Is(args[2]))
        {
            JavascriptSIMDUint32x4 *a = JavascriptSIMDUint32x4::FromVar(args[1]);
            JavascriptSIMDUint32x4 *b = JavascriptSIMDUint32x4::FromVar(args[2]);
            Assert(a && b);

            SIMDValue result, aValue, bValue;

            aValue = a->GetValue();
            bValue = b->GetValue();

            result = SIMDUint32x4Operation::OpMin(aValue, bValue);

            return JavascriptSIMDUint32x4::New(&result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"lessThan");
    }

    Var SIMDUint32x4Lib::EntryMax(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        // If any of the args are missing, then it is Undefined type which causes TypeError exception.
        // strict type on both operands
        if (args.Info.Count >= 3 && JavascriptSIMDUint32x4::Is(args[1]) && JavascriptSIMDUint32x4::Is(args[2]))
        {
            JavascriptSIMDUint32x4 *a = JavascriptSIMDUint32x4::FromVar(args[1]);
            JavascriptSIMDUint32x4 *b = JavascriptSIMDUint32x4::FromVar(args[2]);
            Assert(a && b);

            SIMDValue result, aValue, bValue;

            aValue = a->GetValue();
            bValue = b->GetValue();

            result = SIMDUint32x4Operation::OpMax(aValue, bValue);

            return JavascriptSIMDUint32x4::New(&result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"lessThan");
    }

    Var SIMDUint32x4Lib::EntryLessThan(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        // If any of the args are missing, then it is Undefined type which causes TypeError exception.
        // strict type on both operands
        if (args.Info.Count >= 3 && JavascriptSIMDUint32x4::Is(args[1]) && JavascriptSIMDUint32x4::Is(args[2]))
        {
            JavascriptSIMDUint32x4 *a = JavascriptSIMDUint32x4::FromVar(args[1]);
            JavascriptSIMDUint32x4 *b = JavascriptSIMDUint32x4::FromVar(args[2]);
            Assert(a && b);

            SIMDValue result, aValue, bValue;

            aValue = a->GetValue();
            bValue = b->GetValue();

            result = SIMDUint32x4Operation::OpLessThan(aValue, bValue);

            return JavascriptSIMDBool32x4::New(&result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"lessThan");
    }

    Var SIMDUint32x4Lib::EntryLessThanOrEqual(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        // If any of the args are missing, then it is Undefined type which causes TypeError exception.
        // strict type on both operands
        if (args.Info.Count >= 3 && JavascriptSIMDUint32x4::Is(args[1]) && JavascriptSIMDUint32x4::Is(args[2]))
        {
            JavascriptSIMDUint32x4 *a = JavascriptSIMDUint32x4::FromVar(args[1]);
            JavascriptSIMDUint32x4 *b = JavascriptSIMDUint32x4::FromVar(args[2]);
            Assert(a && b);

            SIMDValue result, aValue, bValue;

            aValue = a->GetValue();
            bValue = b->GetValue();

            result = SIMDUint32x4Operation::OpLessThanOrEqual(aValue, bValue);

            return JavascriptSIMDBool32x4::New(&result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"lessThanOrEqual");
    }

    Var SIMDUint32x4Lib::EntryEqual(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        // If any of the args are missing, then it is Undefined type which causes TypeError exception.
        // strict type on both operands
        if (args.Info.Count >= 3 && JavascriptSIMDUint32x4::Is(args[1]) && JavascriptSIMDUint32x4::Is(args[2]))
        {
            JavascriptSIMDUint32x4 *a = JavascriptSIMDUint32x4::FromVar(args[1]);
            JavascriptSIMDUint32x4 *b = JavascriptSIMDUint32x4::FromVar(args[2]);
            Assert(a && b);

            SIMDValue result, aValue, bValue;

            aValue = a->GetValue();
            bValue = b->GetValue();

            result = SIMDInt32x4Operation::OpEqual(aValue, bValue);

            return JavascriptSIMDBool32x4::New(&result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"equal");
    }

    Var SIMDUint32x4Lib::EntryNotEqual(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        // If any of the args are missing, then it is Undefined type which causes TypeError exception.
        // strict type on both operands
        if (args.Info.Count >= 3 && JavascriptSIMDUint32x4::Is(args[1]) && JavascriptSIMDUint32x4::Is(args[2]))
        {
            JavascriptSIMDUint32x4 *a = JavascriptSIMDUint32x4::FromVar(args[1]);
            JavascriptSIMDUint32x4 *b = JavascriptSIMDUint32x4::FromVar(args[2]);
            Assert(a && b);

            SIMDValue result, aValue, bValue;

            aValue = a->GetValue();
            bValue = b->GetValue();

            result = SIMDInt32x4Operation::OpEqual(aValue, bValue);
            result = SIMDInt32x4Operation::OpNot(result);
            return JavascriptSIMDBool32x4::New(&result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"notEqual");
    }

    Var SIMDUint32x4Lib::EntryGreaterThan(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        // If any of the args are missing, then it is Undefined type which causes TypeError exception.
        // strict type on both operands
        if (args.Info.Count >= 3 && JavascriptSIMDUint32x4::Is(args[1]) && JavascriptSIMDUint32x4::Is(args[2]))
        {
            JavascriptSIMDUint32x4 *a = JavascriptSIMDUint32x4::FromVar(args[1]);
            JavascriptSIMDUint32x4 *b = JavascriptSIMDUint32x4::FromVar(args[2]);
            Assert(a && b);

            SIMDValue result, aValue, bValue;

            aValue = a->GetValue();
            bValue = b->GetValue();

            result = SIMDUint32x4Operation::OpLessThanOrEqual(aValue, bValue);
            result = SIMDInt32x4Operation::OpNot(result);
            return JavascriptSIMDBool32x4::New(&result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"greaterThan");
    }

    Var SIMDUint32x4Lib::EntryGreaterThanOrEqual(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        // If any of the args are missing, then it is Undefined type which causes TypeError exception.
        // strict type on both operands
        if (args.Info.Count >= 3 && JavascriptSIMDUint32x4::Is(args[1]) && JavascriptSIMDUint32x4::Is(args[2]))
        {
            JavascriptSIMDUint32x4 *a = JavascriptSIMDUint32x4::FromVar(args[1]);
            JavascriptSIMDUint32x4 *b = JavascriptSIMDUint32x4::FromVar(args[2]);
            Assert(a && b);

            SIMDValue result, aValue, bValue;

            aValue = a->GetValue();
            bValue = b->GetValue();

            result = SIMDUint32x4Operation::OpLessThan(aValue, bValue);
            result = SIMDInt32x4Operation::OpNot(result);
            return JavascriptSIMDBool32x4::New(&result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"greaterThanOrEqual");
    }

    Var SIMDUint32x4Lib::EntrySwizzle(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        // If any of the args are missing, then it is Undefined type which causes TypeError exception.
        // strict type on both operands
        if (args.Info.Count >= 2 && JavascriptSIMDUint32x4::Is(args[1]))
        {
            // type check on lane indices
            if (args.Info.Count < 6)
            {
                // missing lane args
                JavascriptError::ThrowTypeError(scriptContext, JSERR_NeedNumber, L"Lane index");
            }

            Var lane0 = args[2];
            Var lane1 = args[3];
            Var lane2 = args[4];
            Var lane3 = args[5];

            return SIMD128SlowShuffle<JavascriptSIMDUint32x4>(args[1], args[1], lane0, lane1, lane2, lane3, 4, scriptContext);
        }
        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"swizzle");
    }

    Var SIMDUint32x4Lib::EntryShuffle(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        // If any of the args are missing, then it is Undefined type which causes TypeError exception.
        // strict type on both operands
        if (args.Info.Count >= 3 && JavascriptSIMDUint32x4::Is(args[1]) && JavascriptSIMDUint32x4::Is(args[2]))
        {
            // type check on lane indices
            if (args.Info.Count < 7)
            {
                // missing lane args
                JavascriptError::ThrowTypeError(scriptContext, JSERR_NeedNumber, L"Lane index");
            }

            Var lane0 = args[3];
            Var lane1 = args[4];
            Var lane2 = args[5];
            Var lane3 = args[6];

            return SIMD128SlowShuffle<JavascriptSIMDUint32x4>(args[1], args[2], lane0, lane1, lane2, lane3, 8, scriptContext);

        }
        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"shuffle");
    }

    Var SIMDUint32x4Lib::EntryShiftLeftByScalar(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        if (args.Info.Count >= 3 && JavascriptSIMDUint32x4::Is(args[1]))
        {
            JavascriptSIMDUint32x4 *a = JavascriptSIMDUint32x4::FromVar(args[1]);
            Assert(a);

            SIMDValue result, aValue;

            aValue = a->GetValue();
            Var countVar = args[2]; // {int} bits Bit count
            int32 count = JavascriptConversion::ToInt32(countVar, scriptContext);

            result = SIMDInt32x4Operation::OpShiftLeftByScalar(aValue, count);

            return JavascriptSIMDUint32x4::New(&result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"shiftLeftByScalar");
    }

    Var SIMDUint32x4Lib::EntryShiftRightByScalar(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        if (args.Info.Count >= 3 && JavascriptSIMDUint32x4::Is(args[1]))
        {
            JavascriptSIMDUint32x4 *a = JavascriptSIMDUint32x4::FromVar(args[1]);
            Assert(a);

            SIMDValue result, aValue;

            aValue = a->GetValue();
            Var countVar = args[2]; // {int} bits Bit count
            int32 count = JavascriptConversion::ToInt32(countVar, scriptContext);

            result = SIMDUint32x4Operation::OpShiftRightByScalar(aValue, count);

            return JavascriptSIMDUint32x4::New(&result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"shiftRightByScalar");
    }

    Var SIMDUint32x4Lib::EntrySelect(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        if (args.Info.Count >= 4 && JavascriptSIMDBool32x4::Is(args[1]) &&
            JavascriptSIMDUint32x4::Is(args[2]) && JavascriptSIMDUint32x4::Is(args[3]))
        {
            JavascriptSIMDBool32x4 *m = JavascriptSIMDBool32x4::FromVar(args[1]);
            JavascriptSIMDUint32x4 *t = JavascriptSIMDUint32x4::FromVar(args[2]);
            JavascriptSIMDUint32x4 *f = JavascriptSIMDUint32x4::FromVar(args[3]);
            Assert(m && t && f);

            SIMDValue result, maskValue, trueValue, falseValue;

            maskValue = m->GetValue();
            trueValue = t->GetValue();
            falseValue = f->GetValue();

            result = SIMDInt32x4Operation::OpSelect(maskValue, trueValue, falseValue);

            return JavascriptSIMDUint32x4::New(&result, scriptContext);
        }

        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdUint32x4TypeMismatch, L"select");
    }

    Var SIMDUint32x4Lib::EntryLoad(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        return SIMD128TypedArrayLoad<JavascriptSIMDUint32x4>(args[1], args[2], 4 * TySize[TyInt32], scriptContext);
    }

    Var SIMDUint32x4Lib::EntryLoad1(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        return SIMD128TypedArrayLoad<JavascriptSIMDUint32x4>(args[1], args[2], 1 * TySize[TyInt32], scriptContext);
    }

    Var SIMDUint32x4Lib::EntryLoad2(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        return SIMD128TypedArrayLoad<JavascriptSIMDUint32x4>(args[1], args[2], 2 * TySize[TyInt32], scriptContext);
    }

    Var SIMDUint32x4Lib::EntryLoad3(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        return SIMD128TypedArrayLoad<JavascriptSIMDUint32x4>(args[1], args[2], 3 * TySize[TyInt32], scriptContext);
    }

    Var SIMDUint32x4Lib::EntryStore(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        if (args.Info.Count >= 4 && JavascriptSIMDUint32x4::Is(args[3]))
        {
            SIMD128TypedArrayStore<JavascriptSIMDUint32x4>(args[1], args[2], args[3], 4 * TySize[TyInt32], scriptContext);
            return NULL;
        }
        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdInvalidArgType, L"SIMD.UInt32x4.store");
    }

    Var SIMDUint32x4Lib::EntryStore1(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        if (args.Info.Count >= 4 && JavascriptSIMDUint32x4::Is(args[3]))
        {
            SIMD128TypedArrayStore<JavascriptSIMDUint32x4>(args[1], args[2], args[3], 1 * TySize[TyInt32], scriptContext);
            return NULL;
        }
        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdInvalidArgType, L"SIMD.UInt32x4.store");
    }

    Var SIMDUint32x4Lib::EntryStore2(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        if (args.Info.Count >= 4 && JavascriptSIMDUint32x4::Is(args[3]))
        {
            SIMD128TypedArrayStore<JavascriptSIMDUint32x4>(args[1], args[2], args[3], 2 * TySize[TyInt32], scriptContext);
            return NULL;
        }
        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdInvalidArgType, L"SIMD.UInt32x4.store");
    }

    Var SIMDUint32x4Lib::EntryStore3(RecyclableObject* function, CallInfo callInfo, ...)
    {
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

        ARGUMENTS(args, callInfo);
        ScriptContext* scriptContext = function->GetScriptContext();

        AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");
        Assert(!(callInfo.Flags & CallFlags_New));

        if (args.Info.Count >= 4 && JavascriptSIMDUint32x4::Is(args[3]))
        {
            SIMD128TypedArrayStore<JavascriptSIMDUint32x4>(args[1], args[2], args[3], 3 * TySize[TyInt32], scriptContext);
            return NULL;
        }
        JavascriptError::ThrowTypeError(scriptContext, JSERR_SimdInvalidArgType, L"SIMD.UInt32x4.store");
    }

}
#endif
