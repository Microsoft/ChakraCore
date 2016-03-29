//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "WasmKeywordTypes.h"

// memory
WASM_MEMREAD_FDI(LOAD,    load)
WASM_MEMREAD_I(LOAD8S,     load8_s)
WASM_MEMREAD_I(LOAD16S,    load16_s)
WASM_MEMREAD_I(LOAD8U,     load8_u)
WASM_MEMREAD_I(LOAD16U,    load16_u)
WASM_MEMSTORE_FDI(STORE,   store)
WASM_MEMSTORE_I(STORE8,    store8)
WASM_MEMSTORE_I(STORE16,   store16)

WASM_KEYWORD(GETPARAM,               getparam)
WASM_KEYWORD(GETLOCAL,               get_local)
WASM_KEYWORD(SETLOCAL,               set_local)

// types
WASM_MEMTYPE(I8,     i8)
WASM_MEMTYPE(I16,    i16)

WASM_LOCALTYPE(I32,    i32)
WASM_LOCALTYPE(I64,    i64)
WASM_LOCALTYPE(F32,    f32)
WASM_LOCALTYPE(F64,    f64)

// calls
WASM_KEYWORD(CALL, call)
WASM_KEYWORD(CALL_INDIRECT, call_indirect)
WASM_KEYWORD(CALL_IMPORT, call_import)

// control flow ops
WASM_KEYWORD(BLOCK,      block)
WASM_KEYWORD(SELECT,     select)
WASM_KEYWORD(IF,         if)
WASM_KEYWORD(IF_ELSE,    if_else)
WASM_KEYWORD(LOOP,       loop)
WASM_KEYWORD(LABEL,      label)
WASM_KEYWORD(BR,         br)
WASM_KEYWORD(BR_IF,      br_if)
WASM_KEYWORD(BR_TABLE,   br_table)
WASM_KEYWORD(RETURN,     return)

WASM_KEYWORD(NOP,        nop)
WASM_KEYWORD_FDI(CONST,  const)

// structures
WASM_KEYWORD(FUNC,       func)
WASM_KEYWORD(PARAM,      param)
WASM_KEYWORD(RESULT,     result)
WASM_KEYWORD(LOCAL,      local)
WASM_KEYWORD(MODULE,     module)
WASM_KEYWORD(EXPORT,     export)
WASM_KEYWORD(IMPORT,     import)
WASM_KEYWORD(TABLE,      table)
WASM_KEYWORD(MEMORY,     memory)
WASM_KEYWORD(TYPE,       type)

// unary ops
WASM_KEYWORD_UNARY_I(NOT,    not, Not)
WASM_KEYWORD_UNARY_I(CLZ,    clz, Clz32)
WASM_KEYWORD_UNARY_FD(NEG,    neg, Neg)
WASM_KEYWORD_UNARY_FD(ABS,    abs, Abs)
WASM_KEYWORD_UNARY_FD(CEIL,   ceil, Ceil)
WASM_KEYWORD_UNARY_FD(FLOOR,  floor, Floor)

WASM_KEYWORD_UNARY_FD(SQRT, sqrt, Sqrt)

// TODO: michhol, new ops
// WASM_KEYWORD_UNARY_FD(TRUNC, trunc, Trunc)
// WASM_KEYWORD_UNARY_I(CTZ,    ctz, Ctz)
// WASM_KEYWORD_UNARY_I(POPCNT, popcnt, PopCnt)
// WASM_KEYWORD_UNARY_FD(ROUND,  round, Round)

// conversion ops
WASM_KEYWORD_UNARY(TRUNC_S_F32_I32, trunc_s, Conv_FTI, I32, F32)
WASM_KEYWORD_UNARY(TRUNC_S_F64_I32, trunc_s, Conv_DTI, I32, F64)
WASM_KEYWORD_UNARY(TRUNC_U_F32_I32, trunc_u, Conv_FTU, I32, F32)
WASM_KEYWORD_UNARY(TRUNC_U_F64_I32, trunc_u, Conv_DTU, I32, F64)
WASM_KEYWORD_UNARY(CONVERT_S_I32_F32, convert_s, Fround_Int, F32, I32)
WASM_KEYWORD_UNARY(CONVERT_S_I32_F64, convert_s, Conv_ITD, F64, I32)
WASM_KEYWORD_UNARY(CONVERT_U_I32_F32, convert_u, Conv_UTF, F32, I32)
WASM_KEYWORD_UNARY(CONVERT_U_I32_F64, convert_u, Conv_UTD, F64, I32)
WASM_KEYWORD_UNARY(PROMOTE_F32_F64, promote, Conv_FTD, F64, F32)
WASM_KEYWORD_UNARY(DEMOTE_F64_F32, demote, Fround_Db, F32, F64)
WASM_KEYWORD_UNARY(REINTERPRET_F32_I32, reinterpret, Reinterpret_FTI, I32, F32)
WASM_KEYWORD_UNARY(REINTERPRET_I32_F32, reinterpret, Reinterpret_ITF, F32, I32)

// binary ops
WASM_KEYWORD_BIN_MATH_FDI(ADD, add, Add)
WASM_KEYWORD_BIN_MATH_FDI(SUB, sub, Sub)
WASM_KEYWORD_BIN_MATH_FDI(MUL, mul, Mul)

WASM_KEYWORD_BIN_MATH_I(DIVS, divs, Div_Int)
WASM_KEYWORD_BIN_MATH_I(MODS, mods, Rem_Int)
WASM_KEYWORD_BIN_MATH_I(AND, and, And_Int)
WASM_KEYWORD_BIN_MATH_I(OR, or, Or_Int)
WASM_KEYWORD_BIN_MATH_I(XOR, xor, Xor_Int)
WASM_KEYWORD_BIN_MATH_I(SHL, shl, Shl_Int)
WASM_KEYWORD_BIN_MATH_I(SHRS, shr_s, Shr_Int)
WASM_KEYWORD_BIN_MATH_I(SHRU, shr_u, ShrU_Int)
WASM_KEYWORD_BIN_MATH_I(DIVU, divu, Div_UInt)
WASM_KEYWORD_BIN_MATH_I(MODU, modu, Rem_UInt)
WASM_KEYWORD_BIN_MATH_I(ROR, rotr, Ror_Int)
WASM_KEYWORD_BIN_MATH_I(ROL, rotl, Rol_Int)

WASM_KEYWORD_BIN_MATH_FD(DIV, div, Div)
WASM_KEYWORD_BIN_MATH_FD(COPYSIGN, copysign, Copysign)

WASM_KEYWORD_BIN_MATH_D(MOD, mod, Rem_Db)
WASM_KEYWORD_BIN_MATH_D(MIN, min, Min_Db)
WASM_KEYWORD_BIN_MATH_D(MAX, max, Max_Db)


// compare ops
WASM_KEYWORD_COMPARE_FDI(EQ,    eq, CmEq)
WASM_KEYWORD_COMPARE_FDI(NEQ,   neq, CmNe)
WASM_KEYWORD_COMPARE_I(LTS,    lts, CmLt_Int)
WASM_KEYWORD_COMPARE_I(LTU,    ltu, CmLt_UnInt)
WASM_KEYWORD_COMPARE_I(LES,    les, CmLe_Int)
WASM_KEYWORD_COMPARE_I(LEU,    leu, CmLe_UnInt)
WASM_KEYWORD_COMPARE_I(GTS,    gts, CmGt_Int)
WASM_KEYWORD_COMPARE_I(GTU,    gtu, CmGt_UnInt)
WASM_KEYWORD_COMPARE_I(GES,    ges, CmGe_Int)
WASM_KEYWORD_COMPARE_I(GEU,    geu, CmGe_UnInt)
WASM_KEYWORD_COMPARE_I(EQZ,    eqz, CmEqz_Int)
WASM_KEYWORD_COMPARE_FD(LT,     lt, CmLt)
WASM_KEYWORD_COMPARE_FD(LE,     le, CmLe)
WASM_KEYWORD_COMPARE_FD(GT,     gt, CmGt)
WASM_KEYWORD_COMPARE_FD(GE,     ge, CmGe)

#undef WASM_MEMSTORE_FDI
#undef WASM_MEMSTORE_I
#undef WASM_MEMSTORE
#undef WASM_MEMREAD_FDI
#undef WASM_MEMREAD_I
#undef WASM_MEMREAD
#undef WASM_MEMOP
#undef WASM_KEYWORD_COMPARE_FDI
#undef WASM_KEYWORD_COMPARE_FD
#undef WASM_KEYWORD_COMPARE_I
#undef WASM_KEYWORD_COMPARE_D
#undef WASM_KEYWORD_COMPARE_F
#undef WASM_KEYWORD_COMPARE
#undef WASM_KEYWORD_UNARY_I
#undef WASM_KEYWORD_UNARY_D
#undef WASM_KEYWORD_UNARY_FD
#undef WASM_KEYWORD_UNARY
#undef WASM_KEYWORD_BIN_MATH_FDI
#undef WASM_KEYWORD_BIN_MATH_FD
#undef WASM_KEYWORD_BIN_MATH_I
#undef WASM_KEYWORD_BIN_MATH_D
#undef WASM_KEYWORD_BIN_MATH_F
#undef WASM_KEYWORD_BIN_TYPED
#undef WASM_KEYWORD_BIN
#undef WASM_LOCALTYPE
#undef WASM_MEMTYPE
#undef WASM_KEYWORD_FDI
#undef WASM_KEYWORD
