#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>

template <typename T>
std::ostream operator<<(std::ostream &os, std::vector<T> w) {
    typename std::vector<T>::const_iterator i = w.begin();
    os << "(";
    if (i != w.end()) {
        os << (*i);
        ++i;
    }
    for (; i != w.end(); ++i) {
        os << ", " << (*i);
    }
    os << ")";
};

#endif
