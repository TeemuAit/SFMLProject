#pragma once // include guard

// Linear algebra:
core::Vector sub(const core::Vector& lhs, const core::Vector& rhs) {
    assert(lhs.size() == rhs.size());
    auto res = lhs;
    for (size_t i = 0; i < res.size(); ++i) {
        res[i] -= rhs[i];
    }
    return res;
}

float dot(const core::Vector& lhs, const core::Vector& rhs) {
    assert(lhs.size() == rhs.size());
    float res = 0.0f;
    for (size_t i = 0; i < lhs.size(); ++i) {
        res += lhs[i] * rhs[i];
    }
    return res;
}

float length(const core::Vector& vec) {
    return sqrtf(dot(vec, vec));
}

float distance(const core::Vector& p2, const core::Vector& p1) {
    return length(sub(p2, p1));
}
