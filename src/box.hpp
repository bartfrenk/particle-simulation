#ifndef BOX_HPP
#define BOX_HPP

#include <iostream>
#include <cstdarg>

#include "share.hpp"
#include "particle.hpp"

template <const dim_t d, typename T>
struct Box {
    Box(const T * const walls);
    T operator[](const size_t index) const { return ws[index]; }
private:
    T ws[2 * d];
};

template <const dim_t d, typename T>
Box<d, T>::Box(const T * const walls) {
    for (int i = 0; i < 2 * d; ++i) {
        ws[i] = walls[i];
    }
}

#endif
