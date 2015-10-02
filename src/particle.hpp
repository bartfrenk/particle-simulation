#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <iostream>

#include "share.hpp"
#include "generator.hpp"
#include "box.hpp"
#include "utils.hpp"

template <const dim_t d, typename T>
class Particle;

template <const dim_t d, typename T>
std::ostream& operator<<(std::ostream&, const Particle<d, T>&);

template <const dim_t d, typename T>
class ParticleGenerator : public Generator<Particle<d, T>*> {
public:
    ParticleGenerator(const double mass, const double radius, Generator<T> &generator)
        : m(mass), r(radius), fn(generator), x_box(NULL), v_box(NULL) {};

    void set_mass(const double mass) { m = mass; }
    void set_radius(const double radius ) { r = radius; }
    void set_generator(Generator<T> &generator) { fn = generator; }
    void set_position_bounds(const Box<d, T> *bounds) { x_box = bounds; }
    void set_velocity_bounds(const Box<d, T> *bounds) { v_box = bounds; }

    virtual Particle<d, T>* get();
    virtual bool empty() const { return fn.empty(); };
private:
    double m;
    double r;
    Generator<T> &fn;
    const Box<d, T> * x_box;
    Box<d, T> * const v_box;

    T* create_vector();
};

template <const dim_t d, typename T>
class Particle {
public:

    Particle(const double mass, const double radius, T *position, T *speed)
        : m(mass), r(radius), x(position), v(speed), counter(0) {};

    ~Particle() { delete[] x; delete[] v; }

    void bounce(Particle<d, T> &p);
    void bounce(const dim_t wall);
    void move(tick_t dt);

    unsigned int get_counter() const { return counter; }

    friend std::ostream& operator<< <>(std::ostream&, const Particle<d, T>&);

private:
    double m;
    double r;
    T *x;
    T *v;

    unsigned int counter;

    static T* convex(const double a, const T* p, const double b, const T* q);
    static void write(std::ostream &os, const T *w);
};

template <const dim_t d, typename T>
Particle<d, T>* ParticleGenerator<d, T>::get() {
    T* x = create_vector();
    T* v = create_vector();
    if (x_box != NULL) {
        for (int i = 0; i < d; ++i) {
            x[i] = bound(x[i], (*x_box)[2 * i], (*x_box)[2 * i + 1]);
        }
    }
    if (v_box != NULL) {
        for (int i = 0; i < d; ++i) {
            v[i] = bound(v[i], (*v_box)[2 * i], (*v_box)[2 * i + 1]);
        }
    }
    return new Particle<d, T>(m, r, x, v);
}

template <const dim_t d, typename T>
T* ParticleGenerator<d, T>::create_vector() {
    T* u = new T[d];
    for (int i = 0; i < d; ++i) {
        u[i] = fn.get();
    }
    return u;
}

template <const dim_t d, typename T>
void Particle<d, T>::move(const tick_t dt) {
    for (int i = 0; i < d; ++i) {
        x[i] += dt * v[i];
    }
}

template <const dim_t d, typename T>
void Particle<d, T>::bounce(Particle<d, T> &p) {
    double dm = m - p.m;
    T *v1 = convex(dm, v, 2 * p.m, p.v);
    T *v2 = convex(-dm, p.v, 2 * m, v);
    delete[] v;
    delete[] p.v;
    v = v1;
    p.v = v2;
    ++counter;
    ++p.counter;
}

template <const dim_t d, typename T>
void Particle<d, T>::bounce(const dim_t wall) {
    v[wall] = -v[wall];
    ++counter;
}

template <const dim_t d, typename T>
std::ostream& operator<<(std::ostream &os, const Particle<d, T> &p) {
    os << "x = "; Particle<d, T>::write(os, p.x); os << "; ";
    os << "v = "; Particle<d, T>::write(os, p.v); os << "; ";
    os << "m = " << p.m << "; r = " << p.r << "; c = " << p.counter;
    return os;
}

template <const dim_t d, typename T>
void Particle<d, T>::write(std::ostream &os, const T *w) {
    os << "(" << w[0];
    for (int i = 1; i < d; ++i) {
        os << ", " << w[i];
    }
    os << ")";
}

template <const dim_t d, typename T>
T* Particle<d, T>::convex(const double a, const T *p, const double b, const T *q) {
    T *result = new T[d];
    double s = a + b;
    for (int i = 0; i < d; ++i)
        result[i] = (a * p[i] + b * q[i]) / s;
    return result;
}

#endif // PARTICLE_HPP
