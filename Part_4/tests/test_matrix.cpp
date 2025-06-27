#include <stdexcept>

#include "gtest/gtest.h"
#include "mapra/matrix.h"
#include "mapra/vector.h"

using namespace mapra;

class MatrixTest : public ::testing::Test {
 protected:
  Matrix<double> m1, m2, m3;
  Vector<double> v;
  
  void SetUp() override {
    // Initialize test matrices
    m1.Redim(2, 3);
    m1(0,0) = 1; m1(0,1) = 2; m1(0,2) = 3;
    m1(1,0) = 4; m1(1,1) = 5; m1(1,2) = 6;
    
    m2.Redim(2, 3);
    m2(0,0) = 7; m2(0,1) = 8; m2(0,2) = 9;
    m2(1,0) = 10; m2(1,1) = 11; m2(1,2) = 12;
    
    m3.Redim(2, 3);
    // m3 remains zero-initialized
    
    v.Redim(3);
    v(0) = 1.0; v(1) = 2.0; v(2) = 3.0;
  }
};

TEST_F(MatrixTest, Construction) {
  Matrix<double> m_default;
  EXPECT_EQ(m_default.GetRows(), 1);
  EXPECT_EQ(m_default.GetCols(), 1);
  EXPECT_DOUBLE_EQ(m_default(0,0), 0.0);
  
  Matrix<double> m_sized(3, 4);
  EXPECT_EQ(m_sized.GetRows(), 3);
  EXPECT_EQ(m_sized.GetCols(), 4);
  for (std::size_t i = 0; i < 3; ++i) {
    for (std::size_t j = 0; j < 4; ++j) {
      EXPECT_DOUBLE_EQ(m_sized(i,j), 0.0);
    }
  }
}

TEST_F(MatrixTest, ElementAccess) {
  EXPECT_DOUBLE_EQ(m1(0,0), 1);
  EXPECT_DOUBLE_EQ(m1(0,1), 2);
  EXPECT_DOUBLE_EQ(m1(1,2), 6);
  
  // Test const access
  const Matrix<double>& cm1 = m1;
  EXPECT_DOUBLE_EQ(cm1(0,0), 1);
  
  // Test modification
  m1(0,0) = 10.0;
  EXPECT_DOUBLE_EQ(m1(0,0), 10.0);
}

TEST_F(MatrixTest, Redim) {
  Matrix<double> m(2, 2);
  m(0,0) = 1; m(0,1) = 2;
  m(1,0) = 3; m(1,1) = 4;
  
  m.Redim(3, 3);
  EXPECT_EQ(m.GetRows(), 3);
  EXPECT_EQ(m.GetCols(), 3);
  
  // New elements initialized as zero
  EXPECT_DOUBLE_EQ(m(0,0), 1);
  EXPECT_DOUBLE_EQ(m(0,1), 4);
  EXPECT_DOUBLE_EQ(m(1,0), 3);
  EXPECT_DOUBLE_EQ(m(1,1), 0);
  EXPECT_DOUBLE_EQ(m(2,0), 2);
  EXPECT_DOUBLE_EQ(m(2,1), 0);
  EXPECT_DOUBLE_EQ(m(2,2), 0);
  EXPECT_DOUBLE_EQ(m(0,2), 0);
  EXPECT_DOUBLE_EQ(m(1,2), 0);
}

TEST_F(MatrixTest, Addition) {
  Matrix<double> result = m1 + m2;
  EXPECT_EQ(result.GetRows(), 2);
  EXPECT_EQ(result.GetCols(), 3);
  
  EXPECT_DOUBLE_EQ(result(0,0), 8);  // 1 + 7
  EXPECT_DOUBLE_EQ(result(0,1), 10); // 2 + 8
  EXPECT_DOUBLE_EQ(result(1,2), 18); // 6 + 12
}

TEST_F(MatrixTest, AdditionAssignment) {
  m1 += m2;
  EXPECT_DOUBLE_EQ(m1(0,0), 8);
  EXPECT_DOUBLE_EQ(m1(0,1), 10);
  EXPECT_DOUBLE_EQ(m1(1,2), 18);
}

TEST_F(MatrixTest, Subtraction) {
  Matrix<double> result = m2 - m1;
  EXPECT_DOUBLE_EQ(result(0,0), 6);  // 7 - 1
  EXPECT_DOUBLE_EQ(result(0,1), 6);  // 8 - 2
  EXPECT_DOUBLE_EQ(result(1,2), 6);  // 12 - 6
}

TEST_F(MatrixTest, SubtractionAssignment) {
  m2 -= m1;
  EXPECT_DOUBLE_EQ(m2(0,0), 6);
  EXPECT_DOUBLE_EQ(m2(0,1), 6);
  EXPECT_DOUBLE_EQ(m2(1,2), 6);
}

TEST_F(MatrixTest, UnaryMinus) {
  Matrix<double> result = -m1;
  EXPECT_DOUBLE_EQ(result(0,0), -1);
  EXPECT_DOUBLE_EQ(result(0,1), -2);
  EXPECT_DOUBLE_EQ(result(1,2), -6);
}

TEST_F(MatrixTest, ScalarMultiplication) {
  Matrix<double> result1 = m1 * 2.0;
  Matrix<double> result2 = 3.0 * m1;
  
  EXPECT_DOUBLE_EQ(result1(0,0), 2);
  EXPECT_DOUBLE_EQ(result1(0,1), 4);
  EXPECT_DOUBLE_EQ(result1(1,2), 12);
  
  EXPECT_DOUBLE_EQ(result2(0,0), 3);
  EXPECT_DOUBLE_EQ(result2(0,1), 6);
  EXPECT_DOUBLE_EQ(result2(1,2), 18);
}

TEST_F(MatrixTest, ScalarMultiplicationAssignment) {
  m1 *= 2.0;
  EXPECT_DOUBLE_EQ(m1(0,0), 2);
  EXPECT_DOUBLE_EQ(m1(0,1), 4);
  EXPECT_DOUBLE_EQ(m1(1,2), 12);
}

TEST_F(MatrixTest, ScalarDivision) {
  Matrix<double> result = m1 / 2.0;
  EXPECT_DOUBLE_EQ(result(0,0), 0.5);
  EXPECT_DOUBLE_EQ(result(0,1), 1.0);
  EXPECT_DOUBLE_EQ(result(1,2), 3.0);
}

TEST_F(MatrixTest, ScalarDivisionAssignment) {
  m1 /= 2.0;
  EXPECT_DOUBLE_EQ(m1(0,0), 0.5);
  EXPECT_DOUBLE_EQ(m1(0,1), 1.0);
  EXPECT_DOUBLE_EQ(m1(1,2), 3.0);
}

TEST_F(MatrixTest, MatrixVectorMultiplication) {
  Vector<double> result = m1 * v;
  EXPECT_EQ(result.GetLength(), 2);
  
  // [1 2 3] * [1]   [1*1 + 2*2 + 3*3]   [14]
  // [4 5 6]   [2] = [4*1 + 5*2 + 6*3] = [32]
  //           [3]
  EXPECT_DOUBLE_EQ(result(0), 14.0);
  EXPECT_DOUBLE_EQ(result(1), 32.0);
}

TEST_F(MatrixTest, MatrixMultiplication) {
  Matrix<double> A(2, 3);
  A(0,0) = 1; A(0,1) = 2; A(0,2) = 3;
  A(1,0) = 4; A(1,1) = 5; A(1,2) = 6;
  
  Matrix<double> B(3, 2);
  B(0,0) = 7; B(0,1) = 8;
  B(1,0) = 9; B(1,1) = 10;
  B(2,0) = 11; B(2,1) = 12;
  
  Matrix<double> C = A * B;
  EXPECT_EQ(C.GetRows(), 2);
  EXPECT_EQ(C.GetCols(), 2);
  
  // [1 2 3] * [7  8 ]   [1*7+2*9+3*11  1*8+2*10+3*12]   [58  64]
  // [4 5 6]   [9  10] = [4*7+5*9+6*11  4*8+5*10+6*12] = [139 154]
  //           [11 12]
  EXPECT_DOUBLE_EQ(C(0,0), 58);
  EXPECT_DOUBLE_EQ(C(0,1), 64);
  EXPECT_DOUBLE_EQ(C(1,0), 139);
  EXPECT_DOUBLE_EQ(C(1,1), 154);
}

TEST_F(MatrixTest, MatrixMultiplicationAssignment) {
  Matrix<double> A(2, 2);
  A(0,0) = 1; A(0,1) = 2;
  A(1,0) = 3; A(1,1) = 4;
  
  Matrix<double> B(2, 2);
  B(0,0) = 5; B(0,1) = 6;
  B(1,0) = 7; B(1,1) = 8;
  
  A *= B;
  
  // [1 2] * [5 6]   [1*5+2*7  1*6+2*8]   [19 22]
  // [3 4]   [7 8] = [3*5+4*7  3*6+4*8] = [43 50]
  EXPECT_DOUBLE_EQ(A(0,0), 19);
  EXPECT_DOUBLE_EQ(A(0,1), 22);
  EXPECT_DOUBLE_EQ(A(1,0), 43);
  EXPECT_DOUBLE_EQ(A(1,1), 50);
}

TEST_F(MatrixTest, Equality) {
  Matrix<double> m1_copy = m1;
  EXPECT_TRUE(m1 == m1_copy);
  EXPECT_FALSE(m1 != m1_copy);
  
  EXPECT_FALSE(m1 == m2);
  EXPECT_TRUE(m1 != m2);
}

TEST_F(MatrixTest, TemplateTypes) {
  Matrix<int> mi(2, 2);
  mi(0,0) = 10; mi(0,1) = 20;
  mi(1,0) = 30; mi(1,1) = 40;
  
  Matrix<int> result = mi * 2;
  EXPECT_EQ(result(0,0), 20);
  EXPECT_EQ(result(0,1), 40);
  EXPECT_EQ(result(1,0), 60);
  EXPECT_EQ(result(1,1), 80);
}

#ifndef NDEBUG
TEST_F(MatrixTest, BoundsChecking) {
  EXPECT_THROW(m1(5, 0), std::runtime_error);
  EXPECT_THROW(m1(0, 5), std::runtime_error);
  
  const Matrix<double>& cm1 = m1;
  EXPECT_THROW(cm1(5, 0), std::runtime_error);
}
#endif

TEST_F(MatrixTest, DimensionMismatch) {
  Matrix<double> wrong_size(3, 2);
  EXPECT_THROW(m1 + wrong_size, std::runtime_error);
  EXPECT_THROW(m1 - wrong_size, std::runtime_error);
  
  Vector<double> wrong_v(2);
  EXPECT_THROW(m1 * wrong_v, std::runtime_error);
}

TEST_F(MatrixTest, DivisionByZero) {
  EXPECT_THROW(m1 /= 0.0, std::runtime_error);
  EXPECT_THROW(m1 / 0.0, std::runtime_error);
}
