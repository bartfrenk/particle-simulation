#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <cstdlib>
#include <stdexcept>
#include <list>
#include <queue>
#include <vector>

#include "space.hpp"
#include "particle.hpp"
#include "generator.hpp"
#include "events.hpp"

template <const dim_t d, typename T>
class Observer {
public:
    // TODO: specify in signature that observers cannot change ps
    virtual bool update(const size_t count, Particle<d, T>** ps) = 0;
};

template <const dim_t d, typename T>
class Simulation {
public:
    Simulation(const size_t count, Generator<Particle<d, T>*> &fn, const tick_t step_size);
    ~Simulation();

    /// resolves the next event
    bool next();
    void subscribe(Observer<d, T> &observer);
    bool notify() const;

private:

    Particle<d, T> **ps;
    const size_t n;

    const tick_t dt_step;

    typedef std::list<Observer<d, T>*> ObserverList;
    ObserverList observers;

    std::priority_queue<Event<d, T>*, std::vector<Event<d, T>*>, EventCompare<d, T> > future;

    tick_t time;

    void step(const tick_t dt);

    void resolveParticleCollision(const tick_t t, const size_t index_p, const size_t index_q);
    void resolveWallCollision(const tick_t, const size_t index_p, const dim_t wall);
    void resolveUpdatePosition(const tick_t);
};

template <const dim_t d, typename T>
Simulation<d, T>::Simulation(const size_t count, Generator<Particle<d, T>*> &fn,
                             const tick_t step_size) : n(count), dt_step(step_size)
{
    time = 0;
    ps = new Particle<d, T>*[n];
    for (size_t i = 0; i < n; ++i) {
        if (!fn.empty()) ps[i] = fn.get();
        else throw std::invalid_argument("Cannot generate enough particles");
    }

    Event<d, T>::set_context(ps);
    UpdatePosition<d, T>::set_callback(this->resolveUpdatePosition);
}

template <const dim_t d, typename T>
Simulation<d, T>::~Simulation() {
    for (size_t i = 0; i < n; ++i) {
        delete ps[i];
    }
    delete[] ps;
}

template <const dim_t d, typename T>
bool Simulation<d, T>::next() {
    if (future.empty()) return false;

    Event<d, T>* current = future.top();
    current->resolve();

    return true;
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

template <const dim_t d, typename T>
bool Simulation<d, T>::notify() const {
    bool result = false;
    for (typename ObserverList::const_iterator it = observers.begin(); it != observers.end(); ++it) {
        result |= (*it)->update(n, ps);
    }
    return result;
}

template <const dim_t d, typename T>
void Simulation<d, T>::resolveUpdatePosition(const tick_t t) {
    assert(t > time);
    tick_t dt = time - t;
    for (int i = 0; i < n; ++i) {
        ps[i]->move(dt);
    }
    time = t;
    future.push(new UpdatePosition<d, T>(time + dt_step));
}

#endif
