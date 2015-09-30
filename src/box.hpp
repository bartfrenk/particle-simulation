#ifndef BOX_HPP
#define BOX_HPP

#include <cstdarg>

#include "share.hpp"
#include "particle.hpp"

template <typename T>
struct Interval {
    Interval(const T lo, const T hi) : lo(lo), hi(hi) {};
    bool contains(const T x) {
        return (lo <= x) && (x <= hi);
    }
    const T lo;
    const T hi;
};

template <const dim_t d, typename T>
struct Box {
    Box(const T...);
    T walls[2 * d];
};

template <const dim_t d, typename T>
Box<d, T>::Box(const T walls...) {
    va_list args;
    va_start(args, walls);
    T lo, hi;
    for (int i = 0; i < 2 * d; ++i) {
        walls[i] = va_arg(args, T);
    }
}

#endif
