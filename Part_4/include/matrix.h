#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <stdexcept>
#include "vector.h"

namespace mapra {

template<typename T>
class Matrix {
private:
    std::vector<std::vector<T>> data;
    std::size_t rows_, cols_;

public:
    // Constructors
    Matrix() : rows_(0), cols_(0) {}
    Matrix(std::size_t rows, std::size_t cols) 
        : data(rows, std::vector<T>(cols)), rows_(rows), cols_(cols) {}
    Matrix(std::size_t rows, std::size_t cols, const T& value)
        : data(rows, std::vector<T>(cols, value)), rows_(rows), cols_(cols) {}

    // Size operations
    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }

    // Element access
    std::vector<T>& operator[](std::size_t row) {
        if (row >= rows_) throw std::out_of_range("Matrix row index out of range");
        return data[row];
    }
    
    const std::vector<T>& operator[](std::size_t row) const {
        if (row >= rows_) throw std::out_of_range("Matrix row index out of range");
        return data[row];
    }

    T& operator()(std::size_t row, std::size_t col) {
        if (row >= rows_ || col >= cols_) throw std::out_of_range("Matrix indices out of range");
        return data[row][col];
    }
    
    const T& operator()(std::size_t row, std::size_t col) const {
        if (row >= rows_ || col >= cols_) throw std::out_of_range("Matrix indices out of range");
        return data[row][col];
    }

    // Matrix operations
    Matrix<T> operator+(const Matrix<T>& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw std::invalid_argument("Matrix dimensions don't match");
        }
        Matrix<T> result(rows_, cols_);
        for (std::size_t i = 0; i < rows_; ++i) {
            for (std::size_t j = 0; j < cols_; ++j) {
                result[i][j] = data[i][j] + other[i][j];
            }
        }
        return result;
    }

    Matrix<T> operator*(const T& scalar) const {
        Matrix<T> result(rows_, cols_);
        for (std::size_t i = 0; i < rows_; ++i) {
            for (std::size_t j = 0; j < cols_; ++j) {
                result[i][j] = data[i][j] * scalar;
            }
        }
        return result;
    }

    Vector<T> operator*(const Vector<T>& vec) const {
        if (cols_ != vec.size()) {
            throw std::invalid_argument("Matrix-vector multiplication dimension mismatch");
        }
        Vector<T> result(rows_);
        for (std::size_t i = 0; i < rows_; ++i) {
            result[i] = T(0);
            for (std::size_t j = 0; j < cols_; ++j) {
                result[i] += data[i][j] * vec[j];
            }
        }
        return result;
    }
};

} // namespace mapra

#endif
