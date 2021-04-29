//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

R"====(
var $262 = {
  createRealm: function () {
    return WScript.LoadScript('', 'samethread').$262;
  },
  global: this,
  agent: {
    start: function (src) {
      WScript.LoadScript(
        `
        $262 = {
          agent:{
            receiveBroadcast: function(callback){ WScript.ReceiveBroadcast(callback); },
            report: function(value){ WScript.Report(value); },
            leaving: function(){ WScript.Leaving(); },
            monotonicNow: () => WScript.monotonicNow()
          }
        };
        ${src}
        `, 'crossthread');
    },
    broadcast: function (sab) { WScript.Broadcast(sab); },
    sleep: function (timeout) { WScript.Sleep(timeout); },
    getReport: function () { return WScript.GetReport(); },
    monotonicNow: () => WScript.monotonicNow()
  },
};
)===="
