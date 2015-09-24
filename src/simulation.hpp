#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <cstdlib>
#include <stdexcept>
#include <list>

#include "space.hpp"
#include "particle.hpp"
#include "generator.hpp"

template <const dim_t d, typename T>
class Observer {
public:
    virtual bool update(const size_t count, Particle<d, T>** ps) = 0;
};

template <const dim_t d, typename T>
class Simulation {
public:
    Simulation(const size_t count, Generator<Particle<d, T>*> &fn);
    ~Simulation();

    void step(const tick_t dt);
    void subscribe(Observer<d, T> &observer);

private:

    Particle<d, T> **ps;
    const size_t n;

    std::list<Observer<d, T>*> observers;

    tick_t time;
};

template <const dim_t d, typename T>
Simulation<d, T>::Simulation(const size_t count,
                             Generator<Particle<d, T>*> &fn) : n(count)
{
    time = 0;
    ps = new Particle<d, T>*[n];
    for (size_t i = 0; i < n; ++i) {
        if (!fn.empty()) ps[i] = fn.get();
        else throw std::invalid_argument("Cannot generate enough particles");
    }
}

template <const dim_t d, typename T>
Simulation<d, T>::~Simulation() {
    for (size_t i = 0; i < n; ++i) {
        delete ps[i];
    }
    delete[] ps;
}

template <const dim_t d, typename T>
void Simulation<d, T>::step(const tick_t dt) {
    time += dt;
    for (size_t i = 0; i < n; ++i) {
        ps[i]->move(dt);
    }
}

template <const dim_t d, typename T>
void Simulation<d, T>::subscribe(Observer<d, T> &observer) {
    observers.push_back(&observer);
    observer.update(n, ps);
}

#endif
