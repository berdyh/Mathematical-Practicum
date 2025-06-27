#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include <stdexcept>
#include <cmath>

namespace mapra {

template<typename T>
class Vector {
private:
    std::vector<T> elems;

public:
    // Constructors
    Vector() = default;
    explicit Vector(std::size_t size) : elems(size) {}
    Vector(std::size_t size, const T& value) : elems(size, value) {}
    Vector(std::initializer_list<T> init) : elems(init) {}

    // Size operations
    std::size_t size() const { return elems.size(); }
    void resize(std::size_t new_size) { elems.resize(new_size); }
    void resize(std::size_t new_size, const T& value) { elems.resize(new_size, value); }

    // Element access
    T& operator[](std::size_t index) {
        if (index >= elems.size()) throw std::out_of_range("Vector index out of range");
        return elems[index];
    }
    
    const T& operator[](std::size_t index) const {
        if (index >= elems.size()) throw std::out_of_range("Vector index out of range");
        return elems[index];
    }

    // Arithmetic operations
    Vector<T> operator+(const Vector<T>& other) const {
        if (size() != other.size()) throw std::invalid_argument("Vector sizes don't match");
        Vector<T> result(size());
        for (std::size_t i = 0; i < size(); ++i) {
            result[i] = elems[i] + other[i];
        }
        return result;
    }

    Vector<T> operator-(const Vector<T>& other) const {
        if (size() != other.size()) throw std::invalid_argument("Vector sizes don't match");
        Vector<T> result(size());
        for (std::size_t i = 0; i < size(); ++i) {
            result[i] = elems[i] - other[i];
        }
        return result;
    }

    Vector<T> operator*(const T& scalar) const {
        Vector<T> result(size());
        for (std::size_t i = 0; i < size(); ++i) {
            result[i] = elems[i] * scalar;
        }
        return result;
    }

    Vector<T>& operator+=(const Vector<T>& other) {
        if (size() != other.size()) throw std::invalid_argument("Vector sizes don't match");
        for (std::size_t i = 0; i < size(); ++i) {
            elems[i] += other[i];
        }
        return *this;
    }

    Vector<T>& operator-=(const Vector<T>& other) {
        if (size() != other.size()) throw std::invalid_argument("Vector sizes don't match");
        for (std::size_t i = 0; i < size(); ++i) {
            elems[i] -= other[i];
        }
        return *this;
    }

    Vector<T>& operator*=(const T& scalar) {
        for (std::size_t i = 0; i < size(); ++i) {
            elems[i] *= scalar;
        }
        return *this;
    }

    // Norm calculation
    T norm() const {
        T sum = T(0);
        for (const auto& elem : elems) {
            sum += elem * elem;
        }
        return std::sqrt(sum);
    }

    T max_norm() const {
        T max_val = T(0);
        for (const auto& elem : elems) {
            max_val = std::max(max_val, std::abs(elem));
        }
        return max_val;
    }

    // Check if zero vector
    bool is_zero() const {
        for (const auto& elem : elems) {
            if (elem != T(0)) return false;
        }
        return true;
    }
};

// Scalar multiplication from left
template<typename T>
Vector<T> operator*(const T& scalar, const Vector<T>& vec) {
    return vec * scalar;
}

}

#endif
