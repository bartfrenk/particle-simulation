#ifndef CANVAS_H
#define CANVAS_H

#include <gtkmm/drawingarea.h>
#include "space.hpp"
#include "particle.hpp"

class Canvas : public Gtk::DrawingArea {
public:
    Canvas() {};
    void init(Space<2, double> *space, Particle<2, double> **particles, const int n) {
        m_space = space;
        m_particles = particles;
        m_count = n;
    }
    virtual ~Canvas() {};

protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

private:
    // space this canvas represents
    Space<2, double> *m_space;
    // particles that exist in the space
    Particle<2, double> **m_particles;
    // number of particles
    int m_count;

    void draw_particle(const Cairo::RefPtr<Cairo::Context>& cr, Particle<2, double> *p);
};

#endif // CANVAS_H
