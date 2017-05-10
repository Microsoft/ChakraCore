//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeLibraryPch.h"
#include "EngineInterfaceObject.h"
#include "JsBuiltInEngineInterfaceExtensionObject.h"
#include "Types/DeferredTypeHandler.h"

#ifdef ENABLE_JS_BUILTINS
#include "ByteCode/ByteCodeSerializer.h"
#include "errstr.h"
#include "ByteCode/ByteCodeDumper.h"

#pragma warning(push)
#pragma warning(disable:4309) // truncation of constant value
#pragma warning(disable:4838) // conversion from 'int' to 'const char' requires a narrowing conversion

#if DISABLE_JIT
#if TARGET_64
#include "JsBuiltIn/JsBuiltIn.js.nojit.bc.64b.h"
#else
#include "JsBuiltIn/JsBuiltIn.js.nojit.bc.32b.h"
#endif // TARGET_64
#else
#if TARGET_64
#include "JsBuiltIn/JsBuiltIn.js.bc.64b.h"
#else
#include "JsBuiltIn/JsBuiltIn.js.bc.32b.h"
#endif // TARGET_64
#endif // DISABLE_JIT

#pragma warning(pop)

#define IfFailAssertMsgAndThrowHr(op, msg) \
    if (FAILED(hr=(op))) \
    { \
    AssertMsg(false, msg); \
    JavascriptError::MapAndThrowError(scriptContext, hr); \
    } \

#endif // ENABLE_JS_BUILTINS

namespace Js
{
#ifdef ENABLE_JS_BUILTINS

    JsBuiltInEngineInterfaceExtensionObject::JsBuiltInEngineInterfaceExtensionObject(ScriptContext * scriptContext) :
        EngineExtensionObjectBase(EngineInterfaceExtensionKind_JsBuiltIn, scriptContext),
        jsBuiltInByteCode(nullptr),
        wasInitialized(false)
    {
    }

    NoProfileFunctionInfo JsBuiltInEngineInterfaceExtensionObject::EntryInfo::JsBuiltIn_RegisterFunction(FORCE_NO_WRITE_BARRIER_TAG(JsBuiltInEngineInterfaceExtensionObject::EntryJsBuiltIn_RegisterFunction));

    NoProfileFunctionInfo JsBuiltInEngineInterfaceExtensionObject::EntryInfo::JsBuiltIn_Internal_ToLengthFunction(FORCE_NO_WRITE_BARRIER_TAG(JsBuiltInEngineInterfaceExtensionObject::EntryJsBuiltIn_Internal_ToLengthFunction));
    NoProfileFunctionInfo JsBuiltInEngineInterfaceExtensionObject::EntryInfo::JsBuiltIn_Internal_ToIntegerFunction(FORCE_NO_WRITE_BARRIER_TAG(JsBuiltInEngineInterfaceExtensionObject::EntryJsBuiltIn_Internal_ToIntegerFunction));

    void JsBuiltInEngineInterfaceExtensionObject::Initialize()
    {
        if (wasInitialized)
        {
            return;
        }

        JavascriptLibrary* library = scriptContext->GetLibrary();
        DynamicObject* commonObject = library->GetEngineInterfaceObject()->GetCommonNativeInterfaces();
        if (scriptContext->IsJsBuiltInEnabled())
        {
            Assert(library->GetEngineInterfaceObject() != nullptr);
            builtInNativeInterfaces = DynamicObject::New(library->GetRecycler(),
                DynamicType::New(scriptContext, TypeIds_Object, commonObject, nullptr,
                    DeferredTypeHandler<InitializeJsBuiltInNativeInterfaces>::GetDefaultInstance()));
            library->AddMember(library->GetEngineInterfaceObject(), Js::PropertyIds::JsBuiltIn, builtInNativeInterfaces);
        }

        wasInitialized = true;
    }

    void JsBuiltInEngineInterfaceExtensionObject::InjectJsBuiltInLibraryCode(ScriptContext * scriptContext)
    {
        if (jsBuiltInByteCode != nullptr)
        {
            return;
        }

        try {
            EnsureJsBuiltInByteCode(scriptContext);
            Assert(jsBuiltInByteCode != nullptr);

            Js::ScriptFunction *function = scriptContext->GetLibrary()->CreateScriptFunction(jsBuiltInByteCode->GetNestedFunctionForExecution(0));

            // If we are profiling, we need to register the script to the profiler callback, so the script compiled event will be sent.
            if (scriptContext->IsProfiling())
            {
                scriptContext->RegisterScript(function->GetFunctionProxy());
            }
            // Mark we are profiling library code already, so that any initialization library code called here won't be reported to profiler
            AutoProfilingUserCode autoProfilingUserCode(scriptContext->GetThreadContext(), /*isProfilingUserCode*/false);

            Js::Var args[] = { scriptContext->GetLibrary()->GetUndefined(), scriptContext->GetLibrary()->GetEngineInterfaceObject() };
            Js::CallInfo callInfo(Js::CallFlags_Value, _countof(args));

            // Clear disable implicit call bit as initialization code doesn't have any side effect
            Js::ImplicitCallFlags saveImplicitCallFlags = scriptContext->GetThreadContext()->GetImplicitCallFlags();
            scriptContext->GetThreadContext()->ClearDisableImplicitFlags();
            JavascriptFunction::CallRootFunctionInScript(function, Js::Arguments(callInfo, args));
            scriptContext->GetThreadContext()->SetImplicitCallFlags((Js::ImplicitCallFlags)(saveImplicitCallFlags));

#if DBG_DUMP
            if (PHASE_DUMP(Js::ByteCodePhase, function->GetFunctionProxy()) && Js::Configuration::Global.flags.Verbose)
            {
                DumpByteCode();
            }
#endif
        }
        catch (const JavascriptException& err)
        {
            Js::JavascriptExceptionObject* ex = err.GetAndClear();
            ex->GetScriptContext();
            JavascriptError::ThrowTypeError(ex->GetScriptContext(), JSERR_BuiltInNotAvailable);
        }
    }

    void JsBuiltInEngineInterfaceExtensionObject::InitializeJsBuiltInNativeInterfaces(DynamicObject * builtInNativeInterfaces, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode)
    {
        typeHandler->Convert(builtInNativeInterfaces, mode, 16);

        ScriptContext* scriptContext = builtInNativeInterfaces->GetScriptContext();
        JavascriptLibrary* library = scriptContext->GetLibrary();

        library->AddFunctionToLibraryObject(builtInNativeInterfaces, Js::PropertyIds::registerFunction, &JsBuiltInEngineInterfaceExtensionObject::EntryInfo::JsBuiltIn_RegisterFunction, 2);

        builtInNativeInterfaces->SetHasNoEnumerableProperties(true);
    }

#if DBG
    void JsBuiltInEngineInterfaceExtensionObject::DumpByteCode()
    {
        Output::Print(_u("Dumping JS Built Ins Byte Code:\r"));
        EnsureJsBuiltInByteCode(scriptContext);
        Js::ByteCodeDumper::DumpRecursively(jsBuiltInByteCode);
    }
#endif // DBG

    void JsBuiltInEngineInterfaceExtensionObject::EnsureJsBuiltInByteCode(ScriptContext * scriptContext)
    {
        if (jsBuiltInByteCode == nullptr)
        {
            SourceContextInfo * sourceContextInfo = scriptContext->GetSourceContextInfo(Js::Constants::NoHostSourceContext, NULL);

            Assert(sourceContextInfo != nullptr);

            SRCINFO si;
            memset(&si, 0, sizeof(si));
            si.sourceContextInfo = sourceContextInfo;
            SRCINFO *hsi = scriptContext->AddHostSrcInfo(&si);
            uint32 flags = fscrIsLibraryCode | (CONFIG_FLAG(CreateFunctionProxy) && !scriptContext->IsProfiling() ? fscrAllowFunctionProxy : 0);

            HRESULT hr = Js::ByteCodeSerializer::DeserializeFromBuffer(scriptContext, flags, (LPCUTF8)nullptr, hsi, (byte*)Library_Bytecode_jsbuiltin, nullptr, &jsBuiltInByteCode);

            IfFailAssertMsgAndThrowHr(hr, "Failed to deserialize JsBuiltIn.js bytecode - very probably the bytecode needs to be rebuilt.");
        }
    }

    DynamicObject* JsBuiltInEngineInterfaceExtensionObject::GetPrototypeFromName(Js::PropertyIds propertyId, ScriptContext* scriptContext)
    {
        JavascriptLibrary* library = scriptContext->GetLibrary();

        switch (propertyId) {
        case PropertyIds::Array:
            return library->arrayPrototype;

        case PropertyIds::String:
            return library->stringPrototype;

        default:
            AssertMsg(false, "Unable to find a prototype that match with this className.");
            return nullptr;
        }
    }

    Var JsBuiltInEngineInterfaceExtensionObject::EntryJsBuiltIn_RegisterFunction(RecyclableObject* function, CallInfo callInfo, ...)
    {
        EngineInterfaceObject_CommonFunctionProlog(function, callInfo);

        Assert(args.Info.Count >= 3 && JavascriptObject::Is(args.Values[1]) && JavascriptFunction::Is(args.Values[2]));

        // retrieves arguments
        RecyclableObject* funcInfo = nullptr;
        if (!JavascriptConversion::ToObject(args.Values[1], scriptContext, &funcInfo))
        {
            JavascriptError::ThrowTypeError(scriptContext, JSERR_FunctionArgument_NeedObject, _u("Object.assign"));
        }

        Var classNameProperty = JavascriptOperators::OP_GetProperty(funcInfo, Js::PropertyIds::className, scriptContext);
        Var methodNameProperty = JavascriptOperators::OP_GetProperty(funcInfo, Js::PropertyIds::methodName, scriptContext);
        Var argumentsCountProperty = JavascriptOperators::OP_GetProperty(funcInfo, Js::PropertyIds::argumentsCount, scriptContext);

        Assert(JavascriptString::Is(classNameProperty));
        Assert(JavascriptString::Is(methodNameProperty));
        Assert(TaggedInt::Is(argumentsCountProperty));

        JavascriptString* className = JavascriptString::FromVar(classNameProperty);
        JavascriptString* methodName = JavascriptString::FromVar(methodNameProperty);
        int argumentsCount = TaggedInt::ToInt32(argumentsCountProperty);
        JavascriptFunction* func = JavascriptFunction::FromVar(args.Values[2]);

        // Set the function's display name, as the function we pass in argument are anonym.
        func->GetFunctionProxy()->SetIsPublicLibraryCode();
        func->GetFunctionProxy()->EnsureDeserialized()->SetDisplayName(methodName->GetString(), methodName->GetLength(), 0);

        DynamicObject* prototype = GetPrototypeFromName(JavascriptOperators::GetPropertyId(className, scriptContext), scriptContext);
        PropertyIds functionIdentifier = JavascriptOperators::GetPropertyId(methodName, scriptContext);

        // Link the function to the prototype.
        ScriptFunction* scriptFunction = scriptContext->GetLibrary()->CreateScriptFunction(func->GetFunctionProxy());
        scriptFunction->SetPropertyWithAttributes(PropertyIds::length, TaggedInt::ToVarUnchecked(argumentsCount), PropertyConfigurable, nullptr);
        scriptFunction->SetPropertyWithAttributes(PropertyIds::arguments, scriptContext->GetLibrary()->GetNull(), PropertyConfigurable, nullptr);
        scriptFunction->SetPropertyWithAttributes(PropertyIds::caller, scriptContext->GetLibrary()->GetNull(), PropertyConfigurable, nullptr);

        scriptFunction->SetConfigurable(PropertyIds::prototype, true);
        scriptFunction->DeleteProperty(PropertyIds::prototype, Js::PropertyOperationFlags::PropertyOperation_None);

        if (scriptFunction->GetScriptContext()->GetConfig()->IsES6FunctionNameEnabled())
        {
            scriptFunction->SetPropertyWithAttributes(PropertyIds::name, methodName, PropertyConfigurable, nullptr);
        }

        scriptContext->GetLibrary()->AddMember(prototype, functionIdentifier, scriptFunction);

        if (functionIdentifier == PropertyIds::indexOf)
        {
            // Special case for Intl who requires to call the non-overriden (by the user) IndexOf function.
            scriptContext->GetLibrary()->AddMember(scriptContext->GetLibrary()->GetEngineInterfaceObject()->GetCommonNativeInterfaces(), Js::PropertyIds::builtInJavascriptArrayEntryIndexOf, scriptFunction);
        }

        //Don't need to return anything
        return scriptContext->GetLibrary()->GetUndefined();
    }

    Var JsBuiltInEngineInterfaceExtensionObject::EntryJsBuiltIn_Internal_ToLengthFunction(RecyclableObject * function, CallInfo callInfo, ...)
    {
        EngineInterfaceObject_CommonFunctionProlog(function, callInfo);
        Assert(args.Info.Count >= 2);
        return JavascriptNumber::ToVar(JavascriptConversion::ToLength(args.Values[1], scriptContext), scriptContext);
    }

    Var JsBuiltInEngineInterfaceExtensionObject::EntryJsBuiltIn_Internal_ToIntegerFunction(RecyclableObject * function, CallInfo callInfo, ...)
    {
        EngineInterfaceObject_CommonFunctionProlog(function, callInfo);
        Assert(args.Info.Count >= 2);

        Var value = args.Values[1];
        if (JavascriptOperators::IsUndefinedOrNull(value))
        {
            return TaggedInt::ToVarUnchecked(0);
        }
        else if (TaggedInt::Is(value))
        {
            return value;
        }

        return JavascriptNumber::ToVarNoCheck(JavascriptConversion::ToInteger(value, scriptContext), scriptContext);
    }

#endif // ENABLE_JS_BUILTINS
}
