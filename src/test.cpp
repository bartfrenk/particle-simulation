#include <iostream>
#include <cstdlib>

#include "particle.hpp"
#include "events.hpp"
#include "generator.hpp"
#include "simulation.hpp"

using std::cout;
using std::endl;

const char * const indent = "\t";

template <const dim_t d, typename T>
Particle<d, T>* create_random_particle() {

}

void test_events() {
    Getter<int> fn = Getter<int>(std::rand);
    Particle<3, int> *p = Particle<3, int>::create_particle(1, 1, fn);
    Particle<3, int> *q = Particle<3, int>::create_particle(2, 2, fn);

    cout << indent << *p << endl
         << indent << *q << endl;

    ParticleCollision<3, int> e(3, *p, *q);
    WallCollision<3, int> f(4, *p, 2);

    cout << e.resolve() << endl;
    cout << indent << *p << endl
         << indent << *q << endl;

    cout << f.resolve() << endl;
    cout << indent << *p << endl;
}

void test_simulation() {
    Getter<int> vectors(std::rand);
    RandomParticleGenerator<3, int> fn(1.0, 1.0, vectors);
    Simulation<3, int> s(3, fn);
    s.step(1);
}

int main() {
    srand(time(NULL));
    test_events();

    return 0;

}
