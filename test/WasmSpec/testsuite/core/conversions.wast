(module
  (func (export "i64.extend_s_i32") (param $x i32) (result i64) (i64.extend_s/i32 (get_local $x)))
  (func (export "i64.extend_u_i32") (param $x i32) (result i64) (i64.extend_u/i32 (get_local $x)))
  (func (export "i32.wrap_i64") (param $x i64) (result i32) (i32.wrap/i64 (get_local $x)))
  (func (export "i32.trunc_s_f32") (param $x f32) (result i32) (i32.trunc_s/f32 (get_local $x)))
  (func (export "i32.trunc_u_f32") (param $x f32) (result i32) (i32.trunc_u/f32 (get_local $x)))
  (func (export "i32.trunc_s_f64") (param $x f64) (result i32) (i32.trunc_s/f64 (get_local $x)))
  (func (export "i32.trunc_u_f64") (param $x f64) (result i32) (i32.trunc_u/f64 (get_local $x)))
  (func (export "i64.trunc_s_f32") (param $x f32) (result i64) (i64.trunc_s/f32 (get_local $x)))
  (func (export "i64.trunc_u_f32") (param $x f32) (result i64) (i64.trunc_u/f32 (get_local $x)))
  (func (export "i64.trunc_s_f64") (param $x f64) (result i64) (i64.trunc_s/f64 (get_local $x)))
  (func (export "i64.trunc_u_f64") (param $x f64) (result i64) (i64.trunc_u/f64 (get_local $x)))
  (func (export "f32.convert_s_i32") (param $x i32) (result f32) (f32.convert_s/i32 (get_local $x)))
  (func (export "f32.convert_s_i64") (param $x i64) (result f32) (f32.convert_s/i64 (get_local $x)))
  (func (export "f64.convert_s_i32") (param $x i32) (result f64) (f64.convert_s/i32 (get_local $x)))
  (func (export "f64.convert_s_i64") (param $x i64) (result f64) (f64.convert_s/i64 (get_local $x)))
  (func (export "f32.convert_u_i32") (param $x i32) (result f32) (f32.convert_u/i32 (get_local $x)))
  (func (export "f32.convert_u_i64") (param $x i64) (result f32) (f32.convert_u/i64 (get_local $x)))
  (func (export "f64.convert_u_i32") (param $x i32) (result f64) (f64.convert_u/i32 (get_local $x)))
  (func (export "f64.convert_u_i64") (param $x i64) (result f64) (f64.convert_u/i64 (get_local $x)))
  (func (export "f64.promote_f32") (param $x f32) (result f64) (f64.promote/f32 (get_local $x)))
  (func (export "f32.demote_f64") (param $x f64) (result f32) (f32.demote/f64 (get_local $x)))
  (func (export "f32.reinterpret_i32") (param $x i32) (result f32) (f32.reinterpret/i32 (get_local $x)))
  (func (export "f64.reinterpret_i64") (param $x i64) (result f64) (f64.reinterpret/i64 (get_local $x)))
  (func (export "i32.reinterpret_f32") (param $x f32) (result i32) (i32.reinterpret/f32 (get_local $x)))
  (func (export "i64.reinterpret_f64") (param $x f64) (result i64) (i64.reinterpret/f64 (get_local $x)))
)

(assert_return (invoke "i64.extend_s_i32" (i32.const 0)) (i64.const 0))
(assert_return (invoke "i64.extend_s_i32" (i32.const 10000)) (i64.const 10000))
(assert_return (invoke "i64.extend_s_i32" (i32.const -10000)) (i64.const -10000))
(assert_return (invoke "i64.extend_s_i32" (i32.const -1)) (i64.const -1))
(assert_return (invoke "i64.extend_s_i32" (i32.const 0x7fffffff)) (i64.const 0x000000007fffffff))
(assert_return (invoke "i64.extend_s_i32" (i32.const 0x80000000)) (i64.const 0xffffffff80000000))

(assert_return (invoke "i64.extend_u_i32" (i32.const 0)) (i64.const 0))
(assert_return (invoke "i64.extend_u_i32" (i32.const 10000)) (i64.const 10000))
(assert_return (invoke "i64.extend_u_i32" (i32.const -10000)) (i64.const 0x00000000ffffd8f0))
(assert_return (invoke "i64.extend_u_i32" (i32.const -1)) (i64.const 0xffffffff))
(assert_return (invoke "i64.extend_u_i32" (i32.const 0x7fffffff)) (i64.const 0x000000007fffffff))
(assert_return (invoke "i64.extend_u_i32" (i32.const 0x80000000)) (i64.const 0x0000000080000000))

(assert_return (invoke "i32.wrap_i64" (i64.const -1)) (i32.const -1))
(assert_return (invoke "i32.wrap_i64" (i64.const -100000)) (i32.const -100000))
(assert_return (invoke "i32.wrap_i64" (i64.const 0x80000000)) (i32.const 0x80000000))
(assert_return (invoke "i32.wrap_i64" (i64.const 0xffffffff7fffffff)) (i32.const 0x7fffffff))
(assert_return (invoke "i32.wrap_i64" (i64.const 0xffffffff00000000)) (i32.const 0x00000000))
(assert_return (invoke "i32.wrap_i64" (i64.const 0xfffffffeffffffff)) (i32.const 0xffffffff))
(assert_return (invoke "i32.wrap_i64" (i64.const 0xffffffff00000001)) (i32.const 0x00000001))
(assert_return (invoke "i32.wrap_i64" (i64.const 0)) (i32.const 0))
(assert_return (invoke "i32.wrap_i64" (i64.const 1311768467463790320)) (i32.const 0x9abcdef0))
(assert_return (invoke "i32.wrap_i64" (i64.const 0x00000000ffffffff)) (i32.const 0xffffffff))
(assert_return (invoke "i32.wrap_i64" (i64.const 0x0000000100000000)) (i32.const 0x00000000))
(assert_return (invoke "i32.wrap_i64" (i64.const 0x0000000100000001)) (i32.const 0x00000001))

(assert_return (invoke "i32.trunc_s_f32" (f32.const 0.0)) (i32.const 0))
(assert_return (invoke "i32.trunc_s_f32" (f32.const -0.0)) (i32.const 0))
(assert_return (invoke "i32.trunc_s_f32" (f32.const 0x1p-149)) (i32.const 0))
(assert_return (invoke "i32.trunc_s_f32" (f32.const -0x1p-149)) (i32.const 0))
(assert_return (invoke "i32.trunc_s_f32" (f32.const 1.0)) (i32.const 1))
(assert_return (invoke "i32.trunc_s_f32" (f32.const 0x1.19999ap+0)) (i32.const 1))
(assert_return (invoke "i32.trunc_s_f32" (f32.const 1.5)) (i32.const 1))
(assert_return (invoke "i32.trunc_s_f32" (f32.const -1.0)) (i32.const -1))
(assert_return (invoke "i32.trunc_s_f32" (f32.const -0x1.19999ap+0)) (i32.const -1))
(assert_return (invoke "i32.trunc_s_f32" (f32.const -1.5)) (i32.const -1))
(assert_return (invoke "i32.trunc_s_f32" (f32.const -1.9)) (i32.const -1))
(assert_return (invoke "i32.trunc_s_f32" (f32.const -2.0)) (i32.const -2))
(assert_return (invoke "i32.trunc_s_f32" (f32.const 2147483520.0)) (i32.const 2147483520))
(assert_return (invoke "i32.trunc_s_f32" (f32.const -2147483648.0)) (i32.const -2147483648))
(assert_trap (invoke "i32.trunc_s_f32" (f32.const 2147483648.0)) "integer overflow")
(assert_trap (invoke "i32.trunc_s_f32" (f32.const -2147483904.0)) "integer overflow")
(assert_trap (invoke "i32.trunc_s_f32" (f32.const infinity)) "integer overflow")
(assert_trap (invoke "i32.trunc_s_f32" (f32.const -infinity)) "integer overflow")
(assert_trap (invoke "i32.trunc_s_f32" (f32.const nan)) "invalid conversion to integer")

(assert_return (invoke "i32.trunc_u_f32" (f32.const 0.0)) (i32.const 0))
(assert_return (invoke "i32.trunc_u_f32" (f32.const -0.0)) (i32.const 0))
(assert_return (invoke "i32.trunc_u_f32" (f32.const 0x1p-149)) (i32.const 0))
(assert_return (invoke "i32.trunc_u_f32" (f32.const -0x1p-149)) (i32.const 0))
(assert_return (invoke "i32.trunc_u_f32" (f32.const 1.0)) (i32.const 1))
(assert_return (invoke "i32.trunc_u_f32" (f32.const 0x1.19999ap+0)) (i32.const 1))
(assert_return (invoke "i32.trunc_u_f32" (f32.const 1.5)) (i32.const 1))
(assert_return (invoke "i32.trunc_u_f32" (f32.const 1.9)) (i32.const 1))
(assert_return (invoke "i32.trunc_u_f32" (f32.const 2.0)) (i32.const 2))
(assert_return (invoke "i32.trunc_u_f32" (f32.const 2147483648)) (i32.const -2147483648)) ;; 0x1.00000p+31 -> 8000 0000
(assert_return (invoke "i32.trunc_u_f32" (f32.const 4294967040.0)) (i32.const -256))
(assert_return (invoke "i32.trunc_u_f32" (f32.const -0x1.ccccccp-1)) (i32.const 0))
(assert_return (invoke "i32.trunc_u_f32" (f32.const -0x1.fffffep-1)) (i32.const 0))
(assert_trap (invoke "i32.trunc_u_f32" (f32.const 4294967296.0)) "integer overflow")
(assert_trap (invoke "i32.trunc_u_f32" (f32.const -1.0)) "integer overflow")
(assert_trap (invoke "i32.trunc_u_f32" (f32.const infinity)) "integer overflow")
(assert_trap (invoke "i32.trunc_u_f32" (f32.const -infinity)) "integer overflow")
(assert_trap (invoke "i32.trunc_u_f32" (f32.const nan)) "invalid conversion to integer")

(assert_return (invoke "i32.trunc_s_f64" (f64.const 0.0)) (i32.const 0))
(assert_return (invoke "i32.trunc_s_f64" (f64.const -0.0)) (i32.const 0))
(assert_return (invoke "i32.trunc_s_f64" (f64.const 0x0.0000000000001p-1022)) (i32.const 0))
(assert_return (invoke "i32.trunc_s_f64" (f64.const -0x0.0000000000001p-1022)) (i32.const 0))
(assert_return (invoke "i32.trunc_s_f64" (f64.const 1.0)) (i32.const 1))
(assert_return (invoke "i32.trunc_s_f64" (f64.const 0x1.199999999999ap+0)) (i32.const 1))
(assert_return (invoke "i32.trunc_s_f64" (f64.const 1.5)) (i32.const 1))
(assert_return (invoke "i32.trunc_s_f64" (f64.const -1.0)) (i32.const -1))
(assert_return (invoke "i32.trunc_s_f64" (f64.const -0x1.199999999999ap+0)) (i32.const -1))
(assert_return (invoke "i32.trunc_s_f64" (f64.const -1.5)) (i32.const -1))
(assert_return (invoke "i32.trunc_s_f64" (f64.const -1.9)) (i32.const -1))
(assert_return (invoke "i32.trunc_s_f64" (f64.const -2.0)) (i32.const -2))
(assert_return (invoke "i32.trunc_s_f64" (f64.const 2147483647.0)) (i32.const 2147483647))
(assert_return (invoke "i32.trunc_s_f64" (f64.const -2147483648.0)) (i32.const -2147483648))
(assert_trap (invoke "i32.trunc_s_f64" (f64.const 2147483648.0)) "integer overflow")
(assert_trap (invoke "i32.trunc_s_f64" (f64.const -2147483649.0)) "integer overflow")
(assert_trap (invoke "i32.trunc_s_f64" (f64.const infinity)) "integer overflow")
(assert_trap (invoke "i32.trunc_s_f64" (f64.const -infinity)) "integer overflow")
(assert_trap (invoke "i32.trunc_s_f64" (f64.const nan)) "invalid conversion to integer")

(assert_return (invoke "i32.trunc_u_f64" (f64.const 0.0)) (i32.const 0))
(assert_return (invoke "i32.trunc_u_f64" (f64.const -0.0)) (i32.const 0))
(assert_return (invoke "i32.trunc_u_f64" (f64.const 0x0.0000000000001p-1022)) (i32.const 0))
(assert_return (invoke "i32.trunc_u_f64" (f64.const -0x0.0000000000001p-1022)) (i32.const 0))
(assert_return (invoke "i32.trunc_u_f64" (f64.const 1.0)) (i32.const 1))
(assert_return (invoke "i32.trunc_u_f64" (f64.const 0x1.199999999999ap+0)) (i32.const 1))
(assert_return (invoke "i32.trunc_u_f64" (f64.const 1.5)) (i32.const 1))
(assert_return (invoke "i32.trunc_u_f64" (f64.const 1.9)) (i32.const 1))
(assert_return (invoke "i32.trunc_u_f64" (f64.const 2.0)) (i32.const 2))
(assert_return (invoke "i32.trunc_u_f64" (f64.const 2147483648)) (i32.const -2147483648)) ;; 0x1.00000p+31 -> 8000 0000
(assert_return (invoke "i32.trunc_u_f64" (f64.const 4294967295.0)) (i32.const -1))
(assert_return (invoke "i32.trunc_u_f64" (f64.const -0x1.ccccccccccccdp-1)) (i32.const 0))
(assert_return (invoke "i32.trunc_u_f64" (f64.const -0x1.fffffffffffffp-1)) (i32.const 0))
(assert_return (invoke "i32.trunc_u_f64" (f64.const 1e8)) (i32.const 100000000))
(assert_trap (invoke "i32.trunc_u_f64" (f64.const 4294967296.0)) "integer overflow")
(assert_trap (invoke "i32.trunc_u_f64" (f64.const -1.0)) "integer overflow")
(assert_trap (invoke "i32.trunc_u_f64" (f64.const 1e16)) "integer overflow")
(assert_trap (invoke "i32.trunc_u_f64" (f64.const 1e30)) "integer overflow")
(assert_trap (invoke "i32.trunc_u_f64" (f64.const 9223372036854775808)) "integer overflow")
(assert_trap (invoke "i32.trunc_u_f64" (f64.const infinity)) "integer overflow")
(assert_trap (invoke "i32.trunc_u_f64" (f64.const -infinity)) "integer overflow")
(assert_trap (invoke "i32.trunc_u_f64" (f64.const nan)) "invalid conversion to integer")

(assert_return (invoke "i64.trunc_s_f32" (f32.const 0.0)) (i64.const 0))
(assert_return (invoke "i64.trunc_s_f32" (f32.const -0.0)) (i64.const 0))
(assert_return (invoke "i64.trunc_s_f32" (f32.const 0x1p-149)) (i64.const 0))
(assert_return (invoke "i64.trunc_s_f32" (f32.const -0x1p-149)) (i64.const 0))
(assert_return (invoke "i64.trunc_s_f32" (f32.const 1.0)) (i64.const 1))
(assert_return (invoke "i64.trunc_s_f32" (f32.const 0x1.19999ap+0)) (i64.const 1))
(assert_return (invoke "i64.trunc_s_f32" (f32.const 1.5)) (i64.const 1))
(assert_return (invoke "i64.trunc_s_f32" (f32.const -1.0)) (i64.const -1))
(assert_return (invoke "i64.trunc_s_f32" (f32.const -0x1.19999ap+0)) (i64.const -1))
(assert_return (invoke "i64.trunc_s_f32" (f32.const -1.5)) (i64.const -1))
(assert_return (invoke "i64.trunc_s_f32" (f32.const -1.9)) (i64.const -1))
(assert_return (invoke "i64.trunc_s_f32" (f32.const -2.0)) (i64.const -2))
(assert_return (invoke "i64.trunc_s_f32" (f32.const 4294967296)) (i64.const 4294967296)) ;; 0x1.00000p+32 -> 1 0000 0000
(assert_return (invoke "i64.trunc_s_f32" (f32.const -4294967296)) (i64.const -4294967296)) ;; -0x1.00000p+32 -> ffff ffff 0000 0000
(assert_return (invoke "i64.trunc_s_f32" (f32.const 9223371487098961920.0)) (i64.const 9223371487098961920))
(assert_return (invoke "i64.trunc_s_f32" (f32.const -9223372036854775808.0)) (i64.const -9223372036854775808))
(assert_trap (invoke "i64.trunc_s_f32" (f32.const 9223372036854775808.0)) "integer overflow")
(assert_trap (invoke "i64.trunc_s_f32" (f32.const -9223373136366403584.0)) "integer overflow")
(assert_trap (invoke "i64.trunc_s_f32" (f32.const infinity)) "integer overflow")
(assert_trap (invoke "i64.trunc_s_f32" (f32.const -infinity)) "integer overflow")
(assert_trap (invoke "i64.trunc_s_f32" (f32.const nan)) "invalid conversion to integer")

(assert_return (invoke "i64.trunc_u_f32" (f32.const 0.0)) (i64.const 0))
(assert_return (invoke "i64.trunc_u_f32" (f32.const -0.0)) (i64.const 0))
(assert_return (invoke "i64.trunc_u_f32" (f32.const 0x1p-149)) (i64.const 0))
(assert_return (invoke "i64.trunc_u_f32" (f32.const -0x1p-149)) (i64.const 0))
(assert_return (invoke "i64.trunc_u_f32" (f32.const 1.0)) (i64.const 1))
(assert_return (invoke "i64.trunc_u_f32" (f32.const 0x1.19999ap+0)) (i64.const 1))
(assert_return (invoke "i64.trunc_u_f32" (f32.const 1.5)) (i64.const 1))
(assert_return (invoke "i64.trunc_u_f32" (f32.const 4294967296)) (i64.const 4294967296))
(assert_return (invoke "i64.trunc_u_f32" (f32.const 18446742974197923840.0)) (i64.const -1099511627776))
(assert_return (invoke "i64.trunc_u_f32" (f32.const -0x1.ccccccp-1)) (i64.const 0))
(assert_return (invoke "i64.trunc_u_f32" (f32.const -0x1.fffffep-1)) (i64.const 0))
(assert_trap (invoke "i64.trunc_u_f32" (f32.const 18446744073709551616.0)) "integer overflow")
(assert_trap (invoke "i64.trunc_u_f32" (f32.const -1.0)) "integer overflow")
(assert_trap (invoke "i64.trunc_u_f32" (f32.const infinity)) "integer overflow")
(assert_trap (invoke "i64.trunc_u_f32" (f32.const -infinity)) "integer overflow")
(assert_trap (invoke "i64.trunc_u_f32" (f32.const nan)) "invalid conversion to integer")

(assert_return (invoke "i64.trunc_s_f64" (f64.const 0.0)) (i64.const 0))
(assert_return (invoke "i64.trunc_s_f64" (f64.const -0.0)) (i64.const 0))
(assert_return (invoke "i64.trunc_s_f64" (f64.const 0x0.0000000000001p-1022)) (i64.const 0))
(assert_return (invoke "i64.trunc_s_f64" (f64.const -0x0.0000000000001p-1022)) (i64.const 0))
(assert_return (invoke "i64.trunc_s_f64" (f64.const 1.0)) (i64.const 1))
(assert_return (invoke "i64.trunc_s_f64" (f64.const 0x1.199999999999ap+0)) (i64.const 1))
(assert_return (invoke "i64.trunc_s_f64" (f64.const 1.5)) (i64.const 1))
(assert_return (invoke "i64.trunc_s_f64" (f64.const -1.0)) (i64.const -1))
(assert_return (invoke "i64.trunc_s_f64" (f64.const -0x1.199999999999ap+0)) (i64.const -1))
(assert_return (invoke "i64.trunc_s_f64" (f64.const -1.5)) (i64.const -1))
(assert_return (invoke "i64.trunc_s_f64" (f64.const -1.9)) (i64.const -1))
(assert_return (invoke "i64.trunc_s_f64" (f64.const -2.0)) (i64.const -2))
(assert_return (invoke "i64.trunc_s_f64" (f64.const 4294967296)) (i64.const 4294967296)) ;; 0x1.00000p+32 -> 1 0000 0000
(assert_return (invoke "i64.trunc_s_f64" (f64.const -4294967296)) (i64.const -4294967296)) ;; -0x1.00000p+32 -> ffff ffff 0000 0000
(assert_return (invoke "i64.trunc_s_f64" (f64.const 9223372036854774784.0)) (i64.const 9223372036854774784))
(assert_return (invoke "i64.trunc_s_f64" (f64.const -9223372036854775808.0)) (i64.const -9223372036854775808))
(assert_trap (invoke "i64.trunc_s_f64" (f64.const 9223372036854775808.0)) "integer overflow")
(assert_trap (invoke "i64.trunc_s_f64" (f64.const -9223372036854777856.0)) "integer overflow")
(assert_trap (invoke "i64.trunc_s_f64" (f64.const infinity)) "integer overflow")
(assert_trap (invoke "i64.trunc_s_f64" (f64.const -infinity)) "integer overflow")
(assert_trap (invoke "i64.trunc_s_f64" (f64.const nan)) "invalid conversion to integer")

(assert_return (invoke "i64.trunc_u_f64" (f64.const 0.0)) (i64.const 0))
(assert_return (invoke "i64.trunc_u_f64" (f64.const -0.0)) (i64.const 0))
(assert_return (invoke "i64.trunc_u_f64" (f64.const 0x0.0000000000001p-1022)) (i64.const 0))
(assert_return (invoke "i64.trunc_u_f64" (f64.const -0x0.0000000000001p-1022)) (i64.const 0))
(assert_return (invoke "i64.trunc_u_f64" (f64.const 1.0)) (i64.const 1))
(assert_return (invoke "i64.trunc_u_f64" (f64.const 0x1.199999999999ap+0)) (i64.const 1))
(assert_return (invoke "i64.trunc_u_f64" (f64.const 1.5)) (i64.const 1))
(assert_return (invoke "i64.trunc_u_f64" (f64.const 4294967295)) (i64.const 0xffffffff))
(assert_return (invoke "i64.trunc_u_f64" (f64.const 4294967296)) (i64.const 0x100000000))
(assert_return (invoke "i64.trunc_u_f64" (f64.const 18446744073709549568.0)) (i64.const -2048))
(assert_return (invoke "i64.trunc_u_f64" (f64.const -0x1.ccccccccccccdp-1)) (i64.const 0))
(assert_return (invoke "i64.trunc_u_f64" (f64.const -0x1.fffffffffffffp-1)) (i64.const 0))
(assert_return (invoke "i64.trunc_u_f64" (f64.const 1e8)) (i64.const 100000000))
(assert_return (invoke "i64.trunc_u_f64" (f64.const 1e16)) (i64.const 10000000000000000))
(assert_return (invoke "i64.trunc_u_f64" (f64.const 9223372036854775808)) (i64.const -9223372036854775808))
(assert_trap (invoke "i64.trunc_u_f64" (f64.const 18446744073709551616.0)) "integer overflow")
(assert_trap (invoke "i64.trunc_u_f64" (f64.const -1.0)) "integer overflow")
(assert_trap (invoke "i64.trunc_u_f64" (f64.const infinity)) "integer overflow")
(assert_trap (invoke "i64.trunc_u_f64" (f64.const -infinity)) "integer overflow")
(assert_trap (invoke "i64.trunc_u_f64" (f64.const nan)) "invalid conversion to integer")

(assert_return (invoke "f32.convert_s_i32" (i32.const 1)) (f32.const 1.0))
(assert_return (invoke "f32.convert_s_i32" (i32.const -1)) (f32.const -1.0))
(assert_return (invoke "f32.convert_s_i32" (i32.const 0)) (f32.const 0.0))
(assert_return (invoke "f32.convert_s_i32" (i32.const 2147483647)) (f32.const 2147483648))
(assert_return (invoke "f32.convert_s_i32" (i32.const -2147483648)) (f32.const -2147483648))
(assert_return (invoke "f32.convert_s_i32" (i32.const 1234567890)) (f32.const 0x1.26580cp+30))
;; Test rounding directions.
(assert_return (invoke "f32.convert_s_i32" (i32.const 16777217)) (f32.const 16777216.0))
(assert_return (invoke "f32.convert_s_i32" (i32.const -16777217)) (f32.const -16777216.0))
(assert_return (invoke "f32.convert_s_i32" (i32.const 16777219)) (f32.const 16777220.0))
(assert_return (invoke "f32.convert_s_i32" (i32.const -16777219)) (f32.const -16777220.0))

(assert_return (invoke "f32.convert_s_i64" (i64.const 1)) (f32.const 1.0))
(assert_return (invoke "f32.convert_s_i64" (i64.const -1)) (f32.const -1.0))
(assert_return (invoke "f32.convert_s_i64" (i64.const 0)) (f32.const 0.0))
(assert_return (invoke "f32.convert_s_i64" (i64.const 9223372036854775807)) (f32.const 9223372036854775807))
(assert_return (invoke "f32.convert_s_i64" (i64.const -9223372036854775808)) (f32.const -9223372036854775808))
(assert_return (invoke "f32.convert_s_i64" (i64.const 314159265358979)) (f32.const 0x1.1db9e8p+48)) ;; PI
;; Test rounding directions.
(assert_return (invoke "f32.convert_s_i64" (i64.const 16777217)) (f32.const 16777216.0))
(assert_return (invoke "f32.convert_s_i64" (i64.const -16777217)) (f32.const -16777216.0))
(assert_return (invoke "f32.convert_s_i64" (i64.const 16777219)) (f32.const 16777220.0))
(assert_return (invoke "f32.convert_s_i64" (i64.const -16777219)) (f32.const -16777220.0))

(assert_return (invoke "f64.convert_s_i32" (i32.const 1)) (f64.const 1.0))
(assert_return (invoke "f64.convert_s_i32" (i32.const -1)) (f64.const -1.0))
(assert_return (invoke "f64.convert_s_i32" (i32.const 0)) (f64.const 0.0))
(assert_return (invoke "f64.convert_s_i32" (i32.const 2147483647)) (f64.const 2147483647))
(assert_return (invoke "f64.convert_s_i32" (i32.const -2147483648)) (f64.const -2147483648))
(assert_return (invoke "f64.convert_s_i32" (i32.const 987654321)) (f64.const 987654321))

(assert_return (invoke "f64.convert_s_i64" (i64.const 1)) (f64.const 1.0))
(assert_return (invoke "f64.convert_s_i64" (i64.const -1)) (f64.const -1.0))
(assert_return (invoke "f64.convert_s_i64" (i64.const 0)) (f64.const 0.0))
(assert_return (invoke "f64.convert_s_i64" (i64.const 9223372036854775807)) (f64.const 9223372036854775807))
(assert_return (invoke "f64.convert_s_i64" (i64.const -9223372036854775808)) (f64.const -9223372036854775808))
(assert_return (invoke "f64.convert_s_i64" (i64.const 4669201609102990)) (f64.const 4669201609102990)) ;; Feigenbaum
;; Test rounding directions.
(assert_return (invoke "f64.convert_s_i64" (i64.const 9007199254740993)) (f64.const 9007199254740992))
(assert_return (invoke "f64.convert_s_i64" (i64.const -9007199254740993)) (f64.const -9007199254740992))
(assert_return (invoke "f64.convert_s_i64" (i64.const 9007199254740995)) (f64.const 9007199254740996))
(assert_return (invoke "f64.convert_s_i64" (i64.const -9007199254740995)) (f64.const -9007199254740996))

(assert_return (invoke "f32.convert_u_i32" (i32.const 1)) (f32.const 1.0))
(assert_return (invoke "f32.convert_u_i32" (i32.const 0)) (f32.const 0.0))
(assert_return (invoke "f32.convert_u_i32" (i32.const 2147483647)) (f32.const 2147483648))
(assert_return (invoke "f32.convert_u_i32" (i32.const -2147483648)) (f32.const 2147483648))
(assert_return (invoke "f32.convert_u_i32" (i32.const 0x12345678)) (f32.const 0x1.234568p+28))
(assert_return (invoke "f32.convert_u_i32" (i32.const 0xffffffff)) (f32.const 4294967296.0))
(assert_return (invoke "f32.convert_u_i32" (i32.const 0x80000080)) (f32.const 0x1.000000p+31))
(assert_return (invoke "f32.convert_u_i32" (i32.const 0x80000081)) (f32.const 0x1.000002p+31))
(assert_return (invoke "f32.convert_u_i32" (i32.const 0x80000082)) (f32.const 0x1.000002p+31))
(assert_return (invoke "f32.convert_u_i32" (i32.const 0xfffffe80)) (f32.const 0x1.fffffcp+31))
(assert_return (invoke "f32.convert_u_i32" (i32.const 0xfffffe81)) (f32.const 0x1.fffffep+31))
(assert_return (invoke "f32.convert_u_i32" (i32.const 0xfffffe82)) (f32.const 0x1.fffffep+31))
;; Test rounding directions.
(assert_return (invoke "f32.convert_u_i32" (i32.const 16777217)) (f32.const 16777216.0))
(assert_return (invoke "f32.convert_u_i32" (i32.const 16777219)) (f32.const 16777220.0))

(assert_return (invoke "f32.convert_u_i64" (i64.const 1)) (f32.const 1.0))
(assert_return (invoke "f32.convert_u_i64" (i64.const 0)) (f32.const 0.0))
(assert_return (invoke "f32.convert_u_i64" (i64.const 9223372036854775807)) (f32.const 9223372036854775807))
(assert_return (invoke "f32.convert_u_i64" (i64.const -9223372036854775808)) (f32.const 9223372036854775808))
(assert_return (invoke "f32.convert_u_i64" (i64.const 0xffffffffffffffff)) (f32.const 18446744073709551616.0))
;; Test rounding directions.
(assert_return (invoke "f32.convert_u_i64" (i64.const 16777217)) (f32.const 16777216.0))
(assert_return (invoke "f32.convert_u_i64" (i64.const 16777219)) (f32.const 16777220.0))

(assert_return (invoke "f64.convert_u_i32" (i32.const 1)) (f64.const 1.0))
(assert_return (invoke "f64.convert_u_i32" (i32.const 0)) (f64.const 0.0))
(assert_return (invoke "f64.convert_u_i32" (i32.const 2147483647)) (f64.const 2147483647))
(assert_return (invoke "f64.convert_u_i32" (i32.const -2147483648)) (f64.const 2147483648))
(assert_return (invoke "f64.convert_u_i32" (i32.const 0xffffffff)) (f64.const 4294967295.0))

(assert_return (invoke "f64.convert_u_i64" (i64.const 1)) (f64.const 1.0))
(assert_return (invoke "f64.convert_u_i64" (i64.const 0)) (f64.const 0.0))
(assert_return (invoke "f64.convert_u_i64" (i64.const 9223372036854775807)) (f64.const 9223372036854775807))
(assert_return (invoke "f64.convert_u_i64" (i64.const -9223372036854775808)) (f64.const 9223372036854775808))
(assert_return (invoke "f64.convert_u_i64" (i64.const 0xffffffffffffffff)) (f64.const 18446744073709551616.0))
(assert_return (invoke "f64.convert_u_i64" (i64.const 0x8000000000000400)) (f64.const 0x1.0000000000000p+63))
;;(assert_return (invoke "f64.convert_u_i64" (i64.const 0x8000000000000401)) (f64.const 0x1.0000000000001p+63))
;;(assert_return (invoke "f64.convert_u_i64" (i64.const 0x8000000000000402)) (f64.const 0x1.0000000000001p+63))
(assert_return (invoke "f64.convert_u_i64" (i64.const 0xfffffffffffff400)) (f64.const 0x1.ffffffffffffep+63))
(assert_return (invoke "f64.convert_u_i64" (i64.const 0xfffffffffffff401)) (f64.const 0x1.fffffffffffffp+63))
(assert_return (invoke "f64.convert_u_i64" (i64.const 0xfffffffffffff402)) (f64.const 0x1.fffffffffffffp+63))
;; Test rounding directions.
(assert_return (invoke "f64.convert_u_i64" (i64.const 9007199254740993)) (f64.const 9007199254740992))
(assert_return (invoke "f64.convert_u_i64" (i64.const 9007199254740995)) (f64.const 9007199254740996))

(assert_return (invoke "f64.promote_f32" (f32.const 0.0)) (f64.const 0.0))
(assert_return (invoke "f64.promote_f32" (f32.const -0.0)) (f64.const -0.0))
(assert_return (invoke "f64.promote_f32" (f32.const 0x1p-149)) (f64.const 0x1p-149))
(assert_return (invoke "f64.promote_f32" (f32.const -0x1p-149)) (f64.const -0x1p-149))
(assert_return (invoke "f64.promote_f32" (f32.const 1.0)) (f64.const 1.0))
(assert_return (invoke "f64.promote_f32" (f32.const -1.0)) (f64.const -1.0))
(assert_return (invoke "f64.promote_f32" (f32.const -0x1.fffffep+127)) (f64.const -0x1.fffffep+127))
(assert_return (invoke "f64.promote_f32" (f32.const 0x1.fffffep+127)) (f64.const 0x1.fffffep+127))
;; Generated randomly by picking a random int and reinterpret it to float.
(assert_return (invoke "f64.promote_f32" (f32.const 0x1p-119)) (f64.const 0x1p-119))
;; Generated randomly by picking a random float.
(assert_return (invoke "f64.promote_f32" (f32.const 0x1.8f867ep+125)) (f64.const 6.6382536710104395e+37))
(assert_return (invoke "f64.promote_f32" (f32.const infinity)) (f64.const infinity))
(assert_return (invoke "f64.promote_f32" (f32.const -infinity)) (f64.const -infinity))
(assert_return_canonical_nan (invoke "f64.promote_f32" (f32.const nan)))

(assert_return (invoke "f32.demote_f64" (f64.const 0.0)) (f32.const 0.0))
(assert_return (invoke "f32.demote_f64" (f64.const -0.0)) (f32.const -0.0))
(assert_return (invoke "f32.demote_f64" (f64.const 0x0.0000000000001p-1022)) (f32.const 0.0))
(assert_return (invoke "f32.demote_f64" (f64.const -0x0.0000000000001p-1022)) (f32.const -0.0))
(assert_return (invoke "f32.demote_f64" (f64.const 1.0)) (f32.const 1.0))
(assert_return (invoke "f32.demote_f64" (f64.const -1.0)) (f32.const -1.0))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.fffffe0000000p-127)) (f32.const 0x1p-126))
(assert_return (invoke "f32.demote_f64" (f64.const -0x1.fffffe0000000p-127)) (f32.const -0x1p-126))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.fffffdfffffffp-127)) (f32.const 0x1.fffffcp-127))
(assert_return (invoke "f32.demote_f64" (f64.const -0x1.fffffdfffffffp-127)) (f32.const -0x1.fffffcp-127))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1p-149)) (f32.const 0x1p-149))
(assert_return (invoke "f32.demote_f64" (f64.const -0x1p-149)) (f32.const -0x1p-149))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.fffffd0000000p+127)) (f32.const 0x1.fffffcp+127))
(assert_return (invoke "f32.demote_f64" (f64.const -0x1.fffffd0000000p+127)) (f32.const -0x1.fffffcp+127))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.fffffd0000001p+127)) (f32.const 0x1.fffffep+127))
(assert_return (invoke "f32.demote_f64" (f64.const -0x1.fffffd0000001p+127)) (f32.const -0x1.fffffep+127))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.fffffep+127)) (f32.const 0x1.fffffep+127))
(assert_return (invoke "f32.demote_f64" (f64.const -0x1.fffffep+127)) (f32.const -0x1.fffffep+127))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.fffffefffffffp+127)) (f32.const 0x1.fffffep+127))
(assert_return (invoke "f32.demote_f64" (f64.const -0x1.fffffefffffffp+127)) (f32.const -0x1.fffffep+127))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.ffffffp+127)) (f32.const infinity))
(assert_return (invoke "f32.demote_f64" (f64.const -0x1.ffffffp+127)) (f32.const -infinity))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1p-119)) (f32.const 0x1p-119))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.8f867ep+125)) (f32.const 0x1.8f867ep+125))
(assert_return (invoke "f32.demote_f64" (f64.const infinity)) (f32.const infinity))
(assert_return (invoke "f32.demote_f64" (f64.const -infinity)) (f32.const -infinity))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.0000000000001p+0)) (f32.const 1.0))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.fffffffffffffp-1)) (f32.const 1.0))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.0000010000000p+0)) (f32.const 0x1.000000p+0))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.0000010000001p+0)) (f32.const 0x1.000002p+0))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.000002fffffffp+0)) (f32.const 0x1.000002p+0))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.0000030000000p+0)) (f32.const 0x1.000004p+0))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.0000050000000p+0)) (f32.const 0x1.000004p+0))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.0000010000000p+24)) (f32.const 0x1.0p+24))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.0000010000001p+24)) (f32.const 0x1.000002p+24))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.000002fffffffp+24)) (f32.const 0x1.000002p+24))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.0000030000000p+24)) (f32.const 0x1.000004p+24))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.4eae4f7024c7p+108)) (f32.const 0x1.4eae5p+108))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.a12e71e358685p-113)) (f32.const 0x1.a12e72p-113))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.cb98354d521ffp-127)) (f32.const 0x1.cb9834p-127))
(assert_return (invoke "f32.demote_f64" (f64.const -0x1.6972b30cfb562p+1)) (f32.const -0x1.6972b4p+1))
(assert_return (invoke "f32.demote_f64" (f64.const -0x1.bedbe4819d4c4p+112)) (f32.const -0x1.bedbe4p+112))
(assert_return_canonical_nan (invoke "f32.demote_f64" (f64.const nan)))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1p-1022)) (f32.const 0.0))
(assert_return (invoke "f32.demote_f64" (f64.const -0x1p-1022)) (f32.const -0.0))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.0p-150)) (f32.const 0.0))
(assert_return (invoke "f32.demote_f64" (f64.const -0x1.0p-150)) (f32.const -0.0))
(assert_return (invoke "f32.demote_f64" (f64.const 0x1.0000000000001p-150)) (f32.const 0x1p-149))
(assert_return (invoke "f32.demote_f64" (f64.const -0x1.0000000000001p-150)) (f32.const -0x1p-149))

(assert_return (invoke "f32.reinterpret_i32" (i32.const 0)) (f32.const 0.0))
(assert_return (invoke "f32.reinterpret_i32" (i32.const 0x80000000)) (f32.const -0.0))
(assert_return (invoke "f32.reinterpret_i32" (i32.const 1)) (f32.const 0x1p-149))
(assert_return_arithmetic_nan (invoke "f32.reinterpret_i32" (i32.const -1)))
(assert_return (invoke "f32.reinterpret_i32" (i32.const 123456789)) (f32.const 0x1.b79a2ap-113))
(assert_return (invoke "f32.reinterpret_i32" (i32.const -2147483647)) (f32.const -0x1p-149))
(assert_return (invoke "f32.reinterpret_i32" (i32.const 0x7f800000)) (f32.const infinity))
(assert_return (invoke "f32.reinterpret_i32" (i32.const 0xff800000)) (f32.const -infinity))
(assert_return (invoke "f32.reinterpret_i32" (i32.const 0x7fc00000)) (f32.const nan))
(assert_return (invoke "f32.reinterpret_i32" (i32.const 0xffc00000)) (f32.const -nan))
(assert_return (invoke "f32.reinterpret_i32" (i32.const 0x7fa00000)) (f32.const nan:0x200000))
(assert_return (invoke "f32.reinterpret_i32" (i32.const 0xffa00000)) (f32.const -nan:0x200000))

(assert_return (invoke "f64.reinterpret_i64" (i64.const 0)) (f64.const 0.0))
(assert_return (invoke "f64.reinterpret_i64" (i64.const 1)) (f64.const 0x0.0000000000001p-1022))
(assert_return_arithmetic_nan (invoke "f64.reinterpret_i64" (i64.const -1)))
(assert_return (invoke "f64.reinterpret_i64" (i64.const 0x8000000000000000)) (f64.const -0.0))
(assert_return (invoke "f64.reinterpret_i64" (i64.const 1234567890)) (f64.const 0x0.00000499602d2p-1022))
(assert_return (invoke "f64.reinterpret_i64" (i64.const -9223372036854775807)) (f64.const -0x0.0000000000001p-1022))
(assert_return (invoke "f64.reinterpret_i64" (i64.const 0x7ff0000000000000)) (f64.const infinity))
(assert_return (invoke "f64.reinterpret_i64" (i64.const 0xfff0000000000000)) (f64.const -infinity))
(assert_return_canonical_nan (invoke "f64.reinterpret_i64" (i64.const 0x7ff8000000000000)))
(assert_return_canonical_nan (invoke "f64.reinterpret_i64" (i64.const 0xfff8000000000000)))
(assert_return (invoke "f64.reinterpret_i64" (i64.const 0x7ff4000000000000)) (f64.const nan:0x4000000000000))
(assert_return (invoke "f64.reinterpret_i64" (i64.const 0xfff4000000000000)) (f64.const -nan:0x4000000000000))

(assert_return (invoke "i32.reinterpret_f32" (f32.const 0.0)) (i32.const 0))
(assert_return (invoke "i32.reinterpret_f32" (f32.const -0.0)) (i32.const 0x80000000))
(assert_return (invoke "i32.reinterpret_f32" (f32.const 0x1p-149)) (i32.const 1))
(assert_return (invoke "i32.reinterpret_f32" (f32.const -nan:0x7fffff)) (i32.const -1))
(assert_return (invoke "i32.reinterpret_f32" (f32.const -0x1p-149)) (i32.const 0x80000001))
(assert_return (invoke "i32.reinterpret_f32" (f32.const 1.0)) (i32.const 1065353216))
(assert_return (invoke "i32.reinterpret_f32" (f32.const 3.1415926)) (i32.const 1078530010))
(assert_return (invoke "i32.reinterpret_f32" (f32.const 0x1.fffffep+127)) (i32.const 2139095039))
(assert_return (invoke "i32.reinterpret_f32" (f32.const -0x1.fffffep+127)) (i32.const -8388609))
(assert_return (invoke "i32.reinterpret_f32" (f32.const infinity)) (i32.const 0x7f800000))
(assert_return (invoke "i32.reinterpret_f32" (f32.const -infinity)) (i32.const 0xff800000))
(assert_return (invoke "i32.reinterpret_f32" (f32.const nan)) (i32.const 0x7fc00000))
(assert_return (invoke "i32.reinterpret_f32" (f32.const -nan)) (i32.const 0xffc00000))
(assert_return (invoke "i32.reinterpret_f32" (f32.const nan:0x200000)) (i32.const 0x7fa00000))
(assert_return (invoke "i32.reinterpret_f32" (f32.const -nan:0x200000)) (i32.const 0xffa00000))

(assert_return (invoke "i64.reinterpret_f64" (f64.const 0.0)) (i64.const 0))
(assert_return (invoke "i64.reinterpret_f64" (f64.const -0.0)) (i64.const 0x8000000000000000))
(assert_return (invoke "i64.reinterpret_f64" (f64.const 0x0.0000000000001p-1022)) (i64.const 1))
(assert_return (invoke "i64.reinterpret_f64" (f64.const -nan:0xfffffffffffff)) (i64.const -1))
(assert_return (invoke "i64.reinterpret_f64" (f64.const -0x0.0000000000001p-1022)) (i64.const 0x8000000000000001))
(assert_return (invoke "i64.reinterpret_f64" (f64.const 1.0)) (i64.const 4607182418800017408))
(assert_return (invoke "i64.reinterpret_f64" (f64.const 3.14159265358979)) (i64.const 4614256656552045841))
(assert_return (invoke "i64.reinterpret_f64" (f64.const 0x1.fffffffffffffp+1023)) (i64.const 9218868437227405311))
(assert_return (invoke "i64.reinterpret_f64" (f64.const -0x1.fffffffffffffp+1023)) (i64.const -4503599627370497))
(assert_return (invoke "i64.reinterpret_f64" (f64.const infinity)) (i64.const 0x7ff0000000000000))
(assert_return (invoke "i64.reinterpret_f64" (f64.const -infinity)) (i64.const 0xfff0000000000000))
(assert_return (invoke "i64.reinterpret_f64" (f64.const nan)) (i64.const 0x7ff8000000000000))
(assert_return (invoke "i64.reinterpret_f64" (f64.const -nan)) (i64.const 0xfff8000000000000))
;;(assert_return (invoke "i64.reinterpret_f64" (f64.const nan:0x4000000000000)) (i64.const 0x7ff4000000000000))
;;(assert_return (invoke "i64.reinterpret_f64" (f64.const -nan:0x4000000000000)) (i64.const 0xfff4000000000000))
