#include "vec2.hpp"

Vec2f to_float(const Vec2i& v) {
    return Vec2f {
        static_cast<float>(v.x),
        static_cast<float>(v.y),
    };
}

Vec2i to_int(const Vec2f& v) {
    return Vec2i {
        static_cast<int>(v.x),
        static_cast<int>(v.y),
    };
}

