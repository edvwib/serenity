/*
 * Copyright (c) 2020, Andreas Kling <kling@serenityos.org>
 * Copyright (c) 2020-2023, Linus Groh <linusg@serenityos.org>
 * Copyright (c) 2021, Idan Horowitz <idan.horowitz@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/BuiltinWrappers.h>
#include <AK/Function.h>
#include <AK/Random.h>
#include <LibJS/Runtime/GlobalObject.h>
#include <LibJS/Runtime/MathObject.h>
#include <math.h>

namespace JS {

MathObject::MathObject(Realm& realm)
    : Object(ConstructWithPrototypeTag::Tag, realm.intrinsics().object_prototype())
{
}

ThrowCompletionOr<void> MathObject::initialize(Realm& realm)
{
    auto& vm = this->vm();
    MUST_OR_THROW_OOM(Base::initialize(realm));
    u8 attr = Attribute::Writable | Attribute::Configurable;
    define_native_function(realm, vm.names.abs, abs, 1, attr);
    define_native_function(realm, vm.names.random, random, 0, attr);
    define_native_function(realm, vm.names.sqrt, sqrt, 1, attr);
    define_native_function(realm, vm.names.floor, floor, 1, attr);
    define_native_function(realm, vm.names.ceil, ceil, 1, attr);
    define_native_function(realm, vm.names.round, round, 1, attr);
    define_native_function(realm, vm.names.max, max, 2, attr);
    define_native_function(realm, vm.names.min, min, 2, attr);
    define_native_function(realm, vm.names.trunc, trunc, 1, attr);
    define_native_function(realm, vm.names.sin, sin, 1, attr);
    define_native_function(realm, vm.names.cos, cos, 1, attr);
    define_native_function(realm, vm.names.tan, tan, 1, attr);
    define_native_function(realm, vm.names.pow, pow, 2, attr);
    define_native_function(realm, vm.names.exp, exp, 1, attr);
    define_native_function(realm, vm.names.expm1, expm1, 1, attr);
    define_native_function(realm, vm.names.sign, sign, 1, attr);
    define_native_function(realm, vm.names.clz32, clz32, 1, attr);
    define_native_function(realm, vm.names.acos, acos, 1, attr);
    define_native_function(realm, vm.names.acosh, acosh, 1, attr);
    define_native_function(realm, vm.names.asin, asin, 1, attr);
    define_native_function(realm, vm.names.asinh, asinh, 1, attr);
    define_native_function(realm, vm.names.atan, atan, 1, attr);
    define_native_function(realm, vm.names.atanh, atanh, 1, attr);
    define_native_function(realm, vm.names.log1p, log1p, 1, attr);
    define_native_function(realm, vm.names.cbrt, cbrt, 1, attr);
    define_native_function(realm, vm.names.atan2, atan2, 2, attr);
    define_native_function(realm, vm.names.fround, fround, 1, attr);
    define_native_function(realm, vm.names.hypot, hypot, 2, attr);
    define_native_function(realm, vm.names.imul, imul, 2, attr);
    define_native_function(realm, vm.names.log, log, 1, attr);
    define_native_function(realm, vm.names.log2, log2, 1, attr);
    define_native_function(realm, vm.names.log10, log10, 1, attr);
    define_native_function(realm, vm.names.sinh, sinh, 1, attr);
    define_native_function(realm, vm.names.cosh, cosh, 1, attr);
    define_native_function(realm, vm.names.tanh, tanh, 1, attr);

    // 21.3.1 Value Properties of the Math Object, https://tc39.es/ecma262/#sec-value-properties-of-the-math-object
    define_direct_property(vm.names.E, Value(M_E), 0);
    define_direct_property(vm.names.LN2, Value(M_LN2), 0);
    define_direct_property(vm.names.LN10, Value(M_LN10), 0);
    define_direct_property(vm.names.LOG2E, Value(::log2(M_E)), 0);
    define_direct_property(vm.names.LOG10E, Value(::log10(M_E)), 0);
    define_direct_property(vm.names.PI, Value(M_PI), 0);
    define_direct_property(vm.names.SQRT1_2, Value(M_SQRT1_2), 0);
    define_direct_property(vm.names.SQRT2, Value(M_SQRT2), 0);

    // 21.3.1.9 Math [ @@toStringTag ], https://tc39.es/ecma262/#sec-math-@@tostringtag
    define_direct_property(vm.well_known_symbol_to_string_tag(), PrimitiveString::create(vm, vm.names.Math.as_string()), Attribute::Configurable);

    return {};
}

// 21.3.2.1 Math.abs ( x ), https://tc39.es/ecma262/#sec-math.abs
JS_DEFINE_NATIVE_FUNCTION(MathObject::abs)
{
    auto number = TRY(vm.argument(0).to_number(vm));
    if (number.is_nan())
        return js_nan();
    if (number.is_negative_zero())
        return Value(0);
    if (number.is_negative_infinity())
        return js_infinity();
    return Value(number.as_double() < 0 ? -number.as_double() : number.as_double());
}

// 21.3.2.27 Math.random ( ), https://tc39.es/ecma262/#sec-math.random
JS_DEFINE_NATIVE_FUNCTION(MathObject::random)
{
    double r = (double)get_random<u32>() / (double)UINT32_MAX;
    return Value(r);
}

// 21.3.2.32 Math.sqrt ( x ), https://tc39.es/ecma262/#sec-math.sqrt
JS_DEFINE_NATIVE_FUNCTION(MathObject::sqrt)
{
    auto number = TRY(vm.argument(0).to_number(vm));
    if (number.is_nan())
        return js_nan();
    return Value(::sqrt(number.as_double()));
}

// 21.3.2.16 Math.floor ( x ), https://tc39.es/ecma262/#sec-math.floor
JS_DEFINE_NATIVE_FUNCTION(MathObject::floor)
{
    auto number = TRY(vm.argument(0).to_number(vm));
    if (number.is_nan())
        return js_nan();
    return Value(::floor(number.as_double()));
}

// 21.3.2.10 Math.ceil ( x ), https://tc39.es/ecma262/#sec-math.ceil
JS_DEFINE_NATIVE_FUNCTION(MathObject::ceil)
{
    auto number = TRY(vm.argument(0).to_number(vm));
    if (number.is_nan())
        return js_nan();
    auto number_double = number.as_double();
    if (number_double < 0 && number_double > -1)
        return Value(-0.f);
    return Value(::ceil(number.as_double()));
}

// 21.3.2.28 Math.round ( x ), https://tc39.es/ecma262/#sec-math.round
JS_DEFINE_NATIVE_FUNCTION(MathObject::round)
{
    auto value = TRY(vm.argument(0).to_number(vm)).as_double();
    double integer = ::ceil(value);
    if (integer - 0.5 > value)
        integer--;
    return Value(integer);
}

// 21.3.2.24 Math.max ( ...args ), https://tc39.es/ecma262/#sec-math.max
JS_DEFINE_NATIVE_FUNCTION(MathObject::max)
{
    Vector<Value> coerced;
    for (size_t i = 0; i < vm.argument_count(); ++i)
        coerced.append(TRY(vm.argument(i).to_number(vm)));

    auto highest = js_negative_infinity();
    for (auto& number : coerced) {
        if (number.is_nan())
            return js_nan();
        if ((number.is_positive_zero() && highest.is_negative_zero()) || number.as_double() > highest.as_double())
            highest = number;
    }
    return highest;
}

// 21.3.2.25 Math.min ( ...args ), https://tc39.es/ecma262/#sec-math.min
JS_DEFINE_NATIVE_FUNCTION(MathObject::min)
{
    Vector<Value> coerced;
    for (size_t i = 0; i < vm.argument_count(); ++i)
        coerced.append(TRY(vm.argument(i).to_number(vm)));

    auto lowest = js_infinity();
    for (auto& number : coerced) {
        if (number.is_nan())
            return js_nan();
        if ((number.is_negative_zero() && lowest.is_positive_zero()) || number.as_double() < lowest.as_double())
            lowest = number;
    }
    return lowest;
}

// 21.3.2.35 Math.trunc ( x ), https://tc39.es/ecma262/#sec-math.trunc
JS_DEFINE_NATIVE_FUNCTION(MathObject::trunc)
{
    auto number = TRY(vm.argument(0).to_number(vm));
    if (number.is_nan())
        return js_nan();
    if (number.as_double() < 0)
        return MathObject::ceil(vm);
    return MathObject::floor(vm);
}

// 21.3.2.30 Math.sin ( x ), https://tc39.es/ecma262/#sec-math.sin
JS_DEFINE_NATIVE_FUNCTION(MathObject::sin)
{
    // 1. Let n be ? ToNumber(x).
    auto number = TRY(vm.argument(0).to_number(vm));
    // 2. If n is NaN, n is +0𝔽, or n is -0𝔽, return n.
    if (number.is_nan() || number.is_positive_zero() || number.is_negative_zero())
        return number;

    // 3. If n is +∞𝔽 or n is -∞𝔽, return NaN.
    if (number.is_infinity())
        return js_nan();

    // 4. Return an implementation-approximated Number value representing the result of the sine of ℝ(n).
    return Value(::sin(number.as_double()));
}

// 21.3.2.12 Math.cos ( x ), https://tc39.es/ecma262/#sec-math.cos
JS_DEFINE_NATIVE_FUNCTION(MathObject::cos)
{
    // 1. Let n be ? ToNumber(x).
    auto number = TRY(vm.argument(0).to_number(vm));

    // 2. If n is NaN, n is +∞𝔽, or n is -∞𝔽, return NaN.
    if (number.is_nan() || number.is_infinity())
        return js_nan();

    // 3. If n is +0𝔽 or n is -0𝔽, return 1𝔽.
    if (number.is_positive_zero() || number.is_negative_zero())
        return Value(1);

    // 4. Return an implementation-approximated Number value representing the result of the cosine of ℝ(n).
    return Value(::cos(number.as_double()));
}

// 21.3.2.33 Math.tan ( x ), https://tc39.es/ecma262/#sec-math.tan
JS_DEFINE_NATIVE_FUNCTION(MathObject::tan)
{
    // Let n be ? ToNumber(x).
    auto number = TRY(vm.argument(0).to_number(vm));

    // 2. If n is NaN, n is +0𝔽, or n is -0𝔽, return n.
    if (number.is_nan() || number.is_positive_zero() || number.is_negative_zero())
        return number;

    // 3. If n is +∞𝔽, or n is -∞𝔽, return NaN.
    if (number.is_infinity())
        return js_nan();

    // 4. Return an implementation-approximated Number value representing the result of the tangent of ℝ(n).
    return Value(::tan(number.as_double()));
}

// 21.3.2.26 Math.pow ( base, exponent ), https://tc39.es/ecma262/#sec-math.pow
JS_DEFINE_NATIVE_FUNCTION(MathObject::pow)
{
    auto base = TRY(vm.argument(0).to_number(vm));
    auto exponent = TRY(vm.argument(1).to_number(vm));
    return JS::exp(vm, base, exponent);
}

// 21.3.2.14 Math.exp ( x ), https://tc39.es/ecma262/#sec-math.exp
JS_DEFINE_NATIVE_FUNCTION(MathObject::exp)
{
    auto number = TRY(vm.argument(0).to_number(vm));
    if (number.is_nan())
        return js_nan();
    return Value(::exp(number.as_double()));
}

// 21.3.2.15 Math.expm1 ( x ), https://tc39.es/ecma262/#sec-math.expm1
JS_DEFINE_NATIVE_FUNCTION(MathObject::expm1)
{
    auto number = TRY(vm.argument(0).to_number(vm));
    if (number.is_nan())
        return js_nan();
    return Value(::expm1(number.as_double()));
}

// 21.3.2.29 Math.sign ( x ), https://tc39.es/ecma262/#sec-math.sign
JS_DEFINE_NATIVE_FUNCTION(MathObject::sign)
{
    auto number = TRY(vm.argument(0).to_number(vm));
    if (number.is_positive_zero())
        return Value(0);
    if (number.is_negative_zero())
        return Value(-0.0);
    if (number.as_double() > 0)
        return Value(1);
    if (number.as_double() < 0)
        return Value(-1);
    return js_nan();
}

// 21.3.2.11 Math.clz32 ( x ), https://tc39.es/ecma262/#sec-math.clz32
JS_DEFINE_NATIVE_FUNCTION(MathObject::clz32)
{
    auto number = TRY(vm.argument(0).to_u32(vm));
    if (number == 0)
        return Value(32);
    return Value(count_leading_zeroes(number));
}

// 21.3.2.2 Math.acos ( x ), https://tc39.es/ecma262/#sec-math.acos
JS_DEFINE_NATIVE_FUNCTION(MathObject::acos)
{
    auto number = TRY(vm.argument(0).to_number(vm));
    if (number.is_nan() || number.as_double() > 1 || number.as_double() < -1)
        return js_nan();
    if (number.as_double() == 1)
        return Value(0);
    return Value(::acos(number.as_double()));
}

// 21.3.2.3 Math.acosh ( x ), https://tc39.es/ecma262/#sec-math.acosh
JS_DEFINE_NATIVE_FUNCTION(MathObject::acosh)
{
    // 1. Let n be ? ToNumber(x).
    auto number = TRY(vm.argument(0).to_number(vm));

    // 2. If n is NaN or n is +∞𝔽, return n.
    if (number.is_nan() || number.is_positive_infinity())
        return number;

    // 3. If n is 1𝔽, return +0𝔽.
    if (number.as_double() == 1.0)
        return Value(0.0);

    // 4. If n < 1𝔽, return NaN.
    if (number.as_double() < 1)
        return js_nan();

    // 5. Return an implementation-approximated Number value representing the result of the inverse hyperbolic cosine of ℝ(n).
    return Value(::acosh(number.as_double()));
}

// 21.3.2.4 Math.asin ( x ), https://tc39.es/ecma262/#sec-math.asin
JS_DEFINE_NATIVE_FUNCTION(MathObject::asin)
{
    // 1. Let n be ? ToNumber(x).
    auto number = TRY(vm.argument(0).to_number(vm));

    // 2. If n is NaN, n is +0𝔽, or n is -0𝔽, return n.
    if (number.is_nan() || number.is_positive_zero() || number.is_negative_zero())
        return number;

    // 3. If n > 1𝔽 or n < -1𝔽, return NaN.
    if (number.as_double() > 1 || number.as_double() < -1)
        return js_nan();

    // 4. Return an implementation-approximated Number value representing the result of the inverse sine of ℝ(n).
    return Value(::asin(number.as_double()));
}

// 21.3.2.5 Math.asinh ( x ), https://tc39.es/ecma262/#sec-math.asinh
JS_DEFINE_NATIVE_FUNCTION(MathObject::asinh)
{
    // 1. Let n be ? ToNumber(x).
    auto number = TRY(vm.argument(0).to_number(vm));

    // 2. If n is not finite or n is either +0𝔽 or -0𝔽, return n.
    if (!number.is_finite_number() || number.is_positive_zero() || number.is_negative_zero())
        return number;

    // 3. Return an implementation-approximated Number value representing the result of the inverse hyperbolic sine of ℝ(n).
    return Value(::asinh(number.as_double()));
}

// 21.3.2.6 Math.atan ( x ), https://tc39.es/ecma262/#sec-math.atan
JS_DEFINE_NATIVE_FUNCTION(MathObject::atan)
{
    auto number = TRY(vm.argument(0).to_number(vm));
    if (number.is_nan() || number.is_positive_zero() || number.is_negative_zero())
        return number;
    if (number.is_positive_infinity())
        return Value(M_PI_2);
    if (number.is_negative_infinity())
        return Value(-M_PI_2);
    return Value(::atan(number.as_double()));
}

// 21.3.2.7 Math.atanh ( x ), https://tc39.es/ecma262/#sec-math.atanh
JS_DEFINE_NATIVE_FUNCTION(MathObject::atanh)
{
    // 1. Let n be ? ToNumber(x).
    auto number = TRY(vm.argument(0).to_number(vm));

    // 2. If n is NaN, n is +0𝔽, or n is -0𝔽, return n.
    if (number.is_nan() || number.is_positive_zero() || number.is_negative_zero())
        return number;

    // 3. If n > 1𝔽 or n < -1𝔽, return NaN.
    if (number.as_double() > 1. || number.as_double() < -1.)
        return js_nan();

    // 4. If n is 1𝔽, return +∞𝔽.
    if (number.as_double() == 1.)
        return js_infinity();

    // 5. If n is -1𝔽, return -∞𝔽.
    if (number.as_double() == -1.)
        return js_negative_infinity();

    // 6. Return an implementation-approximated Number value representing the result of the inverse hyperbolic tangent of ℝ(n).
    return Value(::atanh(number.as_double()));
}

// 21.3.2.21 Math.log1p ( x ), https://tc39.es/ecma262/#sec-math.log1p
JS_DEFINE_NATIVE_FUNCTION(MathObject::log1p)
{
    // 1. Let n be ? ToNumber(x).
    auto number = TRY(vm.argument(0).to_number(vm));

    // 2. If n is NaN, n is +0𝔽, n is -0𝔽, or n is +∞𝔽, return n.
    if (number.is_nan() || number.is_positive_zero() || number.is_negative_zero() || number.is_positive_infinity())
        return number;

    // 3. If n is -1𝔽, return -∞𝔽.
    if (number.as_double() == -1.)
        return js_negative_infinity();

    // 4. If n < -1𝔽, return NaN.
    if (number.as_double() < -1.)
        return js_nan();

    // 5. Return an implementation-approximated Number value representing the result of the natural logarithm of 1 + ℝ(n).
    return Value(::log1p(number.as_double()));
}

// 21.3.2.9 Math.cbrt ( x ), https://tc39.es/ecma262/#sec-math.cbrt
JS_DEFINE_NATIVE_FUNCTION(MathObject::cbrt)
{
    return Value(::cbrt(TRY(vm.argument(0).to_number(vm)).as_double()));
}

// 21.3.2.8 Math.atan2 ( y, x ), https://tc39.es/ecma262/#sec-math.atan2
JS_DEFINE_NATIVE_FUNCTION(MathObject::atan2)
{
    auto constexpr three_quarters_pi = M_PI_4 + M_PI_2;

    auto y = TRY(vm.argument(0).to_number(vm));
    auto x = TRY(vm.argument(1).to_number(vm));

    if (y.is_nan() || x.is_nan())
        return js_nan();
    if (y.is_positive_infinity()) {
        if (x.is_positive_infinity())
            return Value(M_PI_4);
        else if (x.is_negative_infinity())
            return Value(three_quarters_pi);
        else
            return Value(M_PI_2);
    }
    if (y.is_negative_infinity()) {
        if (x.is_positive_infinity())
            return Value(-M_PI_4);
        else if (x.is_negative_infinity())
            return Value(-three_quarters_pi);
        else
            return Value(-M_PI_2);
    }
    if (y.is_positive_zero()) {
        if (x.as_double() > 0 || x.is_positive_zero())
            return Value(0.0);
        else
            return Value(M_PI);
    }
    if (y.is_negative_zero()) {
        if (x.as_double() > 0 || x.is_positive_zero())
            return Value(-0.0);
        else
            return Value(-M_PI);
    }
    VERIFY(y.is_finite_number() && !y.is_positive_zero() && !y.is_negative_zero());
    if (y.as_double() > 0) {
        if (x.is_positive_infinity())
            return Value(0);
        else if (x.is_negative_infinity())
            return Value(M_PI);
        else if (x.is_positive_zero() || x.is_negative_zero())
            return Value(M_PI_2);
    }
    if (y.as_double() < 0) {
        if (x.is_positive_infinity())
            return Value(-0.0);
        else if (x.is_negative_infinity())
            return Value(-M_PI);
        else if (x.is_positive_zero() || x.is_negative_zero())
            return Value(-M_PI_2);
    }
    VERIFY(x.is_finite_number() && !x.is_positive_zero() && !x.is_negative_zero());
    return Value(::atan2(y.as_double(), x.as_double()));
}

// 21.3.2.17 Math.fround ( x ), https://tc39.es/ecma262/#sec-math.fround
JS_DEFINE_NATIVE_FUNCTION(MathObject::fround)
{
    auto number = TRY(vm.argument(0).to_number(vm));
    if (number.is_nan())
        return js_nan();
    return Value((float)number.as_double());
}

// 21.3.2.18 Math.hypot ( ...args ), https://tc39.es/ecma262/#sec-math.hypot
JS_DEFINE_NATIVE_FUNCTION(MathObject::hypot)
{
    Vector<Value> coerced;
    for (size_t i = 0; i < vm.argument_count(); ++i)
        coerced.append(TRY(vm.argument(i).to_number(vm)));

    for (auto& number : coerced) {
        if (number.is_positive_infinity() || number.is_negative_infinity())
            return js_infinity();
    }

    auto only_zero = true;
    double sum_of_squares = 0;
    for (auto& number : coerced) {
        if (number.is_nan() || number.is_positive_infinity())
            return number;
        if (number.is_negative_infinity())
            return js_infinity();
        if (!number.is_positive_zero() && !number.is_negative_zero())
            only_zero = false;
        sum_of_squares += number.as_double() * number.as_double();
    }
    if (only_zero)
        return Value(0);
    return Value(::sqrt(sum_of_squares));
}

// 21.3.2.19 Math.imul ( x, y ), https://tc39.es/ecma262/#sec-math.imul
JS_DEFINE_NATIVE_FUNCTION(MathObject::imul)
{
    auto a = TRY(vm.argument(0).to_u32(vm));
    auto b = TRY(vm.argument(1).to_u32(vm));
    return Value(static_cast<i32>(a * b));
}

// 21.3.2.20 Math.log ( x ), https://tc39.es/ecma262/#sec-math.log
JS_DEFINE_NATIVE_FUNCTION(MathObject::log)
{
    // 1. Let n be ? ToNumber(x).
    auto number = TRY(vm.argument(0).to_number(vm));

    // 2. If n is NaN or n is +∞𝔽, return n.
    if (number.is_nan() || number.is_positive_infinity())
        return number;

    // 3. If n is 1𝔽, return +0𝔽.
    if (number.as_double() == 1.)
        return Value(0);

    // 4. If n is +0𝔽 or n is -0𝔽, return -∞𝔽.
    if (number.is_positive_zero() || number.is_negative_zero())
        return js_negative_infinity();

    // 5. If n < -0𝔽, return NaN.
    if (number.as_double() < -0.)
        return js_nan();

    // 6. Return an implementation-approximated Number value representing the result of the natural logarithm of ℝ(n).
    return Value(::log(number.as_double()));
}

// 21.3.2.23 Math.log2 ( x ), https://tc39.es/ecma262/#sec-math.log2
JS_DEFINE_NATIVE_FUNCTION(MathObject::log2)
{
    // 1. Let n be ? ToNumber(x).
    auto number = TRY(vm.argument(0).to_number(vm));

    // 2. If n is NaN or n is +∞𝔽, return n.
    if (number.is_nan() || number.is_positive_infinity())
        return number;

    // 3. If n is 1𝔽, return +0𝔽.
    if (number.as_double() == 1.)
        return Value(0);

    // 4. If n is +0𝔽 or n is -0𝔽, return -∞𝔽.
    if (number.is_positive_zero() || number.is_negative_zero())
        return js_negative_infinity();

    // 5. If n < -0𝔽, return NaN.
    if (number.as_double() < -0.)
        return js_nan();

    // 6. Return an implementation-approximated Number value representing the result of the base 2 logarithm of ℝ(n).
    return Value(::log2(number.as_double()));
}

// 21.3.2.22 Math.log10 ( x ), https://tc39.es/ecma262/#sec-math.log10
JS_DEFINE_NATIVE_FUNCTION(MathObject::log10)
{
    // 1. Let n be ? ToNumber(x).
    auto number = TRY(vm.argument(0).to_number(vm));

    // 2. If n is NaN or n is +∞𝔽, return n.
    if (number.is_nan() || number.is_positive_infinity())
        return number;

    // 3. If n is 1𝔽, return +0𝔽.
    if (number.as_double() == 1.)
        return Value(0);

    // 4. If n is +0𝔽 or n is -0𝔽, return -∞𝔽.
    if (number.is_positive_zero() || number.is_negative_zero())
        return js_negative_infinity();

    // 5. If n < -0𝔽, return NaN.
    if (number.as_double() < -0.)
        return js_nan();

    // 6. Return an implementation-approximated Number value representing the result of the base 10 logarithm of ℝ(n).
    return Value(::log10(number.as_double()));
}

// 21.3.2.31 Math.sinh ( x ), https://tc39.es/ecma262/#sec-math.sinh
JS_DEFINE_NATIVE_FUNCTION(MathObject::sinh)
{
    // 1. Let n be ? ToNumber(x).
    auto number = TRY(vm.argument(0).to_number(vm));

    // 2. If n is not finite or n is either +0𝔽 or -0𝔽, return n.
    if (!number.is_finite_number() || number.is_positive_zero() || number.is_negative_zero())
        return number;

    // 3. Return an implementation-approximated Number value representing the result of the hyperbolic sine of ℝ(n).
    return Value(::sinh(number.as_double()));
}

// 21.3.2.13 Math.cosh ( x ), https://tc39.es/ecma262/#sec-math.cosh
JS_DEFINE_NATIVE_FUNCTION(MathObject::cosh)
{
    // 1. Let n be ? ToNumber(x).
    auto number = TRY(vm.argument(0).to_number(vm));

    // 2. If n is NaN, return NaN.
    if (number.is_nan())
        return js_nan();

    // 3. If n is +∞𝔽 or n is -∞𝔽, return +∞𝔽.
    if (number.is_positive_infinity() || number.is_negative_infinity())
        return js_infinity();

    // 4. If n is +0𝔽 or n is -0𝔽, return 1𝔽.
    if (number.is_positive_zero() || number.is_negative_zero())
        return Value(1);

    // 5. Return an implementation-approximated Number value representing the result of the hyperbolic cosine of ℝ(n).
    return Value(::cosh(number.as_double()));
}

// 21.3.2.34 Math.tanh ( x ), https://tc39.es/ecma262/#sec-math.tanh
JS_DEFINE_NATIVE_FUNCTION(MathObject::tanh)
{
    // 1. Let n be ? ToNumber(x).
    auto number = TRY(vm.argument(0).to_number(vm));

    // 2. If n is NaN, n is +0𝔽, or n is -0𝔽, return n.
    if (number.is_nan() || number.is_positive_zero() || number.is_negative_zero())
        return number;

    // 3. If n is +∞𝔽, return 1𝔽.
    if (number.is_positive_infinity())
        return Value(1);

    // 4. If n is -∞𝔽, return -1𝔽.
    if (number.is_negative_infinity())
        return Value(-1);

    // 5. Return an implementation-approximated Number value representing the result of the hyperbolic tangent of ℝ(n).
    return Value(::tanh(number.as_double()));
}

}
