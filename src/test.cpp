#include <iostream>
#include <cstdlib>

#include "particle.hpp"
#include "generator.hpp"
#include "simulation.hpp"
#include "observer.hpp"
#include "box.hpp"

using std::cout;
using std::endl;

typedef double coord_t;

const double MAX = 10;
const double MIN = 0;

double rand_double() {
    double x = (double) rand() / RAND_MAX;
    return MIN + x * (MAX - MIN);
}

void test_simulation() {
    Getter<coord_t> vectors(rand_double);
    ParticleGenerator<3, coord_t> fn(1.0, 1.0, vectors);

    coord_t pos_bounds[] = {0, 10, 0, 10, 0, 10};
    fn.bound_position(new Box<3, coord_t>(pos_bounds));

    coord_t vel_bounds[] = {-1, 1, -1, 1, -1, 1};
    fn.bound_velocity(new Box<3, coord_t>(vel_bounds));

    Simulation<3, coord_t> s(3, fn, 1, pos_bounds);
    StreamWriter<3, coord_t> observer(cout);
    s.subscribe(observer);

    do {
        s.notify();
        s.next();
    } while (std::cin.get() == '\n');
}

void test_box() {
}

int main() {
    srand(time(NULL));
    test_simulation();

    return 0;

}
