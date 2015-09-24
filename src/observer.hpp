#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include "simulation.hpp"
#include <iostream>

using std::ostream;
using std::endl;

template <const dim_t d, typename T>
class StreamWriter : public Observer<d, T> {
public:
    StreamWriter(ostream& output) : os(output) {};
    virtual bool update(const size_t count, Particle<d, T>** ps);
private:
    ostream &os;
};

template <const dim_t d, typename T>
bool StreamWriter<d, T>::update(const size_t count, Particle<d, T>** ps) {
    for (size_t i = 0; i < count; ++i) {
        os << *ps[i] << endl;
    }
    os << endl;
    return true;
}
#endif
