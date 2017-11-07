//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#ifdef ENABLE_JS_BUILTINS
namespace Js
{
    class JsBuiltInEngineInterfaceExtensionObject : public EngineExtensionObjectBase
    {
    public:
        JsBuiltInEngineInterfaceExtensionObject(ScriptContext* scriptContext);
        void Initialize();
        void InitializePrototypes(ScriptContext * scriptContext);
        void InjectJsBuiltInLibraryCode(ScriptContext * scriptContext);

        static bool __cdecl InitializeJsBuiltInNativeInterfaces(DynamicObject* intlNativeInterfaces, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);

#if DBG
        void DumpByteCode() override;
#endif

        class EntryInfo
        {
        public:
            static NoProfileFunctionInfo JsBuiltIn_RegisterChakraLibraryFunction;
            static NoProfileFunctionInfo JsBuiltIn_RegisterFunction;
            static NoProfileFunctionInfo JsBuiltIn_Internal_GetLength;
            static NoProfileFunctionInfo JsBuiltIn_Internal_SetPrototype;
            static NoProfileFunctionInfo JsBuiltIn_Internal_GetIteratorPrototype;
            static NoProfileFunctionInfo JsBuiltIn_Internal_InitInternalProperties;
        };

    private:
        Field(DynamicObject*) builtInNativeInterfaces;
        Field(FunctionBody*) jsBuiltInByteCode;

        Field(bool) wasInitialized;

        void EnsureJsBuiltInByteCode(ScriptContext * scriptContext);

        static DynamicObject* GetPrototypeFromName(Js::PropertyIds propertyId, ScriptContext* scriptContext);
        static Var EntryJsBuiltIn_RegisterChakraLibraryFunction(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryJsBuiltIn_RegisterFunction(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryJsBuiltIn_Internal_GetLength(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryJsBuiltIn_Internal_SetPrototype(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryJsBuiltIn_Internal_GetIteratorPrototype(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryJsBuiltIn_Internal_InitInternalProperties(RecyclableObject* function, CallInfo callInfo, ...);
    };
}
#endif // ENABLE_JS_BUILTINS
