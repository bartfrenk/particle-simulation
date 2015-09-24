#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <iostream>

#include "share.hpp"
#include "generator.hpp"

template <const dim_t d, typename T>
class Particle;

template <const dim_t d, typename T>
std::ostream& operator<<(std::ostream&, const Particle<d, T>&);

template <const dim_t d, typename T>
class RandomParticleGenerator : public Generator<Particle<d, T>*> {
public:
    RandomParticleGenerator(const double mass, const double radius, Generator<T> &fn)
        : m(mass), r(radius), fn(fn) {};

    virtual Particle<d, T>* get() { return Particle<d, T>::create_particle(m, r, fn); };
    virtual bool empty() const { return fn.empty(); };
private:
    const double m;
    const double r;
    Generator<T> &fn;
};

template <const dim_t d, typename T>
class Particle {
public:

    Particle(const double mass, const double radius, T *position, T *speed)
        : m(mass), r(radius), x(position), v(speed), counter(0) {};

    ~Particle() { delete[] x; delete[] v; }

    static Particle<d, T>* create_particle(const double mass, const double radius, Generator<T> &fn);

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
    static T* create_vector(Generator<T> &fn);
    static void write(std::ostream &os, const T *w);
};

template <const dim_t d, typename T>
Particle<d, T>* Particle<d, T>::create_particle(const double mass, const double radius, Generator<T> &fn) {
    T* x = create_vector(fn);
    T* v = create_vector(fn);
    return new Particle<d, T>(mass, radius, x, v);
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
T* Particle<d, T>::create_vector(Generator<T> &fn) {
    T* u = new T[d];
    for (int i = 0; i < d; ++i) {
        u[i] = fn.get();
    }
    return u;
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
