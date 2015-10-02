#ifndef BOX_HPP
#define BOX_HPP

#include <cstdarg>

#include "share.hpp"
#include "particle.hpp"
#include <iostream>

template <typename T>
struct Interval {
public:
    Interval(const T lo, const T hi) : lo(lo), hi(hi) {};
    bool contains(const T x) {
        return (lo <= x) && (x <= hi);
    }
    const T lo;
    const T hi;
};

template <const dim_t d, typename T>
struct Box {
    Box(const dim_t dim, ...);
    T operator[](const size_t index) const { return ws[index]; }
private:
    T ws[2 * d];
};

template <const dim_t d, typename T>
Box<d, T>::Box(const dim_t dim, ...) {
    va_list args;
    va_start(args, dim);
    for (int i = 0; i < 2 * d; ++i) {
        ws[i] = va_arg(args, T);
    }
    va_end(args);
}

#endif
