#ifndef EVENT_H
#define EVENT_H

#include <cstdlib>
#include "share.hpp"

enum EventType {UPDATE_POSITION,
                PARTICLE_COLLISION,
                WALL_COLLISION};

struct Event {
    const tick_t time;

    Event(const tick_t time) : time(time) {}
    virtual ~Event() {};
    virtual EventType get_type() const = 0;
};

struct UpdatePosition : Event {
    UpdatePosition(const tick_t time) : Event(time) {}
    virtual ~UpdatePosition() {};
    virtual EventType get_type() const { return UPDATE_POSITION; }
};

struct WallCollision : Event {

    const dim_t wall;
    const size_t p_index;
    const unsigned int p_count;

    WallCollision(const tick_t time, const dim_t wall, const size_t p_index,
                  const unsigned int p_count)
        : Event(time), wall(wall), p_index(p_index), p_count(p_count) {}

    virtual ~WallCollision() {};
    virtual EventType get_type() const { return WALL_COLLISION; }
};

#endif
