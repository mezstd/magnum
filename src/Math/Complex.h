#ifndef Magnum_Math_Complex_h
#define Magnum_Math_Complex_h
/*
    Copyright © 2010, 2011, 2012 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Magnum.

    Magnum is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Magnum is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

/** @file
 * @brief Class Magnum::Math::Complex
 */

#include <limits>
#include <Utility/Assert.h>
#include <Utility/Debug.h>

#include "Math/Matrix.h"
#include "Math/Vector2.h"

namespace Magnum { namespace Math {

/**
@brief %Complex number
@tparam T   Data type

Represents 2D rotation. See @ref transformations for brief introduction.
@see Magnum::Complex, Matrix3
*/
template<class T> class Complex {
    public:
        typedef T Type;                         /**< @brief Underlying data type */

        /**
         * @brief Dot product
         *
         * @f[
         *      c_0 \cdot c_1 = a_0 a_1 + b_0 b_1
         * @f]
         * @see dot() const
         */
        inline static T dot(const Complex<T>& a, const Complex<T>& b) {
            return a._real*b._real + a._imaginary*b._imaginary;
        }

        /**
         * @brief Angle between normalized complex numbers
         *
         * Expects that both complex numbers are normalized. @f[
         *      \theta = acos \left( \frac{Re(c_0 \cdot c_1))}{|c_0| |c_1|} \right) = acos (a_0 a_1 + b_0 b_1)
         * @f]
         * @see Quaternion::angle(), Vector::angle()
         */
        inline static Rad<T> angle(const Complex<T>& normalizedA, const Complex<T>& normalizedB) {
            CORRADE_ASSERT(MathTypeTraits<T>::equals(normalizedA.dot(), T(1)) && MathTypeTraits<T>::equals(normalizedB.dot(), T(1)),
                           "Math::Complex::angle(): complex numbers must be normalized", Rad<T>(std::numeric_limits<T>::quiet_NaN()));
            return Rad<T>(std::acos(normalizedA._real*normalizedB._real + normalizedA._imaginary*normalizedB._imaginary));
        }

        /**
         * @brief Rotation complex number
         * @param angle             Rotation angle (counterclockwise)
         *
         * @f[
         *      c = cos \theta + i sin \theta
         * @f]
         * @see rotationAngle(), Matrix3::rotation(), Quaternion::rotation()
         */
        inline static Complex<T> rotation(Rad<T> angle) {
            return {std::cos(T(angle)), std::sin(T(angle))};
        }

        /**
         * @brief Default constructor
         *
         * Constructs unit complex number. @f[
         *      c = 1 + i0
         * @f]
         */
        inline constexpr /*implicit*/ Complex(): _real(T(1)), _imaginary(T(0)) {}

        /**
         * @brief Construct complex number from real and imaginary part
         *
         * @f[
         *      c = a + ib
         * @f]
         */
        inline constexpr /*implicit*/ Complex(T real, T imaginary): _real(real), _imaginary(imaginary) {}

        /**
         * @brief Construct complex number from vector
         *
         * To be used in transformations later. @f[
         *      c = v_x + iv_y
         * @f]
         * @see operator Vector2(), transformVector()
         */
        inline constexpr explicit Complex(const Vector2<T>& vector): _real(vector.x()), _imaginary(vector.y()) {}

        /** @brief Equality comparison */
        inline bool operator==(const Complex<T>& other) const {
            return MathTypeTraits<T>::equals(_real, other._real) &&
                   MathTypeTraits<T>::equals(_imaginary, other._imaginary);
        }

        /** @brief Non-equality comparison */
        inline bool operator!=(const Complex<T>& other) const {
            return !operator==(other);
        }

        /** @brief Real part */
        inline constexpr T real() const { return _real; }

        /** @brief Imaginary part */
        inline constexpr T imaginary() const { return _imaginary; }

        /**
         * @brief Convert complex number to vector
         *
         * @f[
         *      \boldsymbol v = \begin{pmatrix} a \\ b \end{pmatrix}
         * @f]
         */
        inline constexpr explicit operator Vector2<T>() const {
            return {_real, _imaginary};
        }

        /**
         * @brief Rotation angle of complex number
         *
         * @f[
         *      \theta = atan2(b, a)
         * @f]
         * @see rotation(), DualComplex::rotationAngle()
         */
        inline Rad<T> rotationAngle() const {
            return Rad<T>(std::atan2(_imaginary, _real));
        }

        /**
         * @brief Convert complex number to rotation matrix
         *
         * @f[
         *      M = \begin{pmatrix}
         *               a & -b \\
         *               b &  a
         *          \end{pmatrix}
         * @f]
         * @see DualComplex::toMatrix(), Matrix3::from(const Matrix<2, T>&, const Vector2<T>&)
         */
        Matrix<2, T> toMatrix() const {
            return {Vector<2, T>(_real, _imaginary),
                    Vector<2, T>(-_imaginary, _real)};
        }

        /**
         * @brief Add complex number and assign
         *
         * The computation is done in-place. @f[
         *      c_0 + c_1 = (a_0 + a_1) + i(b_0 + b_1)
         * @f]
         */
        inline Complex<T>& operator+=(const Complex<T>& other) {
            _real += other._real;
            _imaginary += other._imaginary;
            return *this;
        }

        /**
         * @brief Add complex number
         *
         * @see operator+=()
         */
        inline Complex<T> operator+(const Complex<T>& other) const {
            return Complex<T>(*this) += other;
        }

        /**
         * @brief Negated complex number
         *
         * @f[
         *      -c = -a -ib
         * @f]
         */
        inline Complex<T> operator-() const {
            return {-_real, -_imaginary};
        }

        /**
         * @brief Subtract complex number and assign
         *
         * The computation is done in-place. @f[
         *      c_0 - c_1 = (a_0 - a_1) + i(b_0 - b_1)
         * @f]
         */
        inline Complex<T>& operator-=(const Complex<T>& other) {
            _real -= other._real;
            _imaginary -= other._imaginary;
            return *this;
        }

        /**
         * @brief Subtract complex number
         *
         * @see operator-=()
         */
        inline Complex<T> operator-(const Complex<T>& other) const {
            return Complex<T>(*this) -= other;
        }

        /**
         * @brief Multiply with scalar and assign
         *
         * The computation is done in-place. @f[
         *      c \cdot t = ta + itb
         * @f]
         */
        inline Complex<T>& operator*=(T scalar) {
            _real *= scalar;
            _imaginary *= scalar;
            return *this;
        }

        /**
         * @brief Multiply with scalar
         *
         * @see operator*=(T)
         */
        inline Complex<T> operator*(T scalar) const {
            return Complex<T>(*this) *= scalar;
        }

        /**
         * @brief Divide with scalar and assign
         *
         * The computation is done in-place. @f[
         *      \frac c t = \frac a t + i \frac b t
         * @f]
         */
        inline Complex<T>& operator/=(T scalar) {
            _real /= scalar;
            _imaginary /= scalar;
            return *this;
        }

        /**
         * @brief Divide with scalar
         *
         * @see operator/=(T)
         */
        inline Complex<T> operator/(T scalar) const {
            return Complex<T>(*this) /= scalar;
        }

        /**
         * @brief Multiply with complex number
         *
         * @f[
         *      c_0 c_1 = (a_0 + ib_0)(a_1 + ib_1) = (a_0 a_1 - b_0 b_1) + i(a_1 b_0 + a_0 b_1)
         * @f]
         */
        inline Complex<T> operator*(const Complex<T>& other) const {
            return {_real*other._real - _imaginary*other._imaginary,
                    _imaginary*other._real + _real*other._imaginary};
        }

        /**
         * @brief Dot product of the complex number
         *
         * Should be used instead of length() for comparing complex number length
         * with other values, because it doesn't compute the square root. @f[
         *      c \cdot c = a^2 + b^2
         * @f]
         * @see dot(const Complex&, const Complex&)
         */
        inline T dot() const {
            return dot(*this, *this);
        }

        /**
         * @brief %Complex number length
         *
         * See also dot() const which is faster for comparing length with other
         * values. @f[
         *      |c| = \sqrt{c \cdot c}
         * @f]
         */
        inline T length() const {
            return std::hypot(_real, _imaginary);
        }

        /** @brief Normalized complex number (of unit length) */
        inline Complex<T> normalized() const {
            return (*this)/length();
        }

        /**
         * @brief Conjugated complex number
         *
         * @f[
         *      c^* = a - ib
         * @f]
         */
        inline Complex<T> conjugated() const {
            return {_real, -_imaginary};
        }

        /**
         * @brief Inverted complex number
         *
         * See invertedNormalized() which is faster for normalized
         * complex numbers. @f[
         *      c^{-1} = \frac{c^*}{|c|^2} = \frac{c^*}{c \cdot c}
         * @f]
         */
        inline Complex<T> inverted() const {
            return conjugated()/dot();
        }

        /**
         * @brief Inverted normalized complex number
         *
         * Equivalent to conjugated(). Expects that the complex number is
         * normalized. @f[
         *      c^{-1} = \frac{c^*}{c \cdot c} = c^*
         * @f]
         * @see inverted()
         */
        inline Complex<T> invertedNormalized() const {
            CORRADE_ASSERT(MathTypeTraits<T>::equals(dot(), T(1)),
                           "Math::Complex::invertedNormalized(): complex number must be normalized",
                           Complex<T>(std::numeric_limits<T>::quiet_NaN(), {}));
            return conjugated();
        }

        /**
         * @brief Rotate vector with complex number
         *
         * @f[
         *      v' = c v = c (v_x + iv_y)
         * @f]
         * @see Complex(const Vector2&), operator Vector2(), Matrix3::transformVector()
         */
        inline Vector2<T> transformVector(const Vector2<T>& vector) const {
            return Vector2<T>((*this)*Complex<T>(vector));
        }

    private:
        T _real, _imaginary;
};

/** @relates Complex
@brief Multiply scalar with complex

Same as Complex::operator*(T) const.
*/
template<class T> inline Complex<T> operator*(T scalar, const Complex<T>& complex) {
    return complex*scalar;
}

/** @relates Complex
@brief Divide complex with number and invert

@f[
    \frac t c = \frac t a + i \frac t b
@f]
@see Complex::operator/()
*/
template<class T> inline Complex<T> operator/(T scalar, const Complex<T>& complex) {
    return {scalar/complex.real(), scalar/complex.imaginary()};
}

/** @debugoperator{Magnum::Math::Complex} */
template<class T> Corrade::Utility::Debug operator<<(Corrade::Utility::Debug debug, const Complex<T>& value) {
    debug << "Complex(";
    debug.setFlag(Corrade::Utility::Debug::SpaceAfterEachValue, false);
    debug << value.real() << ", " << value.imaginary() << ")";
    debug.setFlag(Corrade::Utility::Debug::SpaceAfterEachValue, true);
    return debug;
}

/* Explicit instantiation for commonly used types */
#ifndef DOXYGEN_GENERATING_OUTPUT
extern template Corrade::Utility::Debug MAGNUM_EXPORT operator<<(Corrade::Utility::Debug, const Complex<Float>&);
#ifndef MAGNUM_TARGET_GLES
extern template Corrade::Utility::Debug MAGNUM_EXPORT operator<<(Corrade::Utility::Debug, const Complex<Double>&);
#endif
#endif

}}

#endif
