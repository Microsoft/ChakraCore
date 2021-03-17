//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

//
// NOTE: This file is intended to be "#include" multiple times. The call site should define various
// macros to be executed for each entry. Macros that are not provided will be given a default,
// empty implementation.
//

//
// NOTE: After modifying this file you might observe unit test failures, especially jd/Debugger unit test failures.
// This can be caused by the build-generated bytecode files in Library/InJavascript becoming invalidated.
// Run regenByteCode.py to update bytecode. (Note: this will build and regen all variants and may take some time.)
//

#if !defined(ENTRY_INTERNAL_SYMBOL)
#define ENTRY_INTERNAL_SYMBOL
#endif
#if !defined(ENTRY_SYMBOL)
#define ENTRY_SYMBOL(name, description)
#endif
#if !defined(ENTRY)
#define ENTRY(name)
#endif
#if !defined(ENTRY2)
#define ENTRY2(name, str)
#endif
ENTRY_SYMBOL(_symbolAsyncIterator, _u("Symbol.asyncIterator"))
ENTRY_SYMBOL(_symbolHasInstance, _u("Symbol.hasInstance"))
ENTRY_SYMBOL(_symbolIsConcatSpreadable, _u("Symbol.isConcatSpreadable"))
ENTRY_SYMBOL(_symbolIterator, _u("Symbol.iterator"))
ENTRY_SYMBOL(_symbolMatch, _u("Symbol.match"))
ENTRY_SYMBOL(_symbolReplace, _u("Symbol.replace"))
ENTRY_SYMBOL(_symbolSearch, _u("Symbol.search"))
ENTRY_SYMBOL(_symbolSplit, _u("Symbol.split"))
ENTRY_SYMBOL(_symbolSpecies, _u("Symbol.species"))
ENTRY_SYMBOL(_symbolToPrimitive, _u("Symbol.toPrimitive"))
ENTRY_SYMBOL(_symbolToStringTag, _u("Symbol.toStringTag"))
ENTRY_SYMBOL(_symbolUnscopables, _u("Symbol.unscopables"))

// math functions must remain contiguous for quick modification check
ENTRY(abs)
ENTRY(acos)
ENTRY(asin)
ENTRY(atan)
ENTRY(atan2)
ENTRY(ceil)
ENTRY(cos)
ENTRY(exp)
ENTRY(floor)
ENTRY(log)
ENTRY(LOG10E)
ENTRY(LOG2E)
ENTRY(min)
ENTRY(PI)
ENTRY(pow)
ENTRY(random)
ENTRY(round)
ENTRY(sin)
ENTRY(sqrt)
ENTRY(tan)
ENTRY(log10)
ENTRY(log2)
ENTRY(log1p)
ENTRY(expm1)
ENTRY(cosh)
ENTRY(sinh)
ENTRY(tanh)
ENTRY(acosh)
ENTRY(asinh)
ENTRY(atanh)
ENTRY(hypot)
ENTRY(trunc)
ENTRY(sign)
ENTRY(cbrt)
ENTRY(imul)
ENTRY(clz32)
ENTRY(fround)
// End math functions

ENTRY(Promise)
ENTRY(asyncIterator)
ENTRY(load)
ENTRY(store)
ENTRY2(and_, _u("and"))
ENTRY2(or_, _u("or"))
ENTRY2(xor_, _u("xor"))
ENTRY(add)
ENTRY(all)
ENTRY(allSettled)
ENTRY(anchor)
ENTRY(apply)
ENTRY(Array)
ENTRY(assign)
ENTRY(at)
ENTRY(atEnd)
ENTRY(big)
ENTRY(bind)
ENTRY(blink)
ENTRY(bold)
ENTRY2(boolean_, _u("boolean")) // "boolean" cannot be an identifier in C++ so using "boolean_" instead
ENTRY(Boolean)
ENTRY(JsBuiltIn)
ENTRY(call)
ENTRY(CanvasPixelArray)
ENTRY(cast)
ENTRY(cause)
ENTRY2(catch_, _u("catch")) // "catch" cannot be an identifier in C++ so using "catch_" instead
ENTRY(charAt)
ENTRY(charCodeAt)
ENTRY(codePointAt)
ENTRY(clear)
ENTRY(copyWithin)
ENTRY(CollectGarbage)
ENTRY(concat)
ENTRY(construct)
ENTRY(constructor)
ENTRY(ConversionError)
ENTRY(create)
ENTRY(date)
ENTRY(Date)
ENTRY(decodeURI)
ENTRY(decodeURIComponent)
ENTRY2(default_, _u("default"))
ENTRY2(delete_, _u("delete")) // "delete" cannot be an identifier in C++ so using "delete_" instead
ENTRY2(star_, _u("*"))
ENTRY(deleteProperty)
ENTRY(description)
ENTRY(detach)
ENTRY(done)
ENTRY(E)
ENTRY(encodeURI)
ENTRY(encodeURIComponent)
ENTRY(endsWith)
ENTRY(entries)
ENTRY(enumerate)
ENTRY(Enumerator)
ENTRY(EPSILON)
ENTRY(escape)
ENTRY(Error)
ENTRY(eval)
ENTRY(EvalError)
ENTRY(every)
ENTRY(exec)
ENTRY2(false_, _u("false")) // "false" cannot be an identifier in C++ so using "false_" instead
ENTRY(flags)
ENTRY(flat)
ENTRY(flatMap)
ENTRY(fill)
ENTRY(filter)
ENTRY(finally)
ENTRY(find)
ENTRY(findIndex)
ENTRY(findLast)
ENTRY(findLastIndex)
ENTRY(fixed)
ENTRY(fontcolor)
ENTRY(fontsize)
ENTRY2(for_, _u("for")) // "for" cannot be an identifier in C++ so using "for_" instead
ENTRY(forEach)
ENTRY(freeze)
ENTRY(from)
ENTRY(fromCharCode)
ENTRY(fromCodePoint)
ENTRY(fromEntries)
ENTRY(fulfilled)
ENTRY(function)
ENTRY(Function)
ENTRY(getDate)
ENTRY(getDay)
ENTRY(getFullYear)
ENTRY(getHours)
ENTRY(getMilliseconds)
ENTRY(getMinutes)
ENTRY(getMonth)
ENTRY(GetObject)
ENTRY(getOwnPropertyDescriptor)
ENTRY(getOwnPropertyDescriptors)
ENTRY(getOwnPropertyNames)
ENTRY(getOwnPropertySymbols)
ENTRY(getSeconds)
ENTRY(getTime)
ENTRY(getTimezoneOffset)
ENTRY(getUTCDate)
ENTRY(getUTCDay)
ENTRY(getUTCFullYear)
ENTRY(getUTCHours)
ENTRY(getUTCMilliseconds)
ENTRY(getUTCMinutes)
ENTRY(getUTCMonth)
ENTRY(getUTCSeconds)
ENTRY(getYear)
ENTRY(has)
ENTRY(hasInstance)
ENTRY(hasOwnProperty)
ENTRY(includes)
ENTRY(indexOf)
ENTRY(Infinity)
ENTRY(Intl)
ENTRY(invokeJit)
ENTRY(is)
ENTRY(isArray)
ENTRY(isConcatSpreadable)
ENTRY(isExtensible)
ENTRY(isFinite)
ENTRY(isFrozen)
ENTRY(isInteger)
ENTRY(isNaN)
ENTRY(isPrototypeOf)
ENTRY(isSafeInteger)
ENTRY(isSealed)
ENTRY(isView)
ENTRY(italics)
ENTRY(item)
ENTRY(iterator)
ENTRY(join)
ENTRY(JSON)
ENTRY(keyFor)
ENTRY(keys)
ENTRY(lastIndexOf)
ENTRY(length)
ENTRY(link)
ENTRY(LN10)
ENTRY(LN2)
ENTRY(map)
ENTRY(Map)
ENTRY(Math)

// Wasm

ENTRY(WebAssembly)
ENTRY(Module)
ENTRY(Instance)
ENTRY(Memory)
ENTRY(Table)
ENTRY(CompileError)
ENTRY(RuntimeError)
ENTRY(LinkError)
ENTRY(validate)
ENTRY(compileStreaming)
ENTRY(instantiate)
ENTRY(instantiateStreaming)
ENTRY(grow)
ENTRY(module)
ENTRY(instance)
ENTRY(kind)
ENTRY(exports)
ENTRY(imports)
ENTRY(customSections)
ENTRY(initial)
ENTRY(maximum)
ENTRY(shared)
ENTRY(element)
ENTRY(low)
ENTRY(high)
ENTRY(arrayBuffer)
// wabt
ENTRY(wabt)
ENTRY(convertWast2Wasm)
ENTRY(spec)
ENTRY(commands)
ENTRY(type)
ENTRY(line)
ENTRY(action)
ENTRY(field)
ENTRY(args)
ENTRY(as)
ENTRY(text)
ENTRY(expected)
// End Wasm

ENTRY(max)
ENTRY(MAX_SAFE_INTEGER)
ENTRY(MAX_VALUE)
ENTRY(match)
ENTRY(message)
ENTRY(MIN_SAFE_INTEGER)
ENTRY(MIN_VALUE)
ENTRY(NaN)
ENTRY(name)
ENTRY(next)
ENTRY(normalize)
ENTRY(now)
ENTRY(null)
ENTRY(Number)
ENTRY(number)
ENTRY(BigInt)
ENTRY(NEGATIVE_INFINITY)
ENTRY(ownKeys)
ENTRY(object)
ENTRY(Object)
ENTRY2(object_Arguments, _u("[object Arguments]"))
ENTRY2(object_Array, _u("[object Array]"))
ENTRY2(object_Boolean, _u("[object Boolean]"))
ENTRY2(object_Date, _u("[object Date]"))
ENTRY2(object_Error, _u("[object Error]"))
ENTRY2(object_Function, _u("[object Function]"))
ENTRY2(object_Number, _u("[object Number]"))
ENTRY2(object_Object, _u("[object Object]"))
ENTRY2(object_RegExp, _u("[object RegExp]"))
ENTRY2(object_String, _u("[object String]"))
ENTRY(of)
ENTRY(parse)
ENTRY(parseFloat)
ENTRY(parseInt)
ENTRY(pop)
ENTRY(POSITIVE_INFINITY)
ENTRY(preventExtensions)
ENTRY(propertyIsEnumerable)
ENTRY(prototype)
ENTRY(proxy)
ENTRY(Proxy)
ENTRY(push)
ENTRY(race)
ENTRY(RangeError)
ENTRY(raw)
ENTRY(reason)
ENTRY(reduce)
ENTRY(reduceRight)
ENTRY(ReferenceError)
ENTRY(Reflect)
ENTRY(RegExpError)
ENTRY(reject)
ENTRY(rejected)
ENTRY(replace)
ENTRY(resolve)
ENTRY2(return_, _u("return")) // "return" cannot be an identifier in C++ so using "return_" instead
ENTRY(reverse)
ENTRY(revocable)
ENTRY(revoke)
ENTRY(RegExp)
ENTRY(repeat)
ENTRY(seal)
ENTRY(search)
ENTRY(Set)
ENTRY(setDate)
ENTRY(setFullYear)
ENTRY(setHours)
ENTRY(setMilliseconds)
ENTRY(setMinutes)
ENTRY(setMonth)
ENTRY(setSeconds)
ENTRY(setTime)
ENTRY(setUTCDate)
ENTRY(setUTCFullYear)
ENTRY(setUTCHours)
ENTRY(setUTCMilliseconds)
ENTRY(setUTCMinutes)
ENTRY(setUTCMonth)
ENTRY(setUTCSeconds)
ENTRY(setYear)
ENTRY(shift)
ENTRY(size)
ENTRY(slice)
ENTRY(some)
ENTRY(sort)
ENTRY(source)
ENTRY(species)
ENTRY(splice)
ENTRY(split)
ENTRY(SQRT1_2)
ENTRY(SQRT2)
ENTRY(status)
ENTRY(strike)
ENTRY(string)
ENTRY(String)
ENTRY(sub)
ENTRY(substring)
ENTRY(substr)
ENTRY(sup)
ENTRY(stack)
ENTRY(stackTraceLimit)
ENTRY(startsWith)
ENTRY(stringify)
ENTRY(symbol)
ENTRY(Symbol)
ENTRY(SyntaxError)
ENTRY(test)
ENTRY(then)
ENTRY2(throw_, _u("throw")) // "throw" cannot be an identifier in C++ so using "throw_" instead
ENTRY(toDateString)
ENTRY(toExponential)
ENTRY(toFixed)
ENTRY(toISOString)
ENTRY(toJSON)
ENTRY(toLocaleDateString)
ENTRY(toLocaleLowerCase)
ENTRY(toLocaleString)
ENTRY(toLocaleTimeString)
ENTRY(toLocaleUpperCase)
ENTRY(toLowerCase)
ENTRY(toPrecision)
ENTRY(toPrimitive)
ENTRY(toString)
ENTRY(toStringTag)
ENTRY(toTimeString)
ENTRY(toUpperCase)
ENTRY(toUTCString)
ENTRY(trim)
ENTRY(trimLeft)
ENTRY(trimStart)
ENTRY(trimRight)
ENTRY(trimEnd)
ENTRY(padStart)
ENTRY(padEnd)
ENTRY2(true_, _u("true")) // "true_" cannot be an identifier in C++ so using "true_" instead
ENTRY(TypeError)
ENTRY(undefined)
ENTRY(unescape)
ENTRY(unknown)
ENTRY(unscopables)
ENTRY(unshift)
ENTRY(URIError)
ENTRY(UTC)
ENTRY(valueOf)
ENTRY(values)
ENTRY(WeakMap)
ENTRY(WeakSet)
ENTRY(setNonUserCodeExceptions)
ENTRY(debuggerEnabled)
ENTRY(enumerable)
ENTRY(configurable)
ENTRY(writable)
ENTRY(value)
ENTRY(get)
ENTRY(getPrototypeOf)
ENTRY(setPrototypeOf)
ENTRY(set)
ENTRY(defineProperty)
ENTRY(defineProperties)
ENTRY(toGMTString)
ENTRY(compile)
ENTRY(global)
ENTRY(globalThis)
ENTRY(lastIndex)
ENTRY(multiline)
ENTRY(dotAll)
ENTRY(ignoreCase)
ENTRY(unicode)
ENTRY(sticky)
ENTRY(byteOffset)
ENTRY(byteLength)
ENTRY(buffer)
ENTRY(BYTES_PER_ELEMENT)
ENTRY(ArrayBuffer)
ENTRY(Int8Array)
ENTRY(Int16Array)
ENTRY(Int32Array)
ENTRY(Uint8Array)
ENTRY(Uint8ClampedArray)
ENTRY(Uint16Array)
ENTRY(Uint32Array)
ENTRY(Float32Array)
ENTRY(Float64Array)
ENTRY(CharArray)
ENTRY(Int64Array)
ENTRY(Uint64Array)
ENTRY(BooleanArray)
ENTRY(DataView)
ENTRY(setInt8)
ENTRY(setUint8)
ENTRY(setInt16)
ENTRY(setUint16)
ENTRY(setInt32)
ENTRY(setUint32)
ENTRY(setFloat32)
ENTRY(setFloat64)
ENTRY(getInt8)
ENTRY(getUint8)
ENTRY(getInt16)
ENTRY(getUint16)
ENTRY(getInt32)
ENTRY(getUint32)
ENTRY(getFloat32)
ENTRY(getFloat64)
ENTRY(subarray)
ENTRY(Debug)
ENTRY(caller)
ENTRY(callee)
ENTRY(arguments)
ENTRY($_)
ENTRY($1)
ENTRY($2)
ENTRY($3)
ENTRY($4)
ENTRY($5)
ENTRY($6)
ENTRY($7)
ENTRY($8)
ENTRY($9)
ENTRY(index)
ENTRY(input)
ENTRY(lastMatch)
ENTRY(lastParen)
ENTRY(leftContext)
ENTRY(rightContext)
ENTRY(options)
ENTRY2(__instance, _u("$__instance"))
ENTRY2(Small, _u("small"))
ENTRY2($Ampersand, _u("$&"))
ENTRY2($Plus, _u("$+"))
ENTRY2($BackTick, _u("$`"))
ENTRY2($Tick, _u("$\'"))
ENTRY(__defineGetter__)
ENTRY(__defineSetter__)
ENTRY(__lookupGetter__)
ENTRY(__lookupSetter__)
ENTRY(__proto__)

// Intl
#ifdef INTL_ENTRY
#undef INTL_ENTRY
#endif
#define INTL_ENTRY(id, func) ENTRY(id)
#include "../Library/IntlExtensionObjectBuiltIns.h"
#undef INTL_ENTRY
ENTRY(Common)
ENTRY(__initializedIntlObject)
ENTRY(__locale)
ENTRY(__relevantExtensionKeys)
ENTRY(format)
ENTRY(isValid)
ENTRY(locale)
ENTRY(localeMatcher)
ENTRY(localeWithoutSubtags)
ENTRY(resolvedOptions)
ENTRY(subTags)
ENTRY(supportedLocalesOf)
ENTRY(winglob)
ENTRY(platform)
ENTRY(formatToParts)
ENTRY(co)
ENTRY(kn)
ENTRY(kf)
ENTRY(nu)
ENTRY(ca)
ENTRY(hc)
ENTRY(upper)
ENTRY(lower)
ENTRY(base)
ENTRY(accent)
ENTRY2(case_, _u("case"))
ENTRY(variant)
ENTRY(code)
ENTRY(decimal)
ENTRY(percent)
ENTRY(NumberFormatStyle)
ENTRY(NumberFormatCurrencyDisplay)
ENTRY(CollatorSensitivity)
ENTRY(CollatorCaseFirst)
ENTRY(LocaleDataKind)
ENTRY(DateToLocaleString)
ENTRY(DateToLocaleDateString)
ENTRY(DateToLocaleTimeString)
ENTRY(NumberToLocaleString)
ENTRY(StringLocaleCompare)
ENTRY(BuiltInFunctionID)

// This symbol is not part of the regular Symbol API and is only used in rare circumstances in Intl.js for backwards compatibility
// with the Intl v1 spec. It is visible to the user only using Object.getOwnPropertySymbols(Intl.NumberFormat.call(new Intl.NumberFormat())).
ENTRY(FallbackSymbol)
ENTRY_SYMBOL(_intlFallbackSymbol, _u("Intl.FallbackSymbol"))

ENTRY(NumberFormat)
ENTRY(__currency)
ENTRY(__currencyDisplay)
ENTRY(__currencyDisplayToUse)
ENTRY(__formatterToUse)
ENTRY(__initializedNumberFormat)
ENTRY(__maximumFractionDigits)
ENTRY(__maximumSignificantDigits)
ENTRY(__minimumFractionDigits)
ENTRY(__minimumIntegerDigits)
ENTRY(__minimumSignificantDigits)
ENTRY(__numberingSystem)
ENTRY(__templateString)
ENTRY(__useGrouping)
ENTRY(currency)
ENTRY(currencyDisplay)
ENTRY(currencyDisplayToUse)
ENTRY(formatterToUse)
ENTRY(maximumFractionDigits)
ENTRY(maximumSignificantDigits)
ENTRY(minimumFractionDigits)
ENTRY(minimumIntegerDigits)
ENTRY(minimumSignificantDigits)
ENTRY(numberingSystem)
ENTRY(useGrouping)

ENTRY(Collator)
ENTRY(__boundCompare)
ENTRY(__collation)
ENTRY(__initializedCollator)
ENTRY(compare)
ENTRY(ignorePunctuation)
ENTRY(numeric)
ENTRY(sensitivity)
ENTRY(sensitivityEnum)
ENTRY(caseFirstEnum)
ENTRY(usage)

ENTRY(DateTimeFormat)
ENTRY(__boundFormat)
ENTRY(__calendar)
ENTRY(__day)
ENTRY(__era)
ENTRY(__formatMatcher)
ENTRY(__hour)
ENTRY(__hour12)
ENTRY(__initializedDateTimeFormat)
ENTRY(__matcher)
ENTRY(__minute)
ENTRY(__month)
ENTRY(__patternStrings)
ENTRY(__second)
ENTRY(__style)
ENTRY(__timeZone)
ENTRY(__timeZoneName)
ENTRY(__weekday)
ENTRY(__windowsCalendar)
ENTRY(__windowsClock)
ENTRY(__year)
ENTRY(calendar)
ENTRY(day)
ENTRY(era)
ENTRY(hour)
ENTRY(hour12)
ENTRY(minute)
ENTRY(month)
ENTRY(pattern)
ENTRY(second)
ENTRY(timeZone)
ENTRY(timeZoneName)
ENTRY(weekday)
ENTRY(year)

ENTRY(PluralRules)
ENTRY(select)

// JsBuiltIns
ENTRY(__chakraLibrary)
ENTRY(GetIteratorPrototype)
ENTRY(GetLength)
ENTRY(InitInternalProperties)
ENTRY(registerChakraLibraryFunction)
ENTRY(registerFunction)
ENTRY(toLength)
ENTRY(toInteger)
ENTRY(arraySpeciesCreate)
ENTRY(arrayCreateDataPropertyOrThrow)

// EngineInterfaceObject built-ins
ENTRY(builtInArray_prototype_filter)
ENTRY(builtInArray_prototype_forEach)
ENTRY(builtInArray_prototype_indexOf)
ENTRY(builtInArray_prototype_reduce)
ENTRY(builtInMath_object_min)
ENTRY(builtInMath_object_max)
ENTRY(EngineInterface)
ENTRY(builtInCallInstanceFunction)

#define GlobalMathBuiltIn(mathMethod) ENTRY(builtInMath##mathMethod)
#define GlobalBuiltIn(global, method) ENTRY(builtIn##global##Entry##method)
#define BuiltInRaiseException(exceptionType, exceptionID) ENTRY(raise##exceptionID)
#define EngineInterfaceBuiltIn2(propId, nativeMethod) ENTRY(propId)
#include "../Library/EngineInterfaceObjectBuiltIns.h"

// SymbolFunctionNameId: for RuntimeFunction nameId

ENTRY2(_RuntimeFunctionNameId_asyncIterator, _u("[Symbol.asyncIterator]"))
ENTRY2(_RuntimeFunctionNameId_hasInstance, _u("[Symbol.hasInstance]"))
ENTRY2(_RuntimeFunctionNameId_toStringTag, _u("[Symbol.toStringTag]"))
ENTRY2(_RuntimeFunctionNameId_toPrimitive, _u("[Symbol.toPrimitive]"))
ENTRY2(_RuntimeFunctionNameId_iterator, _u("[Symbol.iterator]"))
ENTRY2(_RuntimeFunctionNameId_species, _u("[Symbol.species]"))
ENTRY2(_RuntimeFunctionNameId_replace, _u("[Symbol.replace]"))
ENTRY2(_RuntimeFunctionNameId_search, _u("[Symbol.search]"))
ENTRY2(_RuntimeFunctionNameId_match, _u("[Symbol.match]"))
ENTRY2(_RuntimeFunctionNameId_split, _u("[Symbol.split]"))

ENTRY(SharedArrayBuffer)
ENTRY(Atomics)
ENTRY(compareExchange)
ENTRY(exchange)
ENTRY(isLockFree)
ENTRY(wait)
ENTRY(notify)

ENTRY(column)
ENTRY(url)
ENTRY(exception)

ENTRY2(_this, _u("*this*"))
ENTRY2(_super, _u("*super*"))
ENTRY2(_superConstructor, _u("*superconstructor*"))
ENTRY2(_newTarget, _u("*new.target*"))


// Note: Do not add fields for conditionally-compiled PropertyIds into this file.
//       The bytecode for internal javascript libraries is built on chk but re-used in fre builds.
//       Having a mismatch in the number of PropertyIds will cause a failure loading bytecode.
//       See BLUE #467459

#undef ENTRY_INTERNAL_SYMBOL
#undef ENTRY_SYMBOL
#undef ENTRY
#undef ENTRY2
