//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

const blob = WScript.LoadBinaryFile('basic.wasm');
print(blob.byteLength)
var a = Wasm.instantiateModule(blob, {});
print(a.a(11));
