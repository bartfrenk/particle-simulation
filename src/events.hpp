#ifndef EVENTS_H
#define EVENTS_H

#include "particle.hpp"
#include "share.hpp"

template <const dim_t d, typename T>
class Event {
public:
    /// Instantiates event taking place on time t.
    Event(const tick_t time) : t(time) {};

    static void set_context(Particle<d, T> **particles) {
        ps = particles;
    }

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
protected:
    tick_t get_time() { return t; }

    static Particle<d, T> **ps;

private:
    /// Keeps the time of the event.
    const tick_t t;
};

template <const dim_t d, typename T>
class EventCompare {
public:
    bool operator() (Event<d, T>* e, Event<d, T>* f) {
        // invert the order to match that of std::priority_queue
        return (*f) < (*e);
    }
};

/// Two particles colliding with each other.
template <const dim_t d, typename T>
class ParticleCollision : Event<d, T> {
public:
    ParticleCollision(const tick_t t, const size_t index_p, const size_t index_q)
        : Event<d, T>(t),
          i_p(index_p), c_p(Event<d, T>::ps[index_p]->get_counter()),
          i_q(index_q), c_q(Event<d, T>::ps[index_q]->get_counter()) {};

    virtual bool resolve();
private:
    static void (*callback)(const tick_t t, const size_t index_p, const size_t index_q);

    bool is_current() {
        return (Event<d, T>::ps[i_p]->get_counter() == c_p &&
                Event<d, T>::ps[i_q]->get_counter() == c_q);
    };

    const size_t i_p;
    const unsigned int c_p;

    const size_t i_q;
    const unsigned int c_q;
};

/// Particle colliding with a wall.
template <const dim_t d, typename T>
class WallCollision : Event<d, T> {
public:
    WallCollision(const tick_t t, const size_t index_p, const dim_t wall)
        : Event<d, T>(t),
          i_p(index_p), c_p(Event<d, T>::ps[index_p]->get_counter()),
          w(wall) {};

    virtual bool resolve();
private:
    static void (*callback)(const tick_t t, const size_t index_p, const dim_t wall);

    bool is_current() {
        return Event<d, T>::ps[i_p]->get_counter() == c_p;
    }

    const size_t i_p;
    const unsigned int c_p;

    const dim_t w;
};

/// Event indicating that particle positions should be updated.
template <const dim_t d, typename T>
class UpdatePosition : public Event<d, T> {
public:
    UpdatePosition(const tick_t t);
    virtual bool resolve();
    static void set_callback(void (*callback)(const tick_t)) {
        cb = callback;
    }
private:
    static void (*cb)(const tick_t t);
};

template <const dim_t d, typename T>
bool ParticleCollision<d, T>::resolve() {
    if (is_current()) {
        callback(this->get_time(), i_p, i_q);
        return true;
    } else {
        return false;
    }
}

template <const dim_t d, typename T>
bool WallCollision<d, T>::resolve() {
    if (is_current()) {
        callback(this->get_time(), i_p, w);
        return true;
    } else {
        return false;
    }
}

template <const dim_t d, typename T>
bool UpdatePosition<d, T>::resolve() {
    cb(this->get_time());
    return true;
}

#endif
