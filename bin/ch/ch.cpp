//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include "Core/AtomLockGuids.h"

unsigned int MessageBase::s_messageCount = 0;
Debugger* Debugger::debugger = nullptr;

#ifdef _WIN32
LPCWSTR hostName = _u("ch.exe");
#else
LPCWSTR hostName = _u("ch");
#endif

JsRuntimeHandle chRuntime = JS_INVALID_RUNTIME_HANDLE;

BOOL doTTRecord = false;
BOOL doTTDebug = false;
char16* ttUri = nullptr;
UINT32 snapInterval = MAXUINT32;
UINT32 snapHistoryLength = MAXUINT32;

const char16* dbgIPAddr = nullptr;
unsigned short dbgPort = 0;

extern "C"
HRESULT __stdcall OnChakraCoreLoadedEntry(TestHooks& testHooks)
{
    return ChakraRTInterface::OnChakraCoreLoaded(testHooks);
}

JsRuntimeAttributes jsrtAttributes = JsRuntimeAttributeAllowScriptInterrupt;

int HostExceptionFilter(int exceptionCode, _EXCEPTION_POINTERS *ep)
{
    ChakraRTInterface::NotifyUnhandledException(ep);

    bool crashOnException = false;
    ChakraRTInterface::GetCrashOnExceptionFlag(&crashOnException);

    if (exceptionCode == EXCEPTION_BREAKPOINT || (crashOnException && exceptionCode != 0xE06D7363))
    {
        return EXCEPTION_CONTINUE_SEARCH;
    }

    fwprintf(stderr, _u("FATAL ERROR: %ls failed due to exception code %x\n"), hostName, exceptionCode);

    _flushall();

    // Exception happened, so we probably didn't clean up properly,
    // Don't exit normally, just terminate
    TerminateProcess(::GetCurrentProcess(), exceptionCode);

    return EXCEPTION_CONTINUE_SEARCH;
}

void __stdcall PrintUsageFormat()
{
    wprintf(_u("\nUsage: %s [flaglist] <source file>\n"), hostName);
}

void __stdcall PrintUsage()
{
#ifndef DEBUG
    wprintf(_u("\nUsage: %s <source file> %s"), hostName,
            _u("\n[flaglist] is not supported for Release mode\n"));
#else
    PrintUsageFormat();
    wprintf(_u("Try '%s -?' for help\n"), hostName);
#endif
}

// On success the param byteCodeBuffer will be allocated in the function.
// The caller of this function should de-allocate the memory.
HRESULT GetSerializedBuffer(LPCOLESTR fileContents, __out BYTE **byteCodeBuffer, __out DWORD *byteCodeBufferSize)
{
    HRESULT hr = S_OK;
    *byteCodeBuffer = nullptr;
    *byteCodeBufferSize = 0;
    BYTE *bcBuffer = nullptr;

    unsigned int bcBufferSize = 0;
    unsigned int newBcBufferSize = 0;
    IfJsErrorFailLog(ChakraRTInterface::JsSerializeScript(fileContents, bcBuffer, &bcBufferSize));
    // Above call will return the size of the buffer only, once succeed we need to allocate memory of that much and call it again.
    if (bcBufferSize == 0)
    {
        AssertMsg(false, "bufferSize should not be zero");
        IfFailGo(E_FAIL);
    }
    bcBuffer = new BYTE[bcBufferSize];
    newBcBufferSize = bcBufferSize;
    IfJsErrorFailLog(ChakraRTInterface::JsSerializeScript(fileContents, bcBuffer, &newBcBufferSize));
    Assert(bcBufferSize == newBcBufferSize);

Error:
    if (hr != S_OK)
    {
        // In the failure release the buffer
        if (bcBuffer != nullptr)
        {
            delete[] bcBuffer;
        }
    }
    else
    {
        *byteCodeBuffer = bcBuffer;
        *byteCodeBufferSize = bcBufferSize;
    }

    return hr;
}

HRESULT CreateLibraryByteCodeHeader(LPCOLESTR fileContents, BYTE * contentsRaw, DWORD lengthBytes, LPCWSTR bcFullPath, LPCSTR libraryNameNarrow)
{
    HANDLE bcFileHandle = nullptr;
    BYTE *bcBuffer = nullptr;
    DWORD bcBufferSize = 0;
    HRESULT hr = GetSerializedBuffer(fileContents, &bcBuffer, &bcBufferSize);

    if (FAILED(hr)) return hr;

    bcFileHandle = CreateFile(bcFullPath, GENERIC_WRITE, FILE_SHARE_DELETE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (bcFileHandle == INVALID_HANDLE_VALUE)
    {
        return E_FAIL;
    }

    DWORD written;

    // For validating the header file against the library file
    auto outputStr =
        "//-------------------------------------------------------------------------------------------------------\r\n"
        "// Copyright (C) Microsoft. All rights reserved.\r\n"
        "// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.\r\n"
        "//-------------------------------------------------------------------------------------------------------\r\n"
        "#if 0\r\n";
    IfFalseGo(WriteFile(bcFileHandle, outputStr, (DWORD)strlen(outputStr), &written, nullptr));
    IfFalseGo(WriteFile(bcFileHandle, contentsRaw, lengthBytes, &written, nullptr));
    if (lengthBytes < 2 || contentsRaw[lengthBytes - 2] != '\r' || contentsRaw[lengthBytes - 1] != '\n')
    {
        outputStr = "\r\n#endif\r\n";
    }
    else
    {
        outputStr = "#endif\r\n";
    }
    IfFalseGo(WriteFile(bcFileHandle, outputStr, (DWORD)strlen(outputStr), &written, nullptr));

    // Write out the bytecode
    outputStr = "namespace Js\r\n{\r\n    const char Library_Bytecode_";
    IfFalseGo(WriteFile(bcFileHandle, outputStr, (DWORD)strlen(outputStr), &written, nullptr));
    IfFalseGo(WriteFile(bcFileHandle, libraryNameNarrow, (DWORD)strlen(libraryNameNarrow), &written, nullptr));
    outputStr = "[] = {\r\n/* 00000000 */";
    IfFalseGo(WriteFile(bcFileHandle, outputStr, (DWORD)strlen(outputStr), &written, nullptr));

    for (unsigned int i = 0; i < bcBufferSize; i++)
    {
        char scratch[6];
        auto scratchLen = sizeof(scratch);
        int num = _snprintf_s(scratch, scratchLen, _countof(scratch), " 0x%02X", bcBuffer[i]);
        Assert(num == 5);
        IfFalseGo(WriteFile(bcFileHandle, scratch, (DWORD)(scratchLen - 1), &written, nullptr));

        // Add a comma and a space if this is not the last item
        if (i < bcBufferSize - 1)
        {
            char commaSpace[2];
            _snprintf_s(commaSpace, sizeof(commaSpace), _countof(commaSpace), ",");  // close quote, new line, offset and open quote
            IfFalseGo(WriteFile(bcFileHandle, commaSpace, (DWORD)strlen(commaSpace), &written, nullptr));
        }

        // Add a line break every 16 scratches, primarily so the compiler doesn't complain about the string being too long.
        // Also, won't add for the last scratch
        if (i % 16 == 15 && i < bcBufferSize - 1)
        {
            char offset[17];
            _snprintf_s(offset, sizeof(offset), _countof(offset), "\r\n/* %08X */", i + 1);  // close quote, new line, offset and open quote
            IfFalseGo(WriteFile(bcFileHandle, offset, (DWORD)strlen(offset), &written, nullptr));
        }
    }
    outputStr = "};\r\n\r\n";
    IfFalseGo(WriteFile(bcFileHandle, outputStr, (DWORD)strlen(outputStr), &written, nullptr));

    outputStr = "}\r\n";
    IfFalseGo(WriteFile(bcFileHandle, outputStr, (DWORD)strlen(outputStr), &written, nullptr));

Error:
    if (bcFileHandle != nullptr)
    {
        CloseHandle(bcFileHandle);
    }
    if (bcBuffer != nullptr)
    {
        delete[] bcBuffer;
    }

    return hr;
}

static void CALLBACK PromiseContinuationCallback(JsValueRef task, void *callbackState)
{
    Assert(task != JS_INVALID_REFERENCE);
    Assert(callbackState != JS_INVALID_REFERENCE);
    MessageQueue * messageQueue = (MessageQueue *)callbackState;

    WScriptJsrt::CallbackMessage *msg = new WScriptJsrt::CallbackMessage(0, task);

#if ENABLE_TTD
    ChakraRTInterface::JsTTDNotifyHostCallbackCreatedOrCanceled(true, false, false, task, msg->GetId());
#endif

    messageQueue->InsertSorted(msg);
}

void StartupDebuggerAsNeeded()
{
    if(dbgIPAddr == nullptr)
    {
        if(doTTDebug)
        {
            //we need to force the script context into dbg mode for replay even if we don't attach the debugger -- so do that here
            ChakraRTInterface::JsTTDSetDebuggerForReplay();
        }
    }
    else
    {
        //
        //TODO: not sure how to set IP and other stuff here !!!
        //
        AssertMsg(false, "Broken");

        Debugger* debugger = Debugger::GetDebugger(chRuntime);
        debugger->StartDebugging(chRuntime);
    }
}

void CreateDirectoryIfNeeded(const char16* path)
{
#ifndef _WIN32
    AssertMsg(false, "Not XPLAT yet.");
#else
    bool isPathDirName = (path[wcslen(path) - 1] == _u('\\'));

    size_t fplength = (wcslen(path) + 2);
    wchar_t* fullpath = (wchar_t*)malloc(fplength * sizeof(char16));
    fullpath[0] = _u('\0');

    wcscat_s(fullpath, fplength, path);
    if(!isPathDirName)
    {
        wcscat_s(fullpath, fplength, _u("\\"));
    }

    DWORD dwAttrib = GetFileAttributes(fullpath);
    if((dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
    {
        free(fullpath);
        return;
    }

    BOOL success = CreateDirectory(fullpath, NULL);
    if(!success)
    {
        DWORD lastError = GetLastError();
        LPTSTR pTemp = NULL;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY, NULL, lastError, 0, (LPTSTR)&pTemp, 0, NULL);
        fwprintf(stderr, _u(": %s"), pTemp);

        AssertMsg(false, "Failed Directory Create");
    }

    free(fullpath);
#endif
}

void DeleteDirectory(const char16* path)
{
#ifndef _WIN32
    AssertMsg(false, "Not XPLAT yet.");
#else
    HANDLE hFile;
    WIN32_FIND_DATA FileInformation;

    bool isPathDirName = (path[wcslen(path) - 1] == _u('\\'));

    size_t splength = (wcslen(path) + 5);
    wchar_t* strPattern = (wchar_t*)malloc(splength * sizeof(char16));
    strPattern[0] = _u('\0');

    wcscat_s(strPattern, splength, path);
    if(!isPathDirName)
    {
        wcscat_s(strPattern, splength, _u("\\"));
    }
    wcscat_s(strPattern, splength, _u("*.*"));

    hFile = ::FindFirstFile(strPattern, &FileInformation);
    if(hFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if(FileInformation.cFileName[0] != '.')
            {
                size_t sfplength = (wcslen(path) + wcslen(FileInformation.cFileName) + 2);
                wchar_t* strFilePath = (wchar_t*)malloc(sfplength * sizeof(char16));
                strFilePath[0] = _u('\0');

                wcscat_s(strFilePath, sfplength, path);
                if(!isPathDirName)
                {
                    wcscat_s(strFilePath, sfplength, _u("\\"));
                }
                wcscat_s(strFilePath, sfplength, FileInformation.cFileName);

                if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    DeleteDirectory(strFilePath);
                    ::RemoveDirectory(strFilePath);
                }
                else
                {
                    // Set file attributes
                    ::SetFileAttributes(strFilePath, FILE_ATTRIBUTE_NORMAL);
                    ::DeleteFile(strFilePath);
                }

                free(strFilePath);
            }
        } while(::FindNextFile(hFile, &FileInformation) == TRUE);

        // Close handle
        ::FindClose(hFile);
    }

    free(strPattern);
#endif
}

void GetFileFromURI(const char16* uri, char16** res)
{
    int urilen = (int)wcslen(uri);
    int fpos = 0;
    for(int spos = urilen - 1; spos >= 0; --spos)
    {
        if(uri[spos] == _u('\\') || uri[spos] == _u('/'))
        {
            fpos = spos + 1;
            break;
        }
    }

    size_t rlength = (wcslen(uri + fpos) + 1);
    *res = (wchar_t*)malloc(rlength * sizeof(char16));
    (*res)[0] = _u('\0');

    wcscat_s(*res, rlength, uri + fpos);
}

void GetDefaultTTDDirectory(char16** res, const char16* optExtraDir)
{
#ifndef _WIN32
    *res = nullptr;
    AssertMsg(false, "Not XPLAT yet.");
#else
    char16* path = (char16*)malloc(MAX_PATH * sizeof(char16));
    path[0] = _u('\0');

    GetModuleFileName(NULL, path, MAX_PATH);

    char16* spos = wcsstr(path, _u("\\Build\\VcBuild\\"));
    AssertMsg(spos != nullptr, "Something got renamed or moved!!!");

    int ccount = (int)((((byte*)spos) - ((byte*)path)) / sizeof(char16));

    *res = (char16*)CoTaskMemAlloc(MAX_PATH * sizeof(char16));
    (*res)[0] = _u('\0');

    for(int i = 0; i < ccount; ++i)
    {
        (*res)[i] = path[i];
    }
    (*res)[ccount] = _u('\0');

    wcscat_s(*res, MAX_PATH, _u("\\test\\_ttdlog\\"));

    if(wcslen(optExtraDir) == 0)
    {
        wcscat_s(*res, MAX_PATH, _u("_defaultLog"));
    }
    else
    {
        wcscat_s(*res, MAX_PATH, optExtraDir);
    }

    bool isPathDirName = ((*res)[wcslen(*res) - 1] == _u('\\'));
    if(!isPathDirName)
    {
        wcscat_s(*res, MAX_PATH, _u("\\"));
    }

    free(path);
#endif
}

static void CALLBACK GetTTDDirectory(const char16* uri, char16** fullTTDUri)
{
#ifndef _WIN32
    *fullTTDUri = nullptr;
    AssertMsg(false, "Not XPLAT yet.");
#else
    if(uri[0] != _u('!'))
    {
        bool isPathDirName = (uri[wcslen(uri) - 1] == _u('\\'));

        size_t rlength = (wcslen(uri) + 2);
        *fullTTDUri = (wchar_t*)CoTaskMemAlloc(rlength * sizeof(char16));
        (*fullTTDUri)[0] = _u('\0');

        wcscat_s(*fullTTDUri, rlength, uri);
        if(!isPathDirName)
        {
            wcscat_s(*fullTTDUri, rlength, _u("\\"));
        }
    }
    else
    {
        GetDefaultTTDDirectory(fullTTDUri, uri + 1);
    }
#endif
}

static void CALLBACK TTInitializeForWriteLogStreamCallback(const char16* uri)
{
    //If the directory does not exist then we want to write it
    CreateDirectoryIfNeeded(uri);

    //Clear the logging directory so it is ready for us to write into
    DeleteDirectory(uri);
}

static HANDLE TTOpenStream_Helper(const char16* uri, bool read, bool write)
{
#ifndef _WIN32
    AssertMsg(false, "Not XPLAT yet.");
    return 0;
#else
    HANDLE res = INVALID_HANDLE_VALUE;

    if(read)
    {
        res = CreateFile(uri, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    }
    else
    {
        res = CreateFile(uri, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    }

    if(res == INVALID_HANDLE_VALUE)
    {
        DWORD lastError = GetLastError();
        LPTSTR pTemp = NULL;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY, NULL, lastError, 0, (LPTSTR)&pTemp, 0, NULL);
        fwprintf(stderr, _u(": %s\n"), pTemp);
        fwprintf(stderr, _u("Failed on file: %ls\n"), uri);

        AssertMsg(false, "Failed File Open");
    }

    return res;
#endif
}

static HANDLE CALLBACK TTGetLogStreamCallback(const char16* uri, bool read, bool write)
{
#ifndef _WIN32
    AssertMsg(false, "Not XPLAT yet.");
    return 0;
#else
    AssertMsg((read | write) & !(read & write), "Should be either read or write and at least one.");

    size_t rlength = (wcslen(uri) + 16);
    char16* logfile = (char16*)malloc(rlength * sizeof(char16));
    logfile[0] = _u('\0');

    wcscat_s(logfile, rlength, uri);
    wcscat_s(logfile, rlength, _u("ttdlog.log"));

    HANDLE res = TTOpenStream_Helper(logfile, read, write);

    free(logfile);
    return res;
#endif
}

static HANDLE CALLBACK TTGetSnapshotStreamCallback(const char16* uri, const char16* snapId, bool read, bool write)
{
#ifndef _WIN32
    AssertMsg(false, "Not XPLAT yet.");
    return 0;
#else
    AssertMsg((read | write) & !(read & write), "Should be either read or write and at least one.");

    size_t rlength = (wcslen(uri) + 64 + 16);
    char16* snapfile = (char16*)malloc(rlength * sizeof(char16));
    snapfile[0] = _u('\0');

    wcscat_s(snapfile, rlength, uri);
    wcscat_s(snapfile, rlength, _u("\\snap_"));
    wcscat_s(snapfile, rlength, snapId);
    wcscat_s(snapfile, rlength, _u(".snp"));

    HANDLE res = TTOpenStream_Helper(snapfile, read, write);

    free(snapfile);
    return res;
#endif
}

static HANDLE CALLBACK TTGetSrcCodeStreamCallback(const char16* uri, const char16* bodyCtrId, const char16* srcFileName, bool read, bool write)
{
#ifndef _WIN32
    AssertMsg(false, "Not XPLAT yet.");
    return 0;
#else
    AssertMsg((read | write) & !(read & write), "Should be either read or write and at least one.");

    char16* sFile = nullptr;
    GetFileFromURI(srcFileName, &sFile);

    size_t rlength = (wcslen(uri) + 64 + wcslen(sFile) + 4);
    char16* srcPath = (char16*)malloc(rlength * sizeof(char16));
    srcPath[0] = _u('\0');

    wcscat_s(srcPath, rlength, uri);
    wcscat_s(srcPath, rlength, bodyCtrId);
    wcscat_s(srcPath, rlength, _u("_"));
    wcscat_s(srcPath, rlength, sFile);

    HANDLE res = TTOpenStream_Helper(srcPath, read, write);

    free(sFile);
    free(srcPath);
    return res;
#endif
}

static BOOL CALLBACK TTReadBytesFromStreamCallback(HANDLE strm, BYTE* buff, DWORD size, DWORD* readCount)
{
#ifndef _WIN32
    AssertMsg(false, "Not XPLAT yet.");
    return FALSE;
#else
    AssertMsg(strm != INVALID_HANDLE_VALUE, "Bad file handle.");

    *readCount = 0;
    BOOL ok = ReadFile(strm, buff, size, readCount, NULL);
    AssertMsg(ok, "Read failed.");

    if(!ok)
    {
        DWORD lastError = GetLastError();
        LPTSTR pTemp = NULL;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY, NULL, lastError, 0, (LPTSTR)&pTemp, 0, NULL);
        fwprintf(stderr, _u(": %s\n"), pTemp);
    }

    return ok;
#endif
}

static BOOL CALLBACK TTWriteBytesToStreamCallback(HANDLE strm, BYTE* buff, DWORD size, DWORD* writtenCount)
{
#ifndef _WIN32
    AssertMsg(false, "Not XPLAT yet.");
    return FALSE;
#else
    AssertMsg(strm != INVALID_HANDLE_VALUE, "Bad file handle.");

    BOOL ok = WriteFile(strm, buff, size, writtenCount, NULL);
    AssertMsg(*writtenCount == size, "Write Failed");

    if(!ok)
    {
        DWORD lastError = GetLastError();
        LPTSTR pTemp = NULL;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY, NULL, lastError, 0, (LPTSTR)&pTemp, 0, NULL);
        fwprintf(stderr, _u(": %s\n"), pTemp);
    }

    return ok;
#endif
}

static void CALLBACK TTFlushAndCloseStreamCallback(HANDLE strm, bool read, bool write)
{
#ifndef _WIN32
    AssertMsg(false, "Not XPLAT yet.");
#else
    AssertMsg((read | write) & !(read & write), "Should be either read or write and at least one.");

    if(strm != INVALID_HANDLE_VALUE)
    {
        if(write)
        {
            FlushFileBuffers(strm);
        }

        CloseHandle(strm);
    }
#endif
}

HRESULT RunScript(const char* fileName, LPCWSTR fileContents, BYTE *bcBuffer, char *fullPath)
{
    HRESULT hr = S_OK;
    MessageQueue * messageQueue = new MessageQueue();
    WScriptJsrt::AddMessageQueue(messageQueue);
    LPWSTR fullPathWide = nullptr;

    IfJsErrorFailLog(ChakraRTInterface::JsSetPromiseContinuationCallback(PromiseContinuationCallback, (void*)messageQueue));

    if(strlen(fileName) >= 14 && strcmp(fileName + strlen(fileName) - 14, "ttdSentinal.js") == 0)
    {
#if !ENABLE_TTD
        wprintf(_u("Sential js file is only ok when in TTDebug mode!!!\n"));
        return E_FAIL;
#else
        if(!doTTDebug)
        {
            wprintf(_u("Sential js file is only ok when in TTDebug mode!!!\n"));
            return E_FAIL;
        }

        ChakraRTInterface::JsTTDStartTimeTravelDebugging();

        try
        {
            INT64 snapEventTime = -1;
            INT64 nextEventTime = -2;

            while(true)
            {
                IfJsErrorFailLog(ChakraRTInterface::JsTTDPrepContextsForTopLevelEventMove(chRuntime, nextEventTime, &snapEventTime));

                ChakraRTInterface::JsTTDMoveToTopLevelEvent(snapEventTime, nextEventTime);

                JsErrorCode res = ChakraRTInterface::JsTTDReplayExecution(&nextEventTime);

                //handle any uncaught exception by immediately time-traveling to the throwing line
                if(res == JsErrorCategoryScript)
                {
                    wprintf(_u("An unhandled script exception occoured!!!\n"));

                    ExitProcess(0);
                }

                if(nextEventTime == -1)
                {
                    wprintf(_u("\nReached end of Execution -- Exiting.\n"));
                    break;
                }
            }
        }
        catch(...)
        {
            wprintf(_u("Exception."));
        }
#endif
    }
    else
    {
        Assert(fileContents != nullptr || bcBuffer != nullptr);
        // TODO: Remove this code in a future iteration once Utf8 versions of the Jsrt API is implemented
        IfFailGo(Helpers::NarrowStringToWideDynamic(fullPath, &fullPathWide));

        JsErrorCode runScript;
        if(bcBuffer != nullptr)
        {
            runScript = ChakraRTInterface::JsRunSerializedScript(fileContents, bcBuffer, WScriptJsrt::GetNextSourceContext(), fullPathWide, nullptr /*result*/);
        }
        else
        {
#if ENABLE_TTD
            if(doTTRecord)
            {
                ChakraRTInterface::JsTTDStartTimeTravelRecording();
            }

            runScript = ChakraRTInterface::JsTTDRunScript(-1, fileContents, WScriptJsrt::GetNextSourceContext(), fullPathWide, nullptr /*result*/);
#else
            runScript = ChakraRTInterface::JsRunScript(fileContents, WScriptJsrt::GetNextSourceContext(), fullPathWide, nullptr /*result*/);
#endif
        }

        free(fullPathWide);

        //Do a yield after the main script body executes
        ChakraRTInterface::JsTTDNotifyYield();

        if(runScript != JsNoError)
        {
            WScriptJsrt::PrintException(fileName, runScript);
        }
        else
        {
            // Repeatedly flush the message queue until it's empty. It is necessary to loop on this
            // because setTimeout can add scripts to execute.
            do
            {
                IfFailGo(messageQueue->ProcessAll(fileName));
            } while(!messageQueue->IsEmpty());
        }
    }

Error:
#if ENABLE_TTD
    if(doTTRecord)
    {
        ChakraRTInterface::JsTTDStopTimeTravelRecording();
    }
#endif

    if (messageQueue != nullptr)
    {
        messageQueue->RemoveAll();
        delete messageQueue;
    }
    return hr;
}

HRESULT CreateAndRunSerializedScript(const char* fileName, LPCWSTR fileContents, char* fullPath)
{
    HRESULT hr = S_OK;
    JsRuntimeHandle runtime = JS_INVALID_RUNTIME_HANDLE;
    JsContextRef context = JS_INVALID_REFERENCE, current = JS_INVALID_REFERENCE;
    BYTE *bcBuffer = nullptr;
    DWORD bcBufferSize = 0;
    IfFailGo(GetSerializedBuffer(fileContents, &bcBuffer, &bcBufferSize));

    // Bytecode buffer is created in one runtime and will be executed on different runtime.

    IfJsErrorFailLog(ChakraRTInterface::JsCreateRuntime(jsrtAttributes, nullptr, &runtime));
    chRuntime = runtime;

    IfJsErrorFailLog(ChakraRTInterface::JsCreateContext(runtime, &context));
    IfJsErrorFailLog(ChakraRTInterface::JsGetCurrentContext(&current));
    IfJsErrorFailLog(ChakraRTInterface::JsSetCurrentContext(context));

    // Initialized the WScript object on the new context
    if (!WScriptJsrt::Initialize())
    {
        IfFailGo(E_FAIL);
    }

    IfFailGo(RunScript(fileName, fileContents, bcBuffer, fullPath));

Error:
    if (bcBuffer != nullptr)
    {
        delete[] bcBuffer;
    }

    if (current != JS_INVALID_REFERENCE)
    {
        ChakraRTInterface::JsSetCurrentContext(current);
    }

    if (runtime != JS_INVALID_RUNTIME_HANDLE)
    {
        ChakraRTInterface::JsDisposeRuntime(runtime);
    }
    return hr;
}

HRESULT ExecuteTest(const char* fileName)
{
    HRESULT hr = S_OK;
    LPCWSTR fileContents = nullptr;
    JsRuntimeHandle runtime = JS_INVALID_RUNTIME_HANDLE;

    if(strlen(fileName) >= 14 && strcmp(fileName + strlen(fileName) - 14, "ttdSentinal.js") == 0)
    {
#if !ENABLE_TTD
        wprintf(_u("Sential js file is only ok when in TTDebug mode!!!\n"));
        return E_FAIL;
#else
        if(!doTTDebug)
        {
            wprintf(_u("Sential js file is only ok when in TTDebug mode!!!\n"));
            return E_FAIL;
        }

        IfJsErrorFailLog(ChakraRTInterface::JsTTDCreateDebugRuntime(jsrtAttributes, ttUri, nullptr, &runtime));
        chRuntime = runtime;

        ChakraRTInterface::JsTTDSetIOCallbacks(runtime, &GetTTDDirectory, &TTInitializeForWriteLogStreamCallback, &TTGetLogStreamCallback, &TTGetSnapshotStreamCallback, &TTGetSrcCodeStreamCallback, &TTReadBytesFromStreamCallback, &TTWriteBytesToStreamCallback, &TTFlushAndCloseStreamCallback);

        JsContextRef context = JS_INVALID_REFERENCE;
        IfJsErrorFailLog(ChakraRTInterface::JsTTDCreateContext(runtime, &context));
        IfJsErrorFailLog(ChakraRTInterface::JsSetCurrentContext(context));

        StartupDebuggerAsNeeded();

        IfFailGo(RunScript(fileName, fileContents, nullptr, nullptr));
#endif
    }
    else
    {
        bool isUtf8 = false;
        LPCOLESTR contentsRaw = nullptr;
        UINT lengthBytes = 0;

        char fullPath[_MAX_PATH];
        size_t len = 0;

        hr = Helpers::LoadScriptFromFile(fileName, fileContents, &isUtf8, &contentsRaw, &lengthBytes);
        contentsRaw; lengthBytes; // Unused for now.

        IfFailGo(hr);
        if(HostConfigFlags::flags.GenerateLibraryByteCodeHeaderIsEnabled)
        {
            jsrtAttributes = (JsRuntimeAttributes)(jsrtAttributes | JsRuntimeAttributeSerializeLibraryByteCode);
        }

#if ENABLE_TTD
        if(doTTRecord)
        {
            IfJsErrorFailLog(ChakraRTInterface::JsTTDCreateRecordRuntime(jsrtAttributes, ttUri, snapInterval, snapHistoryLength, nullptr, &runtime));
            chRuntime = runtime;

            ChakraRTInterface::JsTTDSetIOCallbacks(runtime, &GetTTDDirectory, &TTInitializeForWriteLogStreamCallback, &TTGetLogStreamCallback, &TTGetSnapshotStreamCallback, &TTGetSrcCodeStreamCallback, &TTReadBytesFromStreamCallback, &TTWriteBytesToStreamCallback, &TTFlushAndCloseStreamCallback);

            JsContextRef context = JS_INVALID_REFERENCE;
            IfJsErrorFailLog(ChakraRTInterface::JsTTDCreateContext(runtime, &context));
            IfJsErrorFailLog(ChakraRTInterface::JsSetCurrentContext(context));

            StartupDebuggerAsNeeded();
        }
        else
        {
            AssertMsg(!doTTDebug, "Should be handled in the else case above!!!");

            IfJsErrorFailLog(ChakraRTInterface::JsCreateRuntime(jsrtAttributes, nullptr, &runtime));
            chRuntime = runtime;

            if(HostConfigFlags::flags.DebugLaunch)
            {
                Debugger* debugger = Debugger::GetDebugger(runtime);
                debugger->StartDebugging(runtime);
            }

            JsContextRef context = JS_INVALID_REFERENCE;
            IfJsErrorFailLog(ChakraRTInterface::JsCreateContext(runtime, &context));
            IfJsErrorFailLog(ChakraRTInterface::JsSetCurrentContext(context));
        }
#else
        IfJsErrorFailLog(ChakraRTInterface::JsCreateRuntime(jsrtAttributes, nullptr, &runtime));
        chRuntime = runtime;

        if(HostConfigFlags::flags.DebugLaunch)
        {
            Debugger* debugger = Debugger::GetDebugger(runtime);
            debugger->StartDebugging(runtime);
        }

        JsContextRef context = JS_INVALID_REFERENCE;
        IfJsErrorFailLog(ChakraRTInterface::JsCreateContext(runtime, &context));
        IfJsErrorFailLog(ChakraRTInterface::JsSetCurrentContext(context));
#endif

#ifdef DEBUG
        ChakraRTInterface::SetCheckOpHelpersFlag(true);
#endif

        if(!WScriptJsrt::Initialize())
        {
            IfFailGo(E_FAIL);
        }

    if (_fullpath(fullPath, fileName, _MAX_PATH) == nullptr)
    {
        IfFailGo(E_FAIL);
    }

    // canonicalize that path name to lower case for the profile storage
    // REVIEW: Assuming no utf8 characters here
    len = strlen(fullPath);
    for (size_t i = 0; i < len; i++)
    {
        fullPath[i] = (char) tolower(fullPath[i]);
    }

    if (HostConfigFlags::flags.GenerateLibraryByteCodeHeaderIsEnabled)
    {
        if (isUtf8)
        {
            if (HostConfigFlags::flags.GenerateLibraryByteCodeHeader != nullptr && *HostConfigFlags::flags.GenerateLibraryByteCodeHeader != _u('\0'))
            {
                CHAR libraryName[_MAX_PATH];
                CHAR ext[_MAX_EXT];
                _splitpath_s(fullPath, NULL, 0, NULL, 0, libraryName, _countof(libraryName), ext, _countof(ext));

                    IfFailGo(CreateLibraryByteCodeHeader(fileContents, (BYTE*)contentsRaw, lengthBytes, HostConfigFlags::flags.GenerateLibraryByteCodeHeader, libraryName));
                }
                else
                {
                    fwprintf(stderr, _u("FATAL ERROR: -GenerateLibraryByteCodeHeader must provide the file name, i.e., -GenerateLibraryByteCodeHeader:<bytecode file name>, exiting\n"));
                    IfFailGo(E_FAIL);
                }
            }
            else
            {
                fwprintf(stderr, _u("FATAL ERROR: GenerateLibraryByteCodeHeader flag can only be used on UTF8 file, exiting\n"));
                IfFailGo(E_FAIL);
            }
        }
        else if(HostConfigFlags::flags.SerializedIsEnabled)
        {
            if(isUtf8)
            {
                CreateAndRunSerializedScript(fileName, fileContents, fullPath);
            }
            else
            {
                fwprintf(stderr, _u("FATAL ERROR: Serialized flag can only be used on UTF8 file, exiting\n"));
                IfFailGo(E_FAIL);
            }
        }
        else
        {
            IfFailGo(RunScript(fileName, fileContents, nullptr, fullPath));
        }
    }

Error:
    if (Debugger::debugger != nullptr)
    {
        Debugger::debugger->CompareOrWriteBaselineFile(fileName);
        Debugger::CloseDebugger();
    }

    ChakraRTInterface::JsSetCurrentContext(nullptr);

    if (runtime != JS_INVALID_RUNTIME_HANDLE)
    {
        ChakraRTInterface::JsDisposeRuntime(runtime);
    }

    _flushall();

    return hr;
}

HRESULT ExecuteTestWithMemoryCheck(char* fileName)
{
    HRESULT hr = E_FAIL;
#ifdef CHECK_MEMORY_LEAK
    // Always check memory leak, unless user specified the flag already
    if (!ChakraRTInterface::IsEnabledCheckMemoryFlag())
    {
        ChakraRTInterface::SetCheckMemoryLeakFlag(true);
    }

    // Disable the output in case an unhandled exception happens
    // We will re-enable it if there is no unhandled exceptions
    ChakraRTInterface::SetEnableCheckMemoryLeakOutput(false);
#endif

#ifdef _WIN32
    __try
    {
        hr = ExecuteTest(fileName);
    }
    __except (HostExceptionFilter(GetExceptionCode(), GetExceptionInformation()))
    {
        Assert(false);
    }
#else
    // REVIEW: Do we need a SEH handler here?
    hr = ExecuteTest(fileName);
    if (FAILED(hr)) exit(0);
#endif // _WIN32

    _flushall();
#ifdef CHECK_MEMORY_LEAK
    ChakraRTInterface::SetEnableCheckMemoryLeakOutput(true);
#endif
    return hr;
}


unsigned int WINAPI StaticThreadProc(void *lpParam)
{
    ChakraRTInterface::ArgInfo* argInfo = static_cast<ChakraRTInterface::ArgInfo* >(lpParam);
    return ExecuteTestWithMemoryCheck(argInfo->filename);
}

int _cdecl wmain(int argc, __in_ecount(argc) LPWSTR argv[])
{
    if(argc < 2)
    {
        PrintUsage();
        return EXIT_FAILURE;
    }

    int cpos = 0;
    for(int i = 0; i < argc; ++i)
    {
        if(wcsstr(argv[i], _u("-TTRecord:")) == argv[i])
        {
            doTTRecord = true;
            ttUri = argv[i] + wcslen(_u("-TTRecord:"));
        }
        else if(wcsstr(argv[i], _u("-TTDebug:")) == argv[i])
        {
            doTTDebug = true;
            ttUri = argv[i] + wcslen(_u("-TTDebug:"));
        }
        else if(wcsstr(argv[i], _u("-TTSnapInterval:")) == argv[i])
        {
            LPCWSTR intervalStr = argv[i] + wcslen(_u("-TTSnapInterval:"));
            snapInterval = (UINT32)_wtoi(intervalStr);
        }
        else if(wcsstr(argv[i], _u("-TTHistoryLength:")) == argv[i])
        {
            LPCWSTR historyStr = argv[i] + wcslen(_u("-TTHistoryLength:"));
            snapHistoryLength = (UINT32)_wtoi(historyStr);
        }
        else if(wcsstr(argv[i], _u("--debug-brk=")) == argv[i])
        {
            dbgIPAddr = _u("127.0.0.1");

            LPCWSTR portStr = argv[i] + wcslen(_u("--debug-brk="));
            dbgPort = (unsigned short)_wtoi(portStr);
        }
        else
        {
            argv[cpos] = argv[i];
            cpos++;
        }
    }
    argc = cpos;

    HostConfigFlags::pfnPrintUsage = PrintUsageFormat;

    // The following code is present to make sure we don't load
    // jscript9.dll etc with ch. Since that isn't a concern on non-Windows
    // builds, it's safe to conditionally compile it out.
#ifdef _WIN32
    ATOM lock = ::AddAtom(szChakraCoreLock);
    AssertMsg(lock, "failed to lock chakracore.dll");
#endif // _WIN32

    HostConfigFlags::HandleArgsFlag(argc, argv);

    ChakraRTInterface::ArgInfo argInfo = { argc, argv, PrintUsage, nullptr };
    HINSTANCE chakraLibrary = ChakraRTInterface::LoadChakraDll(&argInfo);

    if (argInfo.filename == nullptr) {
        Helpers::WideStringToNarrowDynamic(argv[1], &argInfo.filename);
    }

    if (chakraLibrary != nullptr)
    {
#ifdef _WIN32
        HANDLE threadHandle;
        threadHandle = reinterpret_cast<HANDLE>(_beginthreadex(0, 0, &StaticThreadProc, &argInfo, STACK_SIZE_PARAM_IS_A_RESERVATION, 0));

        if (threadHandle != nullptr)
        {
            DWORD waitResult = WaitForSingleObject(threadHandle, INFINITE);
            Assert(waitResult == WAIT_OBJECT_0);
            CloseHandle(threadHandle);
        }
        else
        {
            fwprintf(stderr, _u("FATAL ERROR: failed to create worker thread error code %d, exiting\n"), errno);
            AssertMsg(false, "failed to create worker thread");
        }
#else
        // On linux, execute on the same thread
        ExecuteTestWithMemoryCheck(argInfo.filename);
#endif
        ChakraRTInterface::UnloadChakraDll(chakraLibrary);
    }

    return 0;
}

#ifndef _WIN32
int main(int argc, char** argv)
{
    PAL_InitializeChakraCore(argc, argv);

    const char* strProgramFile = argv[0];

    // Ignoring mem-alloc failures here as this is
    // simply a test tool. We can add more error checking
    // here later if desired.
    char16** args = new char16*[argc];
    for (int i = 0; i < argc; i++)
    {
        Helpers::NarrowStringToWideDynamic(argv[i], &args[i]);
    }

    // Call wmain with a copy of args, as HostConfigFlags may change argv
    char16** argsCopy = new char16*[argc];
    memcpy(argsCopy, args, sizeof(args[0]) * argc);
    int ret = wmain(argc, argsCopy);
    delete[] argsCopy;

    for (int i = 0; i < argc; i++)
    {
        free(args[i]);
    }
    delete[] args;

    PAL_Shutdown();
    return ret;
}
#endif // !_WIN32
