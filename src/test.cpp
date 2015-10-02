#include <iostream>
#include <cstdlib>

#include "particle.hpp"
#include "generator.hpp"
#include "simulation.hpp"
#include "observer.hpp"
#include "box.hpp"

using std::cout;
using std::endl;

void test_simulation() {
    Getter<int> vectors(std::rand);
    ParticleGenerator<3, int> fn(1.0, 1.0, vectors);
    fn.set_position_bounds(new Box<3, int>(3, 0, 10, 0, 10, 0, 10));
    Simulation<3, int> s(3, fn, 1);
    StreamWriter<3, int> observer(cout);
    s.subscribe(observer);
    s.next();
    s.notify();
    s.next();
    s.notify();
}

void test_box() {
    Box<3, int> box(1, 2, 1, 2, 1, 2);
}

int main() {
    srand(time(NULL));
    test_simulation();

    return 0;

}
