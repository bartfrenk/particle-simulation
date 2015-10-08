#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <cstdlib>
#include <stdexcept>
#include <list>
#include <queue>
#include <vector>
#include <cassert>
#include <cstdio>
#include <limits>

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
    Simulation(const size_t count, Generator<Particle<d, T>*> &fn,
               const tick_t update_dt, const Box<d, T>& box);
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

    const Box<d, T> &box;

    void step(const tick_t dt);

    void resolve(UpdatePosition * const event);
    void resolve(WallCollision * const event);

    void schedule(Event * const event);

    WallCollision *get_first_wall_collision(const size_t p_index);
};

template <const dim_t d, typename T>
Simulation<d, T>::Simulation(const size_t count, Generator<Particle<d, T>*> &fn,
                             const tick_t update_dt, const Box<d, T>& box)
    : n(count), update_dt(update_dt), box(box)
{
    now = 0;
    ps = new Particle<d, T>*[n];
    for (size_t i = 0; i < n; ++i) {
        if (!fn.empty()) ps[i] = fn.get();
        else throw std::invalid_argument("Cannot generate enough particles");
    }

    for (size_t i = 0; i < n; ++i) {
        WallCollision *collision = get_first_wall_collision(i);
        schedule(collision);
    }

    schedule(new UpdatePosition(update_dt));
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
    pending.pop();

    switch (current->get_type()) {
    case UPDATE_POSITION:
        std::cout << "\nUPDATE_POSITION\n";
        resolve(static_cast<UpdatePosition*> (current));
        break;
    case PARTICLE_COLLISION:
        break;
    case WALL_COLLISION:
        std::cout << "\nWALL_COLLISION\n";
        resolve(static_cast<WallCollision*> (current));
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
}

template <const dim_t d, typename T>
bool Simulation<d, T>::notify() const {
    bool result = false;
    for (typename ObserverList::const_iterator it = observers.begin(); it != observers.end();
         ++it) {

        result |= (*it)->update(now, ps, n);
    }
    return result;
}

template <const dim_t d, typename T>
void Simulation<d, T>::resolve(UpdatePosition * const event) {
    assert (event->time > now);
    tick_t dt = event->time - now;
    for (size_t i = 0; i < n; ++i) {
        ps[i]->move(dt);
    }
    now = event->time;
    delete event;
    schedule(new UpdatePosition(now + update_dt));
}

template <const dim_t d, typename T>
void Simulation<d, T>::resolve(WallCollision * const event) {
    if (event->p_count < ps[event->p_index]->get_counter()) {
        delete event;
        return;
    }
    ps[event->p_index]->bounce(event->wall);
    schedule(get_first_wall_collision(event->p_index));
    delete event;
}

/// Assume that all particles are positioned within box
template <const dim_t d, typename T>
WallCollision * Simulation<d, T>::get_first_wall_collision(const size_t p_index) {
    Particle<d, T> &p = *ps[p_index];
    tick_t t;
    tick_t earliest = std::numeric_limits<tick_t>::max();
    dim_t wall;
    for (int i = 0; i < d; ++i) {
        t = p.time_to_collide(i, box[2 * i]);
        if (t >= 0 && t <= earliest) {
            earliest = t;
            wall = i;
        }
        t = p.time_to_collide(i, box[2 * i + 1]);
        if (t >= 0 && t <= earliest) {
            earliest = t;
            wall = i;
        }
    }
    return new WallCollision(now + earliest, wall, p_index, ps[p_index]->get_counter());
}

template <const dim_t d, typename T>
void Simulation<d, T>::schedule(Event * const event) {
    std::cout << "Scheduled event of type " << event->get_type() << " at time " << event->time
              << "\n";
    pending.push(event);
}

#endif
