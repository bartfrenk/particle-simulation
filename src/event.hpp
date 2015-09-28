#ifndef EVENT_H
#define EVENT_H

#include "share.hpp"

enum EventType {UPDATE_POSITION,
                PARTICLE_COLLISION,
                WALL_COLLISION};

struct Event {
    const tick_t time;

    Event(const tick_t time) : time(time) {};
    virtual ~Event() {};
    virtual EventType get_type() const = 0;

};

struct UpdatePosition : Event {
    UpdatePosition(const tick_t time) : Event(time) {};
    virtual ~UpdatePosition() {};
    virtual EventType get_type() const { return UPDATE_POSITION; }
};

#endif
