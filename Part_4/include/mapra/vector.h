// Copyright (c) 2022, The MaPra Authors.

#ifndef VECTOR_H_
#define VECTOR_H_

#include <cstddef>
#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

namespace mapra {

template <typename T = double>
class Vector {
 public:
  explicit Vector(std::size_t len = 1) : elems_(len, T(0)) {}

  T& operator()(std::size_t i) {
#ifndef NDEBUG
    if (i >= elems_.size()) {
      VecError("Vector index out of range");
    }
#endif
    return elems_[i];
  }

  T operator()(std::size_t i) const {
#ifndef NDEBUG
    if (i >= elems_.size()) {
      VecError("Vector index out of range");
    }
#endif
    return elems_[i];
  }

  Vector<T>& operator+=(const Vector<T>& other) {
    if (elems_.size() != other.elems_.size()) {
      VecError("Vector size mismatch in +=");
    }
    for (std::size_t i = 0; i < elems_.size(); ++i) {
      elems_[i] += other.elems_[i];
    }
    return *this;
  }

  Vector<T>& operator-=(const Vector<T>& other) {
    if (elems_.size() != other.elems_.size()) {
      VecError("Vector size mismatch in -=");
    }
    for (std::size_t i = 0; i < elems_.size(); ++i) {
      elems_[i] -= other.elems_[i];
    }
    return *this;
  }

  Vector<T>& operator*=(T scalar) {
    for (auto& elem : elems_) {
      elem *= scalar;
    }
    return *this;
  }

  Vector<T>& operator/=(T scalar) {
    if (scalar == T(0)) {
      VecError("Division by zero");
    }
    for (auto& elem : elems_) {
      elem /= scalar;
    }
    return *this;
  }

  Vector<T>& Redim(std::size_t new_size) {
    elems_.resize(new_size);
    return *this;
  }

  std::size_t GetLength() const {
    return elems_.size();
  }

  T Norm2() const {
    T sum = T(0);
    for (const auto& elem : elems_) {
      sum += elem * elem;
    }
    return std::sqrt(sum);
  }

  T NormMax() const {
    T max_val = T(0);
    for (const auto& elem : elems_) {
      max_val = std::max(max_val, std::abs(elem));
    }
    return max_val;
  }

  static void VecError(const char str[]) {
    std::cerr << "Vector Error: " << str << std::endl;
    throw std::runtime_error(str);
  }

  friend Vector<T> operator+(const Vector<T>& a, const Vector<T>& b) {
    if (a.elems_.size() != b.elems_.size()) {
      Vector<T>::VecError("Vector size mismatch in +");
    }
    Vector<T> result(a.elems_.size());
    for (std::size_t i = 0; i < a.elems_.size(); ++i) {
      result.elems_[i] = a.elems_[i] + b.elems_[i];
    }
    return result;
  }

  friend Vector<T> operator-(const Vector<T>& a, const Vector<T>& b) {
    if (a.elems_.size() != b.elems_.size()) {
      Vector<T>::VecError("Vector size mismatch in -");
    }
    Vector<T> result(a.elems_.size());
    for (std::size_t i = 0; i < a.elems_.size(); ++i) {
      result.elems_[i] = a.elems_[i] - b.elems_[i];
    }
    return result;
  }

  friend Vector<T> operator-(const Vector<T>& a) {
    Vector<T> result(a.elems_.size());
    for (std::size_t i = 0; i < a.elems_.size(); ++i) {
      result.elems_[i] = -a.elems_[i];
    }
    return result;
  }

  friend T operator*(const Vector<T>& a, const Vector<T>& b) {
    if (a.elems_.size() != b.elems_.size()) {
      Vector<T>::VecError("Vector size mismatch in dot product");
    }
    T sum = T(0);
    for (std::size_t i = 0; i < a.elems_.size(); ++i) {
      sum += a.elems_[i] * b.elems_[i];
    }
    return sum;
  }

  friend Vector<T> operator*(T scalar, const Vector<T>& v) {
    Vector<T> result(v.elems_.size());
    for (std::size_t i = 0; i < v.elems_.size(); ++i) {
      result.elems_[i] = scalar * v.elems_[i];
    }
    return result;
  }

  friend Vector<T> operator*(const Vector<T>& v, T scalar) {
    return scalar * v;
  }

  friend Vector<T> operator/(const Vector<T>& v, T scalar) {
    if (scalar == T(0)) {
      Vector<T>::VecError("Division by zero");
    }
    Vector<T> result(v.elems_.size());
    for (std::size_t i = 0; i < v.elems_.size(); ++i) {
      result.elems_[i] = v.elems_[i] / scalar;
    }
    return result;
  }

  friend bool operator==(const Vector<T>& a, const Vector<T>& b) {
    if (a.elems_.size() != b.elems_.size()) return false;
    for (std::size_t i = 0; i < a.elems_.size(); ++i) {
      if (a.elems_[i] != b.elems_[i]) return false;
    }
    return true;
  }

  friend bool operator!=(const Vector<T>& a, const Vector<T>& b) {
    return !(a == b);
  }

  friend std::ostream& operator<<(std::ostream& os, const Vector<T>& v) {
    os << "[";
    for (std::size_t i = 0; i < v.elems_.size(); ++i) {
      if (i > 0) os << ", ";
      os << v.elems_[i];
    }
    os << "]";
    return os;
  }

 private:
  std::vector<T> elems_;
};

}  // namespace mapra

#endif  // VECTOR_H_
