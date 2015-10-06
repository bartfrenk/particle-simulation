#include "utils.hpp"
#include <iostream>

int bound(const int value, const int lo, const int hi) {
    return (value % (hi - lo)) + lo;
}

double bound(const double value, const double lo, const double hi) {
    return fmod(value, (hi - lo)) + lo;
}
