#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include "simulation.hpp"
#include <iostream>

using std::cout;
using std::endl;

template <const dim_t d, typename T>
class ConsoleWriter : public Observer<d, T> {
public:
    virtual bool update(const size_t count, Particle<d, T>** ps) {
        for (size_t i = 0; i < count; ++i) {
            cout << *ps[i] << endl;
        }
        cout << endl;
        return true;
    }
};

#endif
