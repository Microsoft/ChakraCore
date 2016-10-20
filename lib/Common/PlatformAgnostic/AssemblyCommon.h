//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#ifndef _WIN32

extern "C" void __register_frame(const void* ehframe);
extern "C" void __deregister_frame(const void* ehframe);

#if defined(_AMD64_) && !defined(DISABLE_JIT)
#if defined(__APPLE__) // no multi fde support
#ifndef __IOS__
#define CC_FDE_SIZE 64
typedef void(*mac_fde_reg_op)(const void*addr);
void mac_fde_wrapper(const char *dataStart, mac_fde_reg_op reg_op);
#define __REGISTER_FRAME(addr) mac_fde_wrapper((const char*)addr, __register_frame)
#define __DEREGISTER_FRAME(addr) mac_fde_wrapper((const char*)addr, __deregister_frame)
#else // __IOS__ // no JIT
#define __REGISTER_FRAME(addr)
#define __DEREGISTER_FRAME(addr)
#endif // !__IOS__
#else // multi FDE support
#define __REGISTER_FRAME(addr) __register_frame(addr)
#define __DEREGISTER_FRAME(addr) __deregister_frame(addr)
#endif // __APPLE__
#endif // _AMD64_ && !DISABLE_JIT

#endif // !_WIN32
