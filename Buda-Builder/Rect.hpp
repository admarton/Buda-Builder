#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

struct Rect {
	float top, bottom, left, right;
};

inline bool intersect(Rect a, Rect b) {
    return (
        a.left <= b.right &&
        a.right >= b.left &&
        a.bottom <= b.top &&
        a.top >= b.bottom
        );
}

inline Rect operator+(const Rect& lhs, const glm::vec2& rhs) {
    return {
    lhs.top + rhs.x,
    lhs.bottom + rhs.x,
    lhs.left + rhs.y,
    lhs.right + rhs.y,
    };
}

inline Rect operator*(const Rect& lhs, const glm::vec2& rhs) {
    return {
    lhs.top * rhs.x,
    lhs.bottom * rhs.x,
    lhs.left * rhs.y,
    lhs.right * rhs.y,
    };
}