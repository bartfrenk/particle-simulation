#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <cstdlib>

#include "space.hpp"
#include "particle.hpp"

template <int d, class T>
class Simulation {
public:
    Simulation(const Space<d, T> space, const unsigned int n, double *mass, double *radius) : space(space) {
        particles = new Particle<d, T>[n];
        for (int i = 0; i < d; ++i) {
            particles[i] = createRandomParticle(mass[i], radius[i]);
        }
    }
    void step(const T dt) {};
private:
    Particle<d, T>* createRandomParticle(const double mass, const double radius);
    const Space<d, T> space;
    Particle<d, T> ** const particles;
    T now;
};

template <int d, class T>
Particle<d, T>* Simulation<d, T>::createRandomParticle(const double mass, const double radius) {
    Particle<d, T> *p = new Particle<d, T>(mass, radius);
    for (int i = 0; i < d; ++i) {
        p->setPosition(i, std::rand());
        p->setSpeed(i, std::rand());
    }
}

#endif
