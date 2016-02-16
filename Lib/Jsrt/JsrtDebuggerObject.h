//---------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
//----------------------------------------------------------------------------

#pragma once

class JsrtDebug;
class DebuggerObjectsManager;

// Type of objects we give to debugger
typedef enum DebuggerObjectType
{
    DebuggerObjectType_Function,
    DebuggerObjectType_Globals,
    DebuggerObjectType_Property,
    DebuggerObjectType_Scope,
    DebuggerObjectType_Script,
    DebuggerObjectType_StackFrame
} DebuggerObjectType;

// Base class representing a debugger object
class DebuggerObjectBase {
public:
    DebuggerObjectBase(DebuggerObjectType type, DebuggerObjectsManager* debuggerObjectsManager);
    virtual ~DebuggerObjectBase();

    DebuggerObjectType GetType() { return type; }
    uint GetHandle() const { return handle; }
    DebuggerObjectsManager* GetDebuggerObjectsManager();
    virtual Js::DynamicObject* GetJSONObject(Js::ScriptContext* scriptContext) = 0;
    virtual Js::DynamicObject* GetChildrens(Js::ScriptContext* scriptContext);

    template<class DebuggerObjectType, class PostFunction>
    static void CreateDebuggerObject(DebuggerObjectsManager* debuggerObjectsManager, Js::ResolvedObject resolvedObject, Js::ScriptContext* scriptContext, PostFunction postFunction)
    {
        AutoPtr<WeakArenaReference<Js::IDiagObjectModelDisplay>> objectDisplayWeakRef = resolvedObject.GetObjectDisplay();
        Js::IDiagObjectModelDisplay* objectDisplay = objectDisplayWeakRef->GetStrongReference();
        if (objectDisplay != nullptr)
        {
            DebuggerObjectBase* debuggerObject = DebuggerObjectType::Make(debuggerObjectsManager, objectDisplayWeakRef);
            Js::DynamicObject* object = debuggerObject->GetJSONObject(resolvedObject.scriptContext);
            Assert(object != nullptr);
            Js::Var marshaledObj = Js::CrossSite::MarshalVar(scriptContext, object);
            postFunction(marshaledObj);
            objectDisplayWeakRef.Detach();
        }
    }

    template<class PostFunction>
    static JsErrorCode ProcessHandlesArray(JsrtDebug* debugObject, Js::ScriptContext* scriptContext, JsValueRef handlesArray, PostFunction postFunction)
    {
        if (!Js::JavascriptArray::Is(handlesArray))
        {
            return JsErrorInvalidArgument;
        }

        Js::JavascriptArray* handles = Js::JavascriptArray::FromVar(handlesArray);

        uint32 length = handles->GetLength();
        uint32 index = handles->GetNextIndex(Js::JavascriptArray::InvalidIndex);

        for (uint32 i = index; i < length; ++i)
        {
            Js::Var item = nullptr;
            if (handles->GetItem(handles, i, &item, scriptContext))
            {
                uint handle = 0;
                if (Js::JavascriptNumber::Is(item))
                {
                    handle = (uint)Js::JavascriptNumber::GetValue(item);
                }
                else if (Js::TaggedInt::Is(item))
                {
                    handle = Js::TaggedInt::ToUInt32(item);
                }

                if (handle) // valid handle starts with 1 see DebuggerObjectsManager::GetNextHandle
                {
                    DebuggerObjectBase* debuggerObject = nullptr;
                    if (debugObject->GetDebuggerObjectsManager()->TryGetDebuggerObjectFromHandle(handle, &debuggerObject))
                    {
                        wchar_t propertyName[11]; // 4294967295 - Max 10 characters
                        ::_ui64tow_s(debuggerObject->GetHandle(), propertyName, sizeof(propertyName) / sizeof(wchar_t), 10);

                        const Js::PropertyRecord* propertyRecord;
                        scriptContext->GetOrAddPropertyRecord(propertyName, wcslen(propertyName), &propertyRecord);

                        Assert(propertyRecord != nullptr);

                        postFunction(propertyRecord, debuggerObject);
                    }
                }
            }
        }
        return JsNoError;
    }

protected:
    Js::DynamicObject* GetChildrens(WeakArenaReference<Js::IDiagObjectModelDisplay>* objectDisplay, Js::ScriptContext * scriptContext);
private:
    DebuggerObjectType type;
    uint handle;
    DebuggerObjectsManager* debuggerObjectsManager;
};

// Class managing objects we give to debugger, it maintains various mappings
class DebuggerObjectsManager
{
public:
    DebuggerObjectsManager(JsrtDebug* debugObject);
    ~DebuggerObjectsManager();

    void ClearAll();
    JsrtDebug* GetDebugObject() { return this->debugObject; };
    ArenaAllocator* GetDebugObjectArena();
    uint GetNextHandle() { return ++handleId; }

    bool TryGetDebuggerObjectFromHandle(uint handle, DebuggerObjectBase** debuggerObject);
    bool TryGetFrameObjectFromFrameIndex(uint frameIndex, DebuggerObjectBase** debuggerObject);

    void AddToDataToDebuggerObjectsDictionary(void* data, DebuggerObjectBase* debuggerObject);
    bool TryGetDataFromDataToDebuggerObjectsDictionary(void* data, DebuggerObjectBase** debuggerObject);

private:
    uint handleId;
    JsrtDebug* debugObject;

    typedef JsUtil::BaseDictionary<uint, DebuggerObjectBase*, ArenaAllocator> DebuggerObjectsDictionary;
    DebuggerObjectsDictionary* handleToDebuggerObjectsDictionary;

    typedef JsUtil::BaseDictionary<void*, DebuggerObjectBase*, ArenaAllocator> DataToDebuggerObjectsDictionary;
    DataToDebuggerObjectsDictionary* dataToDebuggerObjectsDictionary;

    void AddToDebuggerObjectsDictionary(DebuggerObjectBase* debuggerObject);
};

class DebuggerObjectFunction : public DebuggerObjectBase
{
public:
    static DebuggerObjectBase* Make(DebuggerObjectsManager* debuggerObjectsManager, Js::FunctionBody* functionBody);
    Js::DynamicObject* GetJSONObject(Js::ScriptContext* scriptContext);

private:
    DebuggerObjectFunction(DebuggerObjectsManager* debuggerObjectsManager, Js::FunctionBody* functionBody);
    ~DebuggerObjectFunction();
    Js::FunctionBody* functionBody;
    Js::DynamicObject* functionObject;
};

class DebuggerObjectProperty : public DebuggerObjectBase
{
public:
    static DebuggerObjectBase* Make(DebuggerObjectsManager* debuggerObjectsManager, WeakArenaReference<Js::IDiagObjectModelDisplay>* objectDisplay);

    Js::DynamicObject* GetJSONObject(Js::ScriptContext* scriptContext);
    Js::DynamicObject* GetChildrens(Js::ScriptContext* scriptContext);

private:
    DebuggerObjectProperty(DebuggerObjectsManager* debuggerObjectsManager, WeakArenaReference<Js::IDiagObjectModelDisplay>* objectDisplay);
    ~DebuggerObjectProperty();
    WeakArenaReference<Js::IDiagObjectModelDisplay>* objectDisplay;
    Js::DynamicObject* propertyObject;
};

class DebuggerObjectGlobalsNode : public DebuggerObjectBase
{
public:
    static DebuggerObjectBase* Make(DebuggerObjectsManager* debuggerObjectsManager, WeakArenaReference<Js::IDiagObjectModelDisplay>* objectDisplay);

    Js::DynamicObject* GetJSONObject(Js::ScriptContext* scriptContext);
    Js::DynamicObject* GetChildrens(Js::ScriptContext* scriptContext);

private:
    DebuggerObjectGlobalsNode(DebuggerObjectsManager* debuggerObjectsManager, WeakArenaReference<Js::IDiagObjectModelDisplay>* objectDisplay);
    ~DebuggerObjectGlobalsNode();
    WeakArenaReference<Js::IDiagObjectModelDisplay>* objectDisplay;
    Js::DynamicObject* propertyObject;
};

class DebuggerObjectScope : public DebuggerObjectBase
{
public:
    static DebuggerObjectBase* Make(DebuggerObjectsManager* debuggerObjectsManager, WeakArenaReference<Js::IDiagObjectModelDisplay>* objectDisplay, uint index);

    Js::DynamicObject* GetJSONObject(Js::ScriptContext* scriptContext);
    Js::DynamicObject* GetChildrens(Js::ScriptContext* scriptContext);

private:
    DebuggerObjectScope(DebuggerObjectsManager* debuggerObjectsManager, WeakArenaReference<Js::IDiagObjectModelDisplay>* objectDisplay, uint index);
    ~DebuggerObjectScope();
    WeakArenaReference<Js::IDiagObjectModelDisplay>* objectDisplay;
    Js::DynamicObject* scopeObject;
    uint index;
};

class DebuggerObjectScript : public DebuggerObjectBase
{
public:
    static DebuggerObjectBase* Make(DebuggerObjectsManager* debuggerObjectsManager, Js::Utf8SourceInfo* utf8SourceInfo);

    Js::DynamicObject* GetJSONObject(Js::ScriptContext* scriptContext);

private:
    DebuggerObjectScript(DebuggerObjectsManager* debuggerObjectsManager, Js::Utf8SourceInfo* utf8SourceInfo);
    ~DebuggerObjectScript();

    Js::Utf8SourceInfo* utf8SourceInfo;
    Js::DynamicObject* sourceObject;
};

class DebuggerObjectStackFrame : public DebuggerObjectBase
{
public:
    static DebuggerObjectBase* Make(DebuggerObjectsManager* debuggerObjectsManager, Js::DiagStackFrame* stackFrame, uint frameIndex);

    Js::DynamicObject* GetJSONObject(Js::ScriptContext* scriptContext);
    Js::DynamicObject* GetLocalsObject();
    Js::DynamicObject* Evaluate(const wchar_t*  pszSrc, bool isLibraryCode);
    uint GetIndex() const { return this->frameIndex; }

private:
    DebuggerObjectStackFrame(DebuggerObjectsManager* debuggerObjectsManager, Js::DiagStackFrame* stackFrame, uint frameIndex);
    ~DebuggerObjectStackFrame();

    uint frameIndex;
    Js::DiagStackFrame* stackFrame;
    WeakArenaReference<Js::IDiagObjectModelWalkerBase>* pObjectModelWalker;
    Js::DynamicObject* stackTraceObject;
    Js::DynamicObject* propertiesObject;
};

class JsrtDebugStackFrames
{
public:
    JsrtDebugStackFrames(JsrtDebug* debugObject);
    ~JsrtDebugStackFrames();
    Js::JavascriptArray* StackFrames(Js::ScriptContext* scriptContext);
    bool TryGetFrameObjectFromFrameIndex(uint frameIndex, DebuggerObjectBase ** debuggerObject);
private:
    Js::DynamicObject* GetStackFrame(Js::DiagStackFrame * stackFrame, uint frameIndex);
    Js::JavascriptArray* stackTraceArray;
    JsrtDebug* debugObject;

    typedef JsUtil::BaseDictionary<uint, DebuggerObjectBase*, ArenaAllocator> FramesDictionary;
    FramesDictionary* framesDictionary;

};
