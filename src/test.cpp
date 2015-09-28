#include <iostream>
#include <cstdlib>

#include "particle.hpp"
#include "generator.hpp"
#include "simulation.hpp"
#include "observer.hpp"

using std::cout;
using std::endl;

void test_simulation() {
    Getter<int> vectors(std::rand);
    RandomParticleGenerator<3, int> fn(1.0, 1.0, vectors);
    Simulation<3, int> s(3, fn, 1);
    StreamWriter<3, int> observer(cout);
    s.subscribe(observer);
    s.next();
    s.notify();
    s.next();
    s.notify();
}

int main() {
    srand(time(NULL));
    test_simulation();

    return 0;

}
