#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <vector>
#include <cstdarg>
#include <iostream>

// Precondition: d > 0
template <int d, class T>
class Particle {
public:
    Particle(const double mass, const double radius) : mass(mass), radius(radius) {
        x = new double[d];
        v = new double[d];
        collisionCount = 0;
    };

    ~Particle();

    void setPosition(const T *y);
    void setSpeed(const T *w);
    void setSpeed(const int i, const T wi);
    void setPosition(const int i, const T xi);
    T getSpeed(const int i);
    void move(const T dt);

    int getCollisionCount() const {
        return collisionCount;
    }
    void incCollisionCount() {
        ++collisionCount;
    }

    void print(std::ostream &os) const;

public:
    const double mass;
    const double radius;
    unsigned int collisionCount;
    T *x;
    T *v;
};

template <int d, class T>
Particle<d, T>::~Particle() {
    delete[] x;
    delete[] v;
}

template <int d, class T>
void Particle<d, T>::setPosition(const T *y) {
    for (int i = 0; i < d; ++i) {
        x[i] = y[i];
    }
}

template <int d, class T>
inline void Particle<d, T>::setPosition(const int i, const T xi) {
    x[i] = xi;
}

template <int d, class T>
void Particle<d, T>::setSpeed(const T *w) {
    for (int i = 0; i < d; ++i) {
        v[i] = w[i];
    }
}

template <int d, class T>
inline void Particle<d, T>::setSpeed(const int i, const T vi) {
    v[i] = vi;
}

template <int d, class T>
inline T Particle<d, T>::getSpeed(const int i) {
    return v[i];
}

template <int d, class T>
void Particle<d, T>::move(const T dt) {
    for (int i = 0; i < d; ++i) {
        x[i] += dt * v[i];
    }
}

template <int d, class T>
void print_vector(std::ostream &os, const T *x) {
    os << "(" << x[0];
    for (int i = 1; i < d; ++i)
        os << ", " << x[i];
    os << ")";
}

template <int d, class T>
void Particle<d, T>::print(std::ostream &os) const {
    os << "x = ";
    print_vector<d>(os, x);
    os << "; v = ";
    print_vector<d>(os, v);
    os << "; collisions = " << collisionCount;
}

#endif // PARTICLE_HPP
