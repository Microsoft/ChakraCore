//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

// Format: FLAG*(ThreadFlag, GlobalFlag)
FLAG(IsCollectGarbageEnabled, CollectGarbage)
FLAG(IsErrorStackTraceEnabled, errorStackTrace)
FLAG(IsES6UnicodeVerboseEnabled, ES6UnicodeVerbose)
FLAG_RELEASE(IsIntlEnabled, Intl)
FLAG_RELEASE(IsES6SpeciesEnabled, ES6Species)
FLAG_RELEASE(IsES6ClassAndExtendsEnabled, ES6Classes)
FLAG_RELEASE(IsES6DateParseFixEnabled, ES6DateParseFix)
FLAG_RELEASE(IsES6DefaultArgsEnabled, ES6DefaultArgs)
FLAG_RELEASE(IsES6DestructuringEnabled, ES6Destructuring)
FLAG_RELEASE(IsES6ForLoopSemanticsEnabled, ES6ForLoopSemantics)
FLAG_RELEASE(IsES6FunctionNameEnabled, ES6FunctionName)
FLAG_RELEASE(IsES6FunctionNameFullEnabled, ES6FunctionNameFull)
FLAG_RELEASE(IsES6GeneratorsEnabled, ES6Generators)
FLAG_RELEASE(IsES7ExponentiationOperatorEnabled, ES7ExponentiationOperator)
FLAG_RELEASE(IsES7TrailingCommaEnabled, ES7TrailingComma)
FLAG_RELEASE(IsES7BuiltinsEnabled, ES7Builtins)
FLAG_RELEASE(IsES6IsConcatSpreadableEnabled, ES6IsConcatSpreadable)
FLAG_RELEASE(IsES6ObjectLiteralsEnabled, ES6ObjectLiterals)
FLAG_RELEASE(IsES6ProxyEnabled, ES6Proxy)
FLAG_RELEASE(IsES6RestEnabled, ES6Rest)
FLAG_RELEASE(IsES6SpreadEnabled, ES6Spread)
FLAG_RELEASE(IsES6StringPrototypeFixEnabled, ES6StringPrototypeFixes)
FLAG_RELEASE(IsES6PrototypeChain, ES6PrototypeChain)
FLAG_RELEASE(IsES6ToPrimitiveEnabled, ES6ToPrimitive)
FLAG_RELEASE(IsES6ToLengthEnabled, ES6ToLength)
FLAG_RELEASE(IsES6ToStringTagEnabled, ES6ToStringTag)
FLAG_RELEASE(IsES6TypedArrayExtensionsEnabled, ES6TypedArrayExtensions)
FLAG_RELEASE(IsES6UnicodeExtensionsEnabled, ES6Unicode)
FLAG_RELEASE(IsES6RegExStickyEnabled, ES6RegExSticky)
FLAG_RELEASE(IsES6RegExPrototypePropertiesEnabled, ES6RegExPrototypeProperties)
FLAG_RELEASE(IsES6HasInstanceEnabled, ES6HasInstance)
FLAG_RELEASE(SkipSplitOnNoResult, SkipSplitOnNoResult)
FLAG_RELEASE(IsES7AsyncAndAwaitEnabled, ES7AsyncAwait)
FLAG_RELEASE(IsArrayBufferTransferEnabled, ArrayBufferTransfer)
#ifdef ENABLE_PROJECTION
FLAG(AreWinRTDelegatesInterfaces, WinRTDelegateInterfaces)
FLAG_RELEASE(IsWinRTAdaptiveAppsEnabled, WinRTAdaptiveApps)
#endif
//SIMD_JS
FLAG_RELEASE(IsSimdjsEnabled, Simdjs)

