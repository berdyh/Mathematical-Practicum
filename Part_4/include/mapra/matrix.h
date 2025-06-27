// Copyright (c) 2022, The MaPra Authors.

#ifndef MATRIX_H_
#define MATRIX_H_

#include <cmath>
#include <cstddef>
#include <iostream>
#include <vector>
#include <stdexcept>

#include "mapra/vector.h"

namespace mapra {

template <typename T = double>
class Matrix {
 public:
  explicit Matrix(std::size_t r = 1, std::size_t c = 1) 
    : rows_(r), cols_(c), elems_(r * c, T(0)) {}

  T& operator()(std::size_t i, std::size_t j) {
#ifndef NDEBUG
    if (i >= rows_ || j >= cols_) {
      MatError("Matrix index out of range");
    }
#endif
    return elems_[i + rows_ * j];
  }

  T operator()(std::size_t i, std::size_t j) const {
#ifndef NDEBUG
    if (i >= rows_ || j >= cols_) {
      MatError("Matrix index out of range");
    }
#endif
    return elems_[i + rows_ * j];
  }

  Matrix<T>& operator+=(const Matrix<T>& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
      MatError("Matrix dimension mismatch in +=");
    }
    for (std::size_t i = 0; i < elems_.size(); ++i) {
      elems_[i] += other.elems_[i];
    }
    return *this;
  }

  Matrix<T>& operator-=(const Matrix<T>& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
      MatError("Matrix dimension mismatch in -=");
    }
    for (std::size_t i = 0; i < elems_.size(); ++i) {
      elems_[i] -= other.elems_[i];
    }
    return *this;
  }

  Matrix<T>& operator*=(const Matrix<T>& other) {
    *this = *this * other;
    return *this;
  }

  Matrix<T>& operator*=(T scalar) {
    for (auto& elem : elems_) {
      elem *= scalar;
    }
    return *this;
  }

  Matrix<T>& operator/=(T scalar) {
    if (scalar == T(0)) {
      MatError("Division by zero");
    }
    for (auto& elem : elems_) {
      elem /= scalar;
    }
    return *this;
  }

  Matrix<T>& Redim(std::size_t r, std::size_t c) {
    rows_ = r;
    cols_ = c;
    elems_.resize(r * c);
    return *this;
  }

  std::size_t GetRows() const { return rows_; }
  std::size_t GetCols() const { return cols_; }

  static void MatError(const char str[]) {
    std::cerr << "Matrix Error: " << str << std::endl;
    throw std::runtime_error(str);
  }

  friend Matrix<T> operator+(const Matrix<T>& a, const Matrix<T>& b) {
    if (a.rows_ != b.rows_ || a.cols_ != b.cols_) {
      Matrix<T>::MatError("Matrix dimension mismatch in +");
    }
    Matrix<T> result(a.rows_, a.cols_);
    for (std::size_t i = 0; i < a.elems_.size(); ++i) {
      result.elems_[i] = a.elems_[i] + b.elems_[i];
    }
    return result;
  }

  friend Matrix<T> operator-(const Matrix<T>& a, const Matrix<T>& b) {
    if (a.rows_ != b.rows_ || a.cols_ != b.cols_) {
      Matrix<T>::MatError("Matrix dimension mismatch in -");
    }
    Matrix<T> result(a.rows_, a.cols_);
    for (std::size_t i = 0; i < a.elems_.size(); ++i) {
      result.elems_[i] = a.elems_[i] - b.elems_[i];
    }
    return result;
  }

  friend Matrix<T> operator-(const Matrix<T>& a) {
    Matrix<T> result(a.rows_, a.cols_);
    for (std::size_t i = 0; i < a.elems_.size(); ++i) {
      result.elems_[i] = -a.elems_[i];
    }
    return result;
  }

  friend Matrix<T> operator*(const Matrix<T>& a, const Matrix<T>& b) {
    if (a.cols_ != b.rows_) {
      Matrix<T>::MatError("Matrix dimension mismatch in *");
    }
    Matrix<T> result(a.rows_, b.cols_);
    for (std::size_t i = 0; i < a.rows_; ++i) {
      for (std::size_t j = 0; j < b.cols_; ++j) {
        T sum = T(0);
        for (std::size_t k = 0; k < a.cols_; ++k) {
          sum += a(i, k) * b(k, j);
        }
        result(i, j) = sum;
      }
    }
    return result;
  }

  friend Matrix<T> operator*(T scalar, const Matrix<T>& m) {
    Matrix<T> result(m.rows_, m.cols_);
    for (std::size_t i = 0; i < m.elems_.size(); ++i) {
      result.elems_[i] = scalar * m.elems_[i];
    }
    return result;
  }

  friend Matrix<T> operator*(const Matrix<T>& m, T scalar) {
    return scalar * m;
  }

  friend Matrix<T> operator/(const Matrix<T>& m, T scalar) {
    if (scalar == T(0)) {
      Matrix<T>::MatError("Division by zero");
    }
    Matrix<T> result(m.rows_, m.cols_);
    for (std::size_t i = 0; i < m.elems_.size(); ++i) {
      result.elems_[i] = m.elems_[i] / scalar;
    }
    return result;
  }

  friend Vector<T> operator*(const Matrix<T>& m, const Vector<T>& v) {
    if (m.cols_ != v.GetLength()) {
      Matrix<T>::MatError("Matrix-vector dimension mismatch");
    }
    Vector<T> result(m.rows_);
    for (std::size_t i = 0; i < m.rows_; ++i) {
      T sum = T(0);
      for (std::size_t j = 0; j < m.cols_; ++j) {
        sum += m(i, j) * v(j);
      }
      result(i) = sum;
    }
    return result;
  }

  friend bool operator==(const Matrix<T>& a, const Matrix<T>& b) {
    if (a.rows_ != b.rows_ || a.cols_ != b.cols_) return false;
    for (std::size_t i = 0; i < a.elems_.size(); ++i) {
      if (a.elems_[i] != b.elems_[i]) return false;
    }
    return true;
  }

  friend bool operator!=(const Matrix<T>& a, const Matrix<T>& b) {
    return !(a == b);
  }

 private:
  std::size_t rows_;
  std::size_t cols_;
  std::vector<T> elems_;
};

}  // namespace mapra

#endif  // MATRIX_H_
