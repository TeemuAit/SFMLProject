#pragma once
#include <vector>
#include <functional>

namespace core {
    typedef std::vector<int> Vector;
    typedef std::vector<Vector> Matrix;

    inline Vector vecN(size_t N, int value) {
        Vector res;
        for(size_t i=0; i<N; ++i) {
            res.push_back(value);
        }
        return res;
    }

    inline Matrix matN(size_t N, int value) {
        Matrix res;
        for(size_t i=0; i<N; ++i) {
            res.push_back(Vector());
            for(size_t j=0; j<N; ++j) {
                res[i].push_back(value);
            }
        }
        return res;
    }

    // Red - Green - Blue - Alpha -color.
    struct RGBA {
        RGBA(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 0xff)
            : r(_r), g(_g), b(_b), a(_a) {
        }

        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

}
