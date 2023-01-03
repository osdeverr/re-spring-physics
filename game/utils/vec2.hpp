#pragma once

/**
 * An extremely fast implementation of two-dimensional
 * vector.
 *
 * Authored:                by KingCakeTheFruity
 * Vectorization:           by AlexRoar
 * Debug and inlining help: by JakMobius
 */

#include <cmath>
#include <iostream>
#include "matrix3.hpp"

#ifdef _MSC_VER
#include <glm/vec2.hpp>
#endif

template<typename T>
struct PackedVec2;

template<typename T>
struct Vec2 {
#ifdef _MSC_VER
    typedef glm::vec<2, T, glm::qualifier::packed_highp> content2;
#else
    typedef T content2 __attribute__((ext_vector_type(2)));
#endif

    union {
        content2 m_content;
        struct {
            T x, y;
        };
    };

    Vec2<T>(): m_content {0, 0} {};

    Vec2<T>(T x, T y): m_content {x, y} {};

    explicit Vec2<T>(content2 newContent): m_content (newContent) {}

    Vec2<T>(T x_, T y_, T z_): m_content {x_, y_, z_} {}

    inline T len() const { return sqrt(len_squared()); }

    inline T len_squared() const {
        const auto squared = m_content * m_content;
        return squared[0] + squared[1];
    }

    inline Vec2<T> normal() const {
        T l = len();
        if(l < 1e-5) return {0, 0};
        else return {m_content / l};
    }

    inline void normalize() {
        T l = len();
        if(l < 1e-5) m_content = {0, 0};
        else m_content /= l;
    }

    [[nodiscard]] inline bool is_zero() const {
        const auto squared = (m_content * m_content) < 1e-5;
        return squared[0] * squared[1];
    }

    inline T dot(const Vec2<T> &other) const {
        const auto res = (m_content * other.m_content);
        return res[0] + res[1];
    }

    void set(int index, T y) { m_content[index] = y; }

    T operator[](const int i) const { return m_content[i]; }

    inline Vec2<T> operator+() { return *this; }

    inline Vec2<T> operator-() const { return Vec2 {m_content * T{-1}}; }

    inline Vec2<T> operator+(const Vec2<T> &second) const { return Vec2 {m_content + second.m_content}; }

    inline Vec2<T> operator-(const Vec2<T> &second) const { return Vec2 {m_content - second.m_content}; }

    inline Vec2<T> operator*(const Vec2<T> &second) const { return Vec2 {m_content * second.m_content}; }

    inline Vec2<T> operator/(const Vec2<T> &second) const { return Vec2 {m_content / second.m_content}; }

    inline Vec2<T> operator*(const T k) const { return Vec2<T> {m_content * k }; }

    inline Vec2<T> operator/(T k) const { return Vec2 {m_content / k}; }

    inline Vec2<T> &operator+=(const Vec2<T> &second) { m_content += second.m_content; return *this; }

    inline Vec2<T> &operator-=(const Vec2<T> &second) { m_content -= second.m_content; return *this; }

    inline Vec2<T> &operator*=(const Vec2<T> &second) { m_content *= second.m_content; return *this; }

    inline Vec2<T> &operator*=(const T k) { m_content *= k; return *this; }

    inline Vec2<T> &operator/=(const Vec2<T> &second) { m_content /= second.m_content; return *this; }

    inline Vec2<T> &operator/=(const T k) { m_content /= k; return *this; }


    void transform_unbound(const Matrix3<T> &matrix) {
        m_content = {
            matrix.transform_x(m_content[0], m_content[1], 0),
            matrix.transform_y(m_content[0], m_content[1], 0)
        };
    }

    inline Vec2<T> &operator*=(const Matrix3<T> &other) {

        m_content = {
            other.transform_x(m_content[0], m_content[1]),
            other.transform_y(m_content[0], m_content[1])
        };

        return *this;
    }

    inline bool operator==(const Vec2<T> &second) const {
        const auto res = m_content - second.m_content;
        return abs(res[0]) < 1e-5 && abs(res[1]) < 1e-5;
    }
    inline bool operator!=(const Vec2<T> other) const { return !(*this == other); }

    template<typename V>
    inline explicit operator Vec2<V>() const {
        return Vec2<V> {(V)m_content[0], (V) m_content[1] };
    }

    template<typename V>
    inline explicit operator PackedVec2<V>() const {
        return PackedVec2<V> {(V)m_content[0], (V) m_content[1] };
    }
};

typedef Vec2<double> Vec2d;
typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;

template<typename T>
struct PackedVec2 {
    T x, y;
};

typedef PackedVec2<double> PackedVec2d;
typedef PackedVec2<float> PackedVec2f;