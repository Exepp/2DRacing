#pragma once
#include <ECSpp/internal/utility/Assert.h>
#include <cstdint>
#include <iostream>
#include <math.h>


#define PI_F 3.14159265358979323846f

template <class T>
inline T maxValue(const T& value1, const T& value2)
{
    if (value1 > value2)
        return value1;
    return value2;
}

template <class T>
inline T minValue(const T& value1, const T& value2)
{
    if (value1 < value2)
        return value1;
    return value2;
}

template <class T>
inline T clamped(const T& value, const T& min, const T& max)
{
    return minValue(maxValue(value, min), max);
}

template <class T>
inline T& clamp(T& value, const T& min, const T& max)
{
    value = clamped(value, min, max);
    return value;
}

inline size_t hash_combine(size_t lhs, size_t rhs)
{
    lhs ^= rhs + 0xc6a4a7935bd1e995ull + (lhs << 6) + (lhs >> 2);
    return lhs;
}

// returns 1 for value > 0; 0 for value == 0, -1 for value < 0
template <class T>
inline int sign(const T& value)
{
    return (value > 0) - signbit(value);
}

template <class Vec2T>
inline Vec2T signs(const Vec2T& vec)
{
    return Vec2T(sign(vec.x), sign(vec.y));
}

inline float_t toRadians(float_t degrees)
{
    return PI_F / 180.f * degrees;
}

inline float_t toDegrees(float_t radians)
{
    return 180.f * radians / PI_F;
}