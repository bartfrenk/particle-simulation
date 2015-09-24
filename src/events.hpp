#ifndef EVENTS_H
#define EVENTS_H

#include "particle.hpp"
#include "share.hpp"

class Event {
public:
    /// Instantiates event taking place on time t.
    Event(const tick_t time) : t(time) {};
    /// Returns the time at which the event takes place.
    tick_t get_time() const {
        return t;
    }
    /// Compares events by the time at which they take place.
    bool operator<(const Event& other) {
        return t < other.t;
    }
    /// Resolves the event;
    virtual bool resolve() = 0;
private:
    /// Keeps the time of the event.
    const tick_t t;
};

/// Two particles colliding with each other.
template <const dim_t d, typename T>
class ParticleCollision : Event {
public:
    ParticleCollision(const tick_t t, Particle<d, T> &p, Particle<d, T> &q)
        : Event(t), p(p), q(q), counter_p(p.get_counter()), counter_q(q.get_counter()) {};
    virtual bool resolve();
private:
    Particle<d, T> &p;
    Particle<d, T> &q;
    const unsigned int counter_p;
    const unsigned int counter_q;
};

/// Particle colliding with a wall.
template <const dim_t d, typename T>
class WallCollision : Event {
public:
    WallCollision(const tick_t t, Particle<d, T> &p, const dim_t wall)
        : Event(t), p(p), wall(wall), counter_p(p.get_counter()) {};
    virtual bool resolve();
private:
    Particle<d, T> &p;
    const dim_t wall;
    const unsigned int counter_p;
};

/// Event indicating that particle positions should be updated.
class UpdatePosition : public Event {
public:
    UpdatePosition(const tick_t t);
};

template <const dim_t d, typename T>
bool ParticleCollision<d, T>::resolve() {
    if (p.get_counter() == counter_p && q.get_counter() == counter_q) {
        p.bounce(q);
        return true;
    } else {
        return false;
    }
};

template <const dim_t d, typename T>
bool WallCollision<d, T>::resolve() {
    if (p.get_counter() == counter_p) {
        p.bounce(wall);
        return true;
    } else {
        return false;
    }
}

#endif
