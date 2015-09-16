#ifndef SPACE_HPP
#define SPACE_HPP

template <int d, class T>
struct Space {
    struct Bounds {
        T lo;
        T hi;
    };
    Bounds walls[d];
};

#endif
