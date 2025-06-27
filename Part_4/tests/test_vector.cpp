#include <cmath>
#include <stdexcept>

#include "gtest/gtest.h"
#include "mapra/vector.h"

using namespace mapra;

class VectorTest : public ::testing::Test {
 protected:
  Vector<double> v1, v2, v3;
  Vector<int> vi;
  
  void SetUp() override {
    // Initialize test vectors
    v1.Redim(3);
    v1(0) = 1.0; v1(1) = 2.0; v1(2) = 3.0;
    
    v2.Redim(3);
    v2(0) = 4.0; v2(1) = 5.0; v2(2) = 6.0;
    
    v3.Redim(3);
    // v3 remains zero-initialized
    
    vi.Redim(2);
    vi(0) = 10; vi(1) = 20;
  }
};

TEST_F(VectorTest, Construction) {
  Vector<double> v_default;
  EXPECT_EQ(v_default.GetLength(), 1);
  
  Vector<double> v_sized(5);
  EXPECT_EQ(v_sized.GetLength(), 5);
  for (std::size_t i = 0; i < 5; ++i) {
    EXPECT_DOUBLE_EQ(v_sized(i), 0.0);
  }
}

TEST_F(VectorTest, ElementAccess) {
  EXPECT_DOUBLE_EQ(v1(0), 1.0);
  EXPECT_DOUBLE_EQ(v1(1), 2.0);
  EXPECT_DOUBLE_EQ(v1(2), 3.0);
  
  // Test const access
  const Vector<double>& cv1 = v1;
  EXPECT_DOUBLE_EQ(cv1(0), 1.0);
  
  // Test modification
  v1(0) = 10.0;
  EXPECT_DOUBLE_EQ(v1(0), 10.0);
}

TEST_F(VectorTest, Redim) {
  Vector<double> v(2);
  v(0) = 1.0; v(1) = 2.0;
  
  v.Redim(4);
  EXPECT_EQ(v.GetLength(), 4);
  EXPECT_DOUBLE_EQ(v(0), 1.0);  // Original values preserved
  EXPECT_DOUBLE_EQ(v(1), 2.0);
  EXPECT_DOUBLE_EQ(v(2), 0.0);  // New elements zero-initialized
  EXPECT_DOUBLE_EQ(v(3), 0.0);
}

TEST_F(VectorTest, Addition) {
  Vector<double> result = v1 + v2;
  EXPECT_EQ(result.GetLength(), 3);
  EXPECT_DOUBLE_EQ(result(0), 5.0);  // 1 + 4
  EXPECT_DOUBLE_EQ(result(1), 7.0);  // 2 + 5
  EXPECT_DOUBLE_EQ(result(2), 9.0);  // 3 + 6
}

TEST_F(VectorTest, AdditionAssignment) {
  v1 += v2;
  EXPECT_DOUBLE_EQ(v1(0), 5.0);
  EXPECT_DOUBLE_EQ(v1(1), 7.0);
  EXPECT_DOUBLE_EQ(v1(2), 9.0);
}

TEST_F(VectorTest, Subtraction) {
  Vector<double> result = v2 - v1;
  EXPECT_DOUBLE_EQ(result(0), 3.0);  // 4 - 1
  EXPECT_DOUBLE_EQ(result(1), 3.0);  // 5 - 2
  EXPECT_DOUBLE_EQ(result(2), 3.0);  // 6 - 3
}

TEST_F(VectorTest, SubtractionAssignment) {
  v2 -= v1;
  EXPECT_DOUBLE_EQ(v2(0), 3.0);
  EXPECT_DOUBLE_EQ(v2(1), 3.0);
  EXPECT_DOUBLE_EQ(v2(2), 3.0);
}

TEST_F(VectorTest, UnaryMinus) {
  Vector<double> result = -v1;
  EXPECT_DOUBLE_EQ(result(0), -1.0);
  EXPECT_DOUBLE_EQ(result(1), -2.0);
  EXPECT_DOUBLE_EQ(result(2), -3.0);
}

TEST_F(VectorTest, ScalarMultiplication) {
  Vector<double> result1 = v1 * 2.0;
  Vector<double> result2 = 3.0 * v1;
  
  EXPECT_DOUBLE_EQ(result1(0), 2.0);
  EXPECT_DOUBLE_EQ(result1(1), 4.0);
  EXPECT_DOUBLE_EQ(result1(2), 6.0);
  
  EXPECT_DOUBLE_EQ(result2(0), 3.0);
  EXPECT_DOUBLE_EQ(result2(1), 6.0);
  EXPECT_DOUBLE_EQ(result2(2), 9.0);
}

TEST_F(VectorTest, ScalarMultiplicationAssignment) {
  v1 *= 2.0;
  EXPECT_DOUBLE_EQ(v1(0), 2.0);
  EXPECT_DOUBLE_EQ(v1(1), 4.0);
  EXPECT_DOUBLE_EQ(v1(2), 6.0);
}

TEST_F(VectorTest, ScalarDivision) {
  Vector<double> result = v1 / 2.0;
  EXPECT_DOUBLE_EQ(result(0), 0.5);
  EXPECT_DOUBLE_EQ(result(1), 1.0);
  EXPECT_DOUBLE_EQ(result(2), 1.5);
}

TEST_F(VectorTest, ScalarDivisionAssignment) {
  v1 /= 2.0;
  EXPECT_DOUBLE_EQ(v1(0), 0.5);
  EXPECT_DOUBLE_EQ(v1(1), 1.0);
  EXPECT_DOUBLE_EQ(v1(2), 1.5);
}

TEST_F(VectorTest, DotProduct) {
  double result = v1 * v2;
  EXPECT_DOUBLE_EQ(result, 32.0);  // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
}

TEST_F(VectorTest, Norm2) {
  Vector<double> v(2);
  v(0) = 3.0; v(1) = 4.0;
  EXPECT_DOUBLE_EQ(v.Norm2(), 5.0);  // sqrt(3² + 4²) = sqrt(25) = 5
  
  EXPECT_DOUBLE_EQ(v3.Norm2(), 0.0);  // Zero vector
}

TEST_F(VectorTest, NormMax) {
  EXPECT_DOUBLE_EQ(v1.NormMax(), 3.0);  // max(|1|, |2|, |3|) = 3
  
  Vector<double> v_neg(3);
  v_neg(0) = -5.0; v_neg(1) = 2.0; v_neg(2) = -1.0;
  EXPECT_DOUBLE_EQ(v_neg.NormMax(), 5.0);  // max(|-5|, |2|, |-1|) = 5
}

TEST_F(VectorTest, Equality) {
  Vector<double> v1_copy = v1;
  EXPECT_TRUE(v1 == v1_copy);
  EXPECT_FALSE(v1 != v1_copy);
  
  EXPECT_FALSE(v1 == v2);
  EXPECT_TRUE(v1 != v2);
}

TEST_F(VectorTest, TemplateTypes) {
  EXPECT_EQ(vi(0), 10);
  EXPECT_EQ(vi(1), 20);
  
  vi *= 2;
  EXPECT_EQ(vi(0), 20);
  EXPECT_EQ(vi(1), 40);
}

#ifndef NDEBUG
TEST_F(VectorTest, BoundsChecking) {
  EXPECT_THROW(v1(5), std::runtime_error);
  
  const Vector<double>& cv1 = v1;
  EXPECT_THROW(cv1(5), std::runtime_error);
}
#endif

TEST_F(VectorTest, DivisionByZero) {
  EXPECT_THROW(v1 /= 0.0, std::runtime_error);
  EXPECT_THROW(v1 / 0.0, std::runtime_error);
}
