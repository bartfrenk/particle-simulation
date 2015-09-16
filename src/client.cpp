#include <iostream>
#include <gtkmm/application.h>
#include <gtkmm/window.h>

#include "event.hpp"
#include "canvas.hpp"
#include "simulation.hpp"


int main(int argc, char** argv) {
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "parsim");

    Gtk::Window window;
    window.set_title("Particle simulation");

    Canvas canvas;
    window.add(canvas);

    Space<2, double> space;
    space.walls[0].lo = 0;
    space.walls[0].hi = 1;
    space.walls[1].lo = 0;
    space.walls[1].hi = 1;

    double *mass = new double[2];
    mass[0] = 1; mass[1] = 1;

    double *radius = new double[2];
    radius[0] = 0.1; radius[1] = 0.1;

    Simulation<2, double> simulation(space, 2, mass, radius);
    simulation.set_canvas(canvas);
    canvas.show();

    return app->run(window);
}

int test() {
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
