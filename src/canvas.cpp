#include "canvas.hpp"

bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    cr->scale(width, height);
    cr->save();
    cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->paint();
    cr->restore();

    for (int i = 0; i < m_count; i++) {
        draw_particle(cr, m_particles[i]);
    }

    return true;
}

void Canvas::draw_particle(const Cairo::RefPtr<Cairo::Context>& cr, Particle<2, double>* p) {
    cr->arc(p->x[0], p->x[1], p->radius, 0, 2 * M_PI);
    cr->save();
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->fill_preserve();
    cr->restore();
}
