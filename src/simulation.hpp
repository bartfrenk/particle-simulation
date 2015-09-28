#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <cstdlib>
#include <stdexcept>
#include <list>
#include <queue>
#include <vector>
#include <cassert>

#include "space.hpp"
#include "particle.hpp"
#include "generator.hpp"
#include "event.hpp"
#include "observer.hpp"

class InverseChronological {
public:
    bool operator() (Event *first, Event *second) {
        return first->time > second->time;
    }
};

template <const dim_t d, typename T>
class Simulation {
public:
    Simulation(const size_t count, Generator<Particle<d, T>*> &fn, const tick_t update_dt);
    ~Simulation();

    /// resolves the next event
    bool next();
    void subscribe(Observer<d, T> &observer);
    bool notify() const;

private:

    Particle<d, T> **ps;
    const size_t n;

    const tick_t update_dt;

    typedef std::list<Observer<d, T>*> ObserverList;
    ObserverList observers;

    std::priority_queue<Event*, std::vector<Event*>, InverseChronological> pending;

    tick_t now;

    void step(const tick_t dt);

    void resolve(UpdatePosition * const event);
};

template <const dim_t d, typename T>
Simulation<d, T>::Simulation(const size_t count, Generator<Particle<d, T>*> &fn,
                             const tick_t update_dt) : n(count), update_dt(update_dt)
{
    now = 0;
    ps = new Particle<d, T>*[n];
    for (size_t i = 0; i < n; ++i) {
        if (!fn.empty()) ps[i] = fn.get();
        else throw std::invalid_argument("Cannot generate enough particles");
    }

    pending.push(new UpdatePosition(update_dt));
}

template <const dim_t d, typename T>
Simulation<d, T>::~Simulation() {
    for (size_t i = 0; i < n; ++i) {
        delete ps[i];
    }
    delete[] ps;
    // TODO: delete pending events
}

template <const dim_t d, typename T>
bool Simulation<d, T>::next() {
    if (pending.empty()) return false;

    Event* current = pending.top();
    switch (current->get_type()) {
    case UPDATE_POSITION:
        resolve(static_cast<UpdatePosition*> (current));
        break;
    case PARTICLE_COLLISION:
        break;
    case WALL_COLLISION:
        break;
    }

    return true;
}

template <const dim_t d, typename T>
void Simulation<d, T>::step(const tick_t dt) {
    now += dt;
    for (size_t i = 0; i < n; ++i) {
        ps[i]->move(dt);
    }
}

template <const dim_t d, typename T>
void Simulation<d, T>::subscribe(Observer<d, T> &observer) {
    observers.push_back(&observer);
    observer.update(now, ps, n);
}

template <const dim_t d, typename T>
bool Simulation<d, T>::notify() const {
    bool result = false;
    for (typename ObserverList::const_iterator it = observers.begin(); it != observers.end(); ++it) {
        result |= (*it)->update(now, ps, n);
    }
    return result;
}

template <const dim_t d, typename T>
void Simulation<d, T>::resolve(UpdatePosition * const event) {
    assert (event->time > now);
    tick_t dt = event->time - now;
    for (int i = 0; i < n; ++i) {
        ps[i]->move(dt);
    }
    now = event->time;
    delete event;
    pending.push(new UpdatePosition(now + update_dt));
}

#endif
