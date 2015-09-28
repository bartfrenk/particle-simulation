#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <iostream>

#include "share.hpp"
#include "particle.hpp"

using std::ostream;
using std::endl;

template <const dim_t d, typename T>
class Observer {
public:
    // TODO: specify in signature that observers cannot change ps
    virtual bool update(const tick_t time, Particle<d, T>** ps, const size_t count) = 0;
};

template <const dim_t d, typename T>
class StreamWriter : public Observer<d, T> {
public:
    StreamWriter(ostream& output) : os(output) {};
    virtual bool update(const tick_t time, Particle<d, T>** ps, const size_t count);
private:
    ostream &os;
};

template <const dim_t d, typename T>
bool StreamWriter<d, T>::update(const tick_t time, Particle<d, T>** ps, const size_t count) {
    os << "Time: " << time << endl;
    for (size_t i = 0; i < count; ++i) {
        os << "\t" << i << ": " << *ps[i] << endl;
    }
    return true;
}
#endif
