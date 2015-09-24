#ifndef GENERATOR_H
#define GENERATOR_H

template <typename T>
class Generator {
public:
    virtual T get() = 0;
    virtual bool empty() const = 0;
};

template <typename T>
class Getter : public Generator<T> {
public:
    Getter(T (*getter)()) : fn(getter) {};
    virtual T get() { return fn(); }
    virtual bool empty() const { return false; }
private:
    T (* const fn)();
};

#endif
