#ifndef UTILS_H
#define UTILS_H

#include <cmath>

int bound(const int value, const int lo, const int hi) {
    return (value % (hi - lo)) + lo;
}

int bound(const double value, const double lo, const double hi) {
    return fmod(value, (hi - lo)) + lo;
}

#endif
