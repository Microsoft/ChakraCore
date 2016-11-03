(module
  (func (export "i_func-i32") (param i32))
  (func (export "i_func-i64") (param i64))
  (func (export "i_func-f32") (param f32))
  (func (export "i_func-f64") (param f64))
  (func (export "i_func->i32") (result i32) (i32.const 22))
  (func (export "i_func->i64") (result i64) (i64.const 87))
  (func (export "i_func->f32") (result f32) (f32.const 11))
  (func (export "i_func->f64") (result f64) (f64.const 1.48))
  (func (export "i_func-i32->i32") (param i32) (result i32) (get_local 0))
  (func (export "i_func-i64->i64") (param i64) (result i64) (get_local 0))
  (func (export "i_func-f32->f32") (param f32) (result f32) (get_local 0))
  (func (export "i_func-f64->f64") (param f64) (result f64) (get_local 0))
)
(register "test")
(assert_return (invoke "i_func->i32") (i32.const 22))
(assert_return (invoke "i_func->i64") (i64.const 87))
(assert_return (invoke "i_func->f32") (f32.const 11))
(assert_return (invoke "i_func->f64") (f64.const 1.48))
(assert_return (invoke "i_func-i32->i32" (i32.const 22)) (i32.const 22))
(assert_return (invoke "i_func-i64->i64" (i64.const 87)) (i64.const 87))
(assert_return (invoke "i_func-f32->f32" (f32.const 11)) (f32.const 11))
(assert_return (invoke "i_func-f64->f64" (f64.const 1.48)) (f64.const 1.48))


(module
  (import "test" "i_func-i32"      (func $i_func-i32 (param i32)))
  (import "test" "i_func-i64"      (func $i_func-i64 (param i64)))
  (import "test" "i_func-f32"      (func $i_func-f32 (param f32)))
  (import "test" "i_func-f64"      (func $i_func-f64 (param f64)))
  (import "test" "i_func->i32"     (func $i_func->i32 (result i32)))
  (import "test" "i_func->i64"     (func $i_func->i64 (result i64)))
  (import "test" "i_func->f32"     (func $i_func->f32 (result f32)))
  (import "test" "i_func->f64"     (func $i_func->f64 (result f64)))
  (import "test" "i_func-i32->i32" (func $i_func-i32->i32 (param i32) (result i32)))
  (import "test" "i_func-i64->i64" (func $i_func-i64->i64 (param i64) (result i64)))
  (import "test" "i_func-f32->f32" (func $i_func-f32->f32 (param f32) (result f32)))
  (import "test" "i_func-f64->f64" (func $i_func-f64->f64 (param f64) (result f64)))
  (import "spectest" "print" (func $print_i32 (param i32) (result i32)))
  (import "spectest" "print" (func $print_i64 (param i64) (result i64)))
  (import "spectest" "print" (func $print_f32 (param f32) (result f32)))
  (import "spectest" "print" (func $print_f64 (param f64) (result f64)))

  (export "func-i32"      (func $i_func-i32))
  (export "func-i64"      (func $i_func-i64))
  (export "func-f32"      (func $i_func-f32))
  (export "func-f64"      (func $i_func-f64))
  (export "func->i32"     (func $i_func->i32))
  (export "func->i64"     (func $i_func->i64))
  (export "func->f32"     (func $i_func->f32))
  (export "func->f64"     (func $i_func->f64))
  (export "func-i32->i32" (func $i_func-i32->i32))
  (export "func-i64->i64" (func $i_func-i64->i64))
  (export "func-f32->f32" (func $i_func-f32->f32))
  (export "func-f64->f64" (func $i_func-f64->f64))
  (export "print_i32" (func $print_i32))
  (export "print_i64" (func $print_i64))
  (export "print_f32" (func $print_f32))
  (export "print_f64" (func $print_f64))

  (func (export "i32") (param i32) (result i32)
    (call $i_func-i32 (get_local 0))
    (call $print_i32 (call $i_func->i32))
    (call $print_i32 (call $i_func-i32->i32 (get_local 0)))
    (return
      (i32.add
        (call $i_func->i32)
        (call $i_func-i32->i32 (get_local 0))
      )
    )
  )
  (func (export "i64") (param i64) (result i64)
    (call $i_func-i64 (get_local 0))
    (return
      (i64.add
        (call $i_func->i64)
        (call $i_func-i64->i64 (get_local 0))
      )
    )
  )
  (func (export "f32") (param f32) (result f32)
    (call $i_func-f32 (get_local 0))
    (call $print_f32 (call $i_func->f32))
    (call $print_f32 (call $i_func-f32->f32 (get_local 0)))
    (return
      (f32.add
        (call $i_func->f32)
        (call $i_func-f32->f32 (get_local 0))
      )
    )
  )
  (func (export "f64") (param f64) (result f64)
    (call $i_func-f64 (get_local 0))
    (call $print_f64 (call $i_func->f64))
    (call $print_f64 (call $i_func-f64->f64 (get_local 0)))
    (return
      (f64.add
        (call $i_func->f64)
        (call $i_func-f64->f64 (get_local 0))
      )
    )
  )
)


(assert_return (invoke "print_i32" (i32.const 54876)) (i32.const 0))
(assert_trap (invoke "print_i64" (i64.const 70368992927654)) "TypeError")
(assert_return_nan (invoke "print_f32" (f32.const 3.14)))
(assert_return_nan (invoke "print_f64" (f64.const 5.14)))
(invoke "func-i32" (i32.const 0))
(invoke "func-i64" (i64.const 0))
(invoke "func-f32" (f32.const 0))
(invoke "func-f64" (f64.const 0))
(assert_return (invoke "func->i32") (i32.const 22))
(assert_return (invoke "func->i64") (i64.const 87))
(assert_return (invoke "func->f32") (f32.const 11))
(assert_return (invoke "func->f64") (f64.const 1.48))
(assert_return (invoke "func-i32->i32" (i32.const -1)) (i32.const -1))
(assert_return (invoke "func-i64->i64" (i64.const 70368992927654)) (i64.const 70368992927654))
(assert_return (invoke "func-f32->f32" (f32.const -4.5)) (f32.const -4.5))
(assert_return (invoke "func-f64->f64" (f64.const 8.4)) (f64.const 8.4))

(assert_return (invoke "i32" (i32.const 5)) (i32.const 27)) ;; 5 + 22
(assert_return (invoke "i64" (i64.const 70368992927654)) (i64.const 70368992927741)) ;; 70368992927654 + 87
(assert_return (invoke "f32" (f32.const 5.14)) (f32.const 16.14)) ;; 5.14 + 11
(assert_return (invoke "f64" (f64.const 3.14)) (f64.const 4.62)) ;; 3.14 + 1.48
