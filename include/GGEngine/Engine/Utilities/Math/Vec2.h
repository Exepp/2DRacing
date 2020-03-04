#pragma once
#include <GGEngine/Engine/Utilities/Math/MathUtilities.h>


template <class T>
struct Vec2 {
    using Val_t = T;

    template <typename U>
    explicit Vec2(const Vec2<U>& vec);

    Vec2(T x, T y);

    Vec2();

    Vec2<T>& normalize();

    Vec2<T>& clamp(const Vec2<T> minValues, const Vec2<T> maxValues);

    T length() const;

    Vec2<T> normalized() const;

public:
    T x;

    T y;

    static const Vec2<T> ZeroVector;

    static const Vec2<T> OneVector;
};

template <class T>
const Vec2<T> Vec2<T>::ZeroVector = Vec2<T>(0, 0);
template <class T>
const Vec2<T> Vec2<T>::OneVector = Vec2<T>(1, 1);

typedef Vec2<size_t> Vec2u;

typedef Vec2<ptrdiff_t> Vec2i;

typedef Vec2<float_t> Vec2f;

typedef Vec2<double_t> Vec2d;

// utilities

template <class T>
float_t dotProduct(const Vec2<T>& vec1, const Vec2<T>& vec2);

template <class T>
float_t crossProduct(const Vec2<T>& vec1, const Vec2<T>& vec2);


template <class T>
float_t angleBetweenVectors(Vec2<T> vec1, Vec2<T> vec2);

template <class T>
inline Vec2<T>& scaleVectorByVector(Vec2<T>& scaled, const Vec2<T>& scale)
{
    scaled.x *= scale.x;
    scaled.y *= scale.y;
    return scaled;
}

template <class T>
inline Vec2<T> VectorScaledByVector(Vec2<T> scaled, const Vec2<T>& scale)
{
    return scaleVectorByVector(scaled, scale);
}

template <class T, class U>
inline T asVec(const U& vec)
{
    using ComponentType = decltype(T::x);

    return T(ComponentType(vec.x), ComponentType(vec.y));
}


namespace std {
template <class T>
struct hash<Vec2<T>> {
    size_t operator()(const Vec2<T>& vec) const { return hash_combine(std::hash<T>()(vec.x), std::hash<T>()(vec.y)); }
};
} // namespace std

#include "Vec2.inl"