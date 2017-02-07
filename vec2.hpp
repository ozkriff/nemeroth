#ifndef VEC2_HPP
#define VEC2_HPP

#include <cfloat>
#include <cmath>

// TODO: add some tests

template<class T>
struct Vec2 {
    T x;
    T y;

    Vec2()
      : x(0),
        y(0)
    {}

    Vec2(T x, T y)
      : x(x),
        y(y)
    {}

    Vec2 operator+(const Vec2& v) const { return Vec2{x, y} += v; }
    Vec2 operator-(const Vec2& v) const { return Vec2{x, y} -= v; }
    Vec2 operator*(const Vec2& v) const { return Vec2{x, y} *= v; }
    Vec2 operator/(const Vec2& v) const { return Vec2{x, y} /= v; }

    Vec2& operator+=(const Vec2& v) { x += v.x; y += v.y; return *this; }
    Vec2& operator-=(const Vec2& v) { x -= v.x; y -= v.y; return *this; }
    Vec2& operator*=(const Vec2& v) { x *= v.x; y *= v.y; return *this; }
    Vec2& operator/=(const Vec2& v) { x /= v.x; y /= v.y; return *this; }

    Vec2 operator*(const T& s) const { return Vec2{x, y} *= s; }
    Vec2 operator/(const T& s) const { return Vec2{x, y} /= s; }

    Vec2& operator*=(const T& s) { x *= s; y *= s; return *this; }
    Vec2& operator/=(const T& s) { x /= s; y /= s; return *this; }

    friend bool operator==(const Vec2& a, const Vec2& b) {
        return (a.x == b.x) && (a.y == b.y);
    }

    friend bool operator!=(const Vec2& a, const Vec2& b) {
        return !(a == b);
    }

    friend bool operator>(const Vec2& a, const Vec2& b) {
        return euclidean_norm(a) > euclidean_norm(b);
    }

    friend bool operator<(const Vec2& a, const Vec2& b) {
        return b > a;
    }

    friend bool operator>=(const Vec2& a, const Vec2& b) {
        return !(a < b);
    }

    friend bool operator<=(const Vec2& a, const Vec2& b) {
        return !(a > b);
    }

    Vec2 operator-() const {
        return Vec2{-x, -y};
    }
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;

Vec2f to_float(const Vec2i& v);
Vec2i to_int(const Vec2f& v);

template<class T>
T dot_product(const Vec2<T>& a, const Vec2<T>& b) {
    return (a.x * b.x) + (a.y * b.y);
}

template<class T>
T cross_product(const Vec2<T>& a, const Vec2<T>& b) {
    return (a.x * b.y) - (a.y * b.x);
}

template<class T>
T euclidean_norm(const Vec2<T>& v) {
    return sqrt((v.x * v.x) + (v.y * v.y));
}

template<class T>
Vec2<T> normal(const Vec2<T>& v) {
    const T magnitude = euclidean_norm(v);
    return Vec2<T>{v.x / magnitude, v.y / magnitude};
}

template<class T>
Vec2<T> perpendicular(const Vec2<T>& v) {
    return Vec2<T>{v.y, -v.x};
}

template<class T>
bool is_intersecting(const Vec2<T>& aa, const Vec2<T>& ab, const Vec2<T>& ba, const Vec2<T>& bb) {
    const Vec2<T> p = aa;
    const Vec2<T> r = ab - aa;
    const Vec2<T> q = ba;
    const Vec2<T> s = bb - ba;
    const T t = cross_product(q - p, s) / cross_product(r, s);
    const T u = cross_product(q - p, r) / cross_product(r, s);
    return 0.0 <= t && t <= 1.0 && 0.0 <= u && u <= 1.0;
}

template<class T>
Vec2<T> intersection_point(const Vec2<T>& aa, const Vec2<T>& ab, const Vec2<T>& ba, const Vec2<T>& bb) {
    const T x = (aa.x * ab.y - aa.y * ab.x) * (ba.x - bb.x)
        - (ba.x * bb.y - ba.y * bb.x) * (aa.x - ab.x);
    const T y = (aa.x * ab.y - aa.y * ab.x) * (ba.y - bb.y)
        - (ba.x * bb.y - ba.y * bb.x)*(aa.y - ab.y);
    const T denominator = (aa.x - ab.x) * (ba.y - bb.y)
        - (aa.y - ab.y) * (ba.x - bb.x);
    return Vec2<T>{x / denominator, y / denominator};
}

#endif
