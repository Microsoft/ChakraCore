//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "stdafx.h"

#define CATCH_CONFIG_RUNNER
#pragma warning(push)
// conversion from 'int' to 'char', possible loss of data
#pragma warning(disable:4242)
#pragma warning(disable:4244)
#pragma warning(disable:26434)
#pragma warning(disable:26439)
#pragma warning(disable:26451)
#pragma warning(disable:26495)
#include "catch.hpp"
#pragma warning(pop)

TestHooks g_testHooks = { 0 };
bool g_testHooksLoaded = false;

extern "C"
HRESULT __stdcall OnChakraCoreLoadedEntry(TestHooks& testHooks)
{
    g_testHooks = testHooks;
    g_testHooksLoaded = true;
    return S_OK;
}

// Use nativetests.exe -? to get all command line options
int _cdecl main(int argc, char * const argv[])
{
    return Catch::Session().run(argc, argv);
}
