//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "RuntimeLibraryPch.h"

#ifdef ENABLE_WASM
#include "WasmLimits.h"

namespace Js
{

WebAssemblyMemory::WebAssemblyMemory(ArrayBufferBase* buffer, uint32 initial, uint32 maximum, DynamicType * type) :
    DynamicObject(type),
    m_buffer(buffer),
    m_initial(initial),
    m_maximum(maximum)
{
    Assert(m_buffer);
    Assert(m_buffer->GetByteLength() >= UInt32Math::Mul<WebAssembly::PageSize>(initial));
}


__checkReturn bool WebAssemblyMemory::AreLimitsValid(uint32 initial, uint32 maximum)
{
    return initial <= maximum && initial <= Wasm::Limits::GetMaxMemoryInitialPages() && maximum <= Wasm::Limits::GetMaxMemoryMaximumPages();
}


__checkReturn bool WebAssemblyMemory::AreLimitsValid(uint32 initial, uint32 maximum, uint32 bufferLength)
{
    if (!AreLimitsValid(initial, maximum))
    {
        return false;
    }
    // Do the mul after initial checks to avoid potential unneeded OOM exception
    const uint32 initBytes = UInt32Math::Mul<WebAssembly::PageSize>(initial);
    const uint32 maxBytes = UInt32Math::Mul<WebAssembly::PageSize>(maximum);
    return initBytes <= bufferLength && bufferLength <= maxBytes;
}

/* static */
bool
WebAssemblyMemory::Is(Var value)
{
    return JavascriptOperators::GetTypeId(value) == TypeIds_WebAssemblyMemory;
}

/* static */
WebAssemblyMemory *
WebAssemblyMemory::FromVar(Var value)
{
    AssertOrFailFast(WebAssemblyMemory::Is(value));
    return static_cast<WebAssemblyMemory*>(value);
}

Var
WebAssemblyMemory::NewInstance(RecyclableObject* function, CallInfo callInfo, ...)
{
    PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

    ARGUMENTS(args, callInfo);
    ScriptContext* scriptContext = function->GetScriptContext();

    AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");

    Var newTarget = callInfo.Flags & CallFlags_NewTarget ? args.Values[args.Info.Count] : args[0];
    bool isCtorSuperCall = (callInfo.Flags & CallFlags_New) && newTarget != nullptr && !JavascriptOperators::IsUndefined(newTarget);
    Assert(isCtorSuperCall || !(callInfo.Flags & CallFlags_New) || args[0] == nullptr);

    if (!(callInfo.Flags & CallFlags_New) || (newTarget && JavascriptOperators::IsUndefinedObject(newTarget)))
    {
        JavascriptError::ThrowTypeError(scriptContext, JSERR_ClassConstructorCannotBeCalledWithoutNew, _u("WebAssembly.Memory"));
    }

    if (args.Info.Count < 2 || !JavascriptOperators::IsObject(args[1]))
    {
        JavascriptError::ThrowTypeError(scriptContext, JSERR_NeedObject, _u("memoryDescriptor"));
    }
    DynamicObject * memoryDescriptor = JavascriptObject::FromVar(args[1]);

    Var initVar = JavascriptOperators::OP_GetProperty(memoryDescriptor, PropertyIds::initial, scriptContext);
    uint32 initial = WebAssembly::ToNonWrappingUint32(initVar, scriptContext);

    uint32 maximum = Wasm::Limits::GetMaxMemoryMaximumPages();
    bool hasMaximum = false;
    if (JavascriptOperators::OP_HasProperty(memoryDescriptor, PropertyIds::maximum, scriptContext))
    {
        hasMaximum = true;
        Var maxVar = JavascriptOperators::OP_GetProperty(memoryDescriptor, PropertyIds::maximum, scriptContext);
        maximum = WebAssembly::ToNonWrappingUint32(maxVar, scriptContext);
    }

    bool isShared = false;
    if (CONFIG_FLAG(WasmThreads))
    {
        if (JavascriptOperators::OP_HasProperty(memoryDescriptor, PropertyIds::shared, scriptContext))
        {
            if (!hasMaximum)
            {
                JavascriptError::ThrowTypeError(scriptContext, WASMERR_SharedNoMaximum);
            }
            Var sharedVar = JavascriptOperators::OP_GetProperty(memoryDescriptor, PropertyIds::shared, scriptContext);
            isShared = JavascriptConversion::ToBool(sharedVar, scriptContext);
        }
    }

    return CreateMemoryObject(initial, maximum, isShared, scriptContext);
}

Var
WebAssemblyMemory::EntryGrow(RecyclableObject* function, CallInfo callInfo, ...)
{
    ScriptContext* scriptContext = function->GetScriptContext();

    PROBE_STACK(scriptContext, Js::Constants::MinStackDefault);

    ARGUMENTS(args, callInfo);

    AssertMsg(args.Info.Count > 0, "Should always have implicit 'this'");

    Assert(!(callInfo.Flags & CallFlags_New));

    if (!WebAssemblyMemory::Is(args[0]))
    {
        JavascriptError::ThrowTypeError(scriptContext, WASMERR_NeedMemoryObject);
    }

    WebAssemblyMemory* memory = WebAssemblyMemory::FromVar(args[0]);
    Assert(ArrayBufferBase::Is(memory->m_buffer));

    if (memory->m_buffer->IsDetached())
    {
        JavascriptError::ThrowTypeError(scriptContext, JSERR_DetachedTypedArray);
    }

    Var deltaVar = scriptContext->GetLibrary()->GetUndefined();
    if (args.Info.Count >= 2)
    {
        deltaVar = args[1];
    }
    uint32 deltaPages = WebAssembly::ToNonWrappingUint32(deltaVar, scriptContext);

    int32 oldPageCount = memory->GrowInternal(deltaPages);
    if (oldPageCount == -1)
    {
        JavascriptError::ThrowRangeError(scriptContext, JSERR_ArgumentOutOfRange);
    }

    return JavascriptNumber::ToVar(oldPageCount, scriptContext);
}

int32
WebAssemblyMemory::GrowInternal(uint32 deltaPages)
{
    const uint64 deltaBytes = (uint64)deltaPages * WebAssembly::PageSize;
    if (deltaBytes > ArrayBuffer::MaxArrayBufferLength)
    {
        return -1;
    }
    const uint32 oldBytes = m_buffer->GetByteLength();
    const uint64 newBytesLong = deltaBytes + oldBytes;
    if (newBytesLong > ArrayBuffer::MaxArrayBufferLength)
    {
        return -1;
    }
    CompileAssert(ArrayBuffer::MaxArrayBufferLength <= UINT32_MAX);
    const uint32 newBytes = (uint32)newBytesLong;

    const uint32 oldPageCount = oldBytes / WebAssembly::PageSize;
    Assert(oldBytes % WebAssembly::PageSize == 0);

    const uint32 newPageCount = oldPageCount + deltaPages;
    if (newPageCount > m_maximum)
    {
        return -1;
    }

    AssertOrFailFast(m_buffer->IsWebAssemblyArrayBuffer());
    if (m_buffer->IsSharedArrayBuffer())
    {
        AssertOrFailFast(CONFIG_FLAG(WasmThreads));
        if (!WebAssemblySharedArrayBuffer::FromVar(m_buffer)->GrowMemory(newBytes))
        {
            return -1;
        }
    }
    else
    {
        JavascriptExceptionObject* caughtExceptionObject = nullptr;
        try
        {
            WebAssemblyArrayBuffer* newBuffer = WebAssemblyArrayBuffer::FromVar(m_buffer)->GrowMemory(newBytes);
            if (newBuffer == nullptr)
            {
                return -1;
            }
            m_buffer = newBuffer;
        }
        catch (const JavascriptException& err)
        {
            caughtExceptionObject = err.GetAndClear();
            Assert(caughtExceptionObject && caughtExceptionObject == ThreadContext::GetContextForCurrentThread()->GetPendingOOMErrorObject());
            return -1;
        }
    }

    CompileAssert(ArrayBuffer::MaxArrayBufferLength / WebAssembly::PageSize <= INT32_MAX);
    return (int32)oldPageCount;
}

int32
WebAssemblyMemory::GrowHelper(WebAssemblyMemory * mem, uint32 deltaPages)
{
    return mem->GrowInternal(deltaPages);
}

#if DBG
void WebAssemblyMemory::TraceMemWrite(WebAssemblyMemory* mem, uint32 index, uint32 offset, Js::ArrayBufferView::ViewType viewType, uint32 bytecodeOffset, ScriptContext* context)
{
    // Must call after the write
    Assert(mem);
    Output::Print(_u("#%04x "), bytecodeOffset);
    uint64 bigIndex = (uint64)index + (uint64)offset;
    if (index >= mem->m_buffer->GetByteLength())
    {
        Output::Print(_u("WasmMemoryTrace:: Writing out of bounds. %llu >= %u\n"), bigIndex, mem->m_buffer->GetByteLength());
    }
    if (offset)
    {
        Output::Print(_u("WasmMemoryTrace:: buf[%u + %u (%llu)] = "), index, offset, bigIndex);
    }
    else
    {
        Output::Print(_u("WasmMemoryTrace:: buf[%u] = "), index);
    }
    BYTE* buffer = mem->m_buffer->GetBuffer();
    switch (viewType)
    {
    case ArrayBufferView::ViewType::TYPE_INT8_TO_INT64:
    case ArrayBufferView::ViewType::TYPE_INT8: Output::Print(_u("%d\n"), *(int8*)(buffer + bigIndex)); break;
    case ArrayBufferView::ViewType::TYPE_UINT8_TO_INT64:
    case ArrayBufferView::ViewType::TYPE_UINT8: Output::Print(_u("%u\n"), *(uint8*)(buffer + bigIndex)); break;
    case ArrayBufferView::ViewType::TYPE_INT16_TO_INT64:
    case ArrayBufferView::ViewType::TYPE_INT16: Output::Print(_u("%d\n"), *(int16*)(buffer + bigIndex)); break;
    case ArrayBufferView::ViewType::TYPE_UINT16_TO_INT64:
    case ArrayBufferView::ViewType::TYPE_UINT16: Output::Print(_u("%u\n"), *(uint16*)(buffer + bigIndex)); break;
    case ArrayBufferView::ViewType::TYPE_INT32_TO_INT64:
    case ArrayBufferView::ViewType::TYPE_INT32: Output::Print(_u("%d\n"), *(int32*)(buffer + bigIndex)); break;
    case ArrayBufferView::ViewType::TYPE_UINT32_TO_INT64:
    case ArrayBufferView::ViewType::TYPE_UINT32: Output::Print(_u("%u\n"), *(uint32*)(buffer + bigIndex)); break;
    case ArrayBufferView::ViewType::TYPE_FLOAT32: Output::Print(_u("%.4f\n"), *(float*)(buffer + bigIndex)); break;
    case ArrayBufferView::ViewType::TYPE_FLOAT64: Output::Print(_u("%.8f\n"), *(double*)(buffer + bigIndex)); break;
    case ArrayBufferView::ViewType::TYPE_INT64: Output::Print(_u("%lld\n"), *(int64*)(buffer + bigIndex)); break;
    default:
        CompileAssert(ArrayBufferView::ViewType::TYPE_COUNT == 15);
        Assert(UNREACHED);
    }
    return;
}
#endif

Var
WebAssemblyMemory::EntryGetterBuffer(RecyclableObject* function, CallInfo callInfo, ...)
{
    PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);

    ARGUMENTS(args, callInfo);
    ScriptContext* scriptContext = function->GetScriptContext();

    Assert(!(callInfo.Flags & CallFlags_New));

    if (args.Info.Count == 0 || !WebAssemblyMemory::Is(args[0]))
    {
        JavascriptError::ThrowTypeError(scriptContext, WASMERR_NeedMemoryObject);
    }

    WebAssemblyMemory* memory = WebAssemblyMemory::FromVar(args[0]);
    Assert(ArrayBufferBase::Is(memory->m_buffer));
    return memory->m_buffer;
}

WebAssemblyMemory *
WebAssemblyMemory::CreateMemoryObject(uint32 initial, uint32 maximum, bool isShared, ScriptContext * scriptContext)
{
    if (!AreLimitsValid(initial, maximum))
    {
        JavascriptError::ThrowRangeError(scriptContext, JSERR_ArgumentOutOfRange);
    }
    // This shouldn't overflow since we checked in the module, but just to be safe
    uint32 byteLength = UInt32Math::Mul<WebAssembly::PageSize>(initial);
    ArrayBufferBase* buffer = nullptr;
    if (!isShared)
    {
        buffer = scriptContext->GetLibrary()->CreateWebAssemblyArrayBuffer(byteLength);
    }
    else
    {
        Assert(CONFIG_FLAG(WasmThreads));
        uint32 maxByteLength = UInt32Math::Mul<WebAssembly::PageSize>(maximum);
        buffer = scriptContext->GetLibrary()->CreateWebAssemblySharedArrayBuffer(byteLength, maxByteLength);
    }
    Assert(buffer);
    if (byteLength > 0 && buffer->GetByteLength() == 0)
    {
        // Failed to allocate buffer
        return nullptr;
    }
    return RecyclerNewFinalized(scriptContext->GetRecycler(), WebAssemblyMemory, buffer, initial, maximum, scriptContext->GetLibrary()->GetWebAssemblyMemoryType());
}

WebAssemblyMemory * WebAssemblyMemory::CreateForExistingBuffer(uint32 initial, uint32 maximum, uint32 currentByteLength, ScriptContext * scriptContext)
{
    if (!AreLimitsValid(initial, maximum, currentByteLength))
    {
        JavascriptError::ThrowRangeError(scriptContext, JSERR_ArgumentOutOfRange);
    }
    ArrayBufferBase* buffer = scriptContext->GetLibrary()->CreateWebAssemblyArrayBuffer(currentByteLength);
    Assert(buffer);
    if (currentByteLength > 0 && buffer->GetByteLength() == 0)
    {
        // Failed to allocate buffer
        return nullptr;
    }
    return RecyclerNewFinalized(scriptContext->GetRecycler(), WebAssemblyMemory, buffer, initial, maximum, scriptContext->GetLibrary()->GetWebAssemblyMemoryType());
}

WebAssemblyMemory * WebAssemblyMemory::CreateFromSharedContents(uint32 initial, uint32 maximum, SharedContents* sharedContents, ScriptContext * scriptContext)
{
    if (!sharedContents || !AreLimitsValid(initial, maximum, sharedContents->bufferLength))
    {
        JavascriptError::ThrowRangeError(scriptContext, JSERR_ArgumentOutOfRange);
    }
    ArrayBufferBase* buffer = scriptContext->GetLibrary()->CreateWebAssemblySharedArrayBuffer(sharedContents);
    return RecyclerNewFinalized(scriptContext->GetRecycler(), WebAssemblyMemory, buffer, initial, maximum, scriptContext->GetLibrary()->GetWebAssemblyMemoryType());
}

ArrayBufferBase*
WebAssemblyMemory::GetBuffer() const
{
    return m_buffer;
}

uint
WebAssemblyMemory::GetInitialLength() const
{
    return m_initial;
}

uint
WebAssemblyMemory::GetMaximumLength() const
{
    return m_maximum;
}

uint
WebAssemblyMemory::GetCurrentMemoryPages() const
{
    return m_buffer->GetByteLength() / WebAssembly::PageSize;
}

bool WebAssemblyMemory::IsSharedMemory() const
{
    return WebAssemblySharedArrayBuffer::Is(m_buffer);
}

} // namespace Js
#endif // ENABLE_WASM
