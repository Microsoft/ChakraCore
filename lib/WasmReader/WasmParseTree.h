//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

namespace Wasm
{

    namespace WasmTypes
    {
        enum WasmType
        {
            Void,
#define WASM_LOCALTYPE(token, name) token,
#include "WasmKeywords.h"
            Limit
        };
    }

    enum WasmOp
    {
#define WASM_KEYWORD(token, name) wn##token,
#include "WasmKeywords.h"
        wnLIMIT
    };

    struct WasmFuncNode
    {
        WasmFunctionInfo * info;

    };

    struct WasmConstLitNode
    {
        union
        {
            float f32;
            double f64;
            int32 i32;
            int64 i64;
        };
    };

    struct WasmVarNode
    {
        uint num;
        union
        {
            LPCUTF8 exportName;
        };
    };

    struct WasmBlockNode
    {
        uint32 count;
    };

    struct WasmMemOpNode
    {
        uint32 offset;
        uint8 alignment;
    };

    struct WasmBrNode
    {
        uint32 depth;
        bool hasSubExpr;
    };

    struct WasmBrTableNode
    {
        uint32 numTargets;
        uint32* targetTable;
        uint32 defaultTarget;
    };

    struct WasmNode
    {
        WasmOp op;
        union
        {
            WasmVarNode var;
            WasmConstLitNode cnst;
            WasmFuncNode func;
            WasmBlockNode block;
            WasmBrNode br;
            WasmBrTableNode brTable;
            WasmMemOpNode mem;
        };
    };

    struct WasmExport
    {
        uint32 funcIndex;
        uint32 nameLength;
        char16* name;
    };

    struct WasmImport
    {
        uint32 sigId;
        uint32 modNameLen;
        char16* modName;
        uint32 fnNameLen;
        char16* fnName;
    };
}

#define FOREACH_WASMNODE_IN_LIST(node, head) \
    Wasm::WasmNode * node; \
    Wasm::WasmNode * wasmNodeList; \
    if (head->op == wnLIST) \
    { \
        wasmNodeList = head; \
        node = head->bin.lhs; \
    } \
    else \
    { \
        wasmNodeList = nullptr; \
        node = head; \
    } \
    while (node != nullptr) \
    {

#define NEXT_WASMNODE_IN_LIST(node) \
        if (wasmNodeList != nullptr && wasmNodeList->bin.rhs != nullptr) \
        { \
            wasmNodeList = wasmNodeList->bin.rhs; \
            node = wasmNodeList->bin.lhs; \
        } \
        else \
        { \
            node = nullptr; \
        } \
    }
