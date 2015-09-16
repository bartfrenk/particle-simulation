#ifndef EVENT_HPP
#define EVENT_HPP
#include "particle.hpp"

class Event {
public:
    bool operator<(const Event &other) const;
    virtual bool resolve() const = 0;
protected:
    double t;
};

template <int d, class T>
class WallCollision : public Event {
public:
    WallCollision(const int i, Particle<d, T> *p);
    virtual bool resolve() const;
private:
    Particle<d, T> * const particle;
    int wall;
    int collisionCount;
};

class ParticleCollision : public Event {
public:
};

bool Event::operator<(const Event &other) const {
    return t < other.t;
}

template <int d, class T>
WallCollision<d, T>::WallCollision(const int i, Particle<d, T> *p) : particle(p) {
    wall = i;
    collisionCount = p->getCollisionCount();
}

template <int d, class T>
bool WallCollision<d, T>::resolve() const {
    if (collisionCount == particle->getCollisionCount()) {
        particle->setSpeed(wall, -particle->getSpeed(wall));
        particle->incCollisionCount();
        return true;
    }
    return false;
}

#endif // EVENT_HPP
