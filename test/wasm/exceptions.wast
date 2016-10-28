;;-------------------------------------------------------------------------------------------------------
;; Copyright (C) Microsoft Corporation and contributors. All rights reserved.
;; Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
;;-------------------------------------------------------------------------------------------------------

(module
  (func (export "i32_div_s") (param $x i32) (param $y i32) (result i32)
    (i32.div_s (get_local $x) (get_local $y)))
  (func (export "i32_div_u") (param $x i32) (param $y i32) (result i32)
    (i32.div_u (get_local $x) (get_local $y)))
    
  (func (export "i32_rem_s") (param $x i32) (param $y i32) (result i32)
    (i32.rem_s (get_local $x) (get_local $y)))
  (func (export "i32_rem_u") (param $x i32) (param $y i32) (result i32)
    (i32.rem_u (get_local $x) (get_local $y)))
)

