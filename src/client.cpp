#include <iostream>
#include "event.hpp"

int main() {
    Particle<2, double> p(10, 4);
    WallCollision<2, double> event(1, &p);
    const double y[] = {1, 2};
    p.setPosition(y);
    p.print(std::cout);
    std::cout << std::endl;
    p.setSpeed(0, -2);
    p.setSpeed(1, 1);
    p.move(1.0);
    p.print(std::cout);
    std::cout << std::endl;
    event.resolve();
    p.print(std::cout);
    std::cout << std::endl;
    return 0;
}
