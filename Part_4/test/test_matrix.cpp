#include <gtest/gtest.h>
#include "../include/matrix.h"

using namespace mapra;

class MatrixTest : public ::testing::Test {
protected:
    Matrix<double> m1, m2;
    Vector<double> v;
    
    void SetUp() override {
        m1 = Matrix<double>(2, 3);
        m1[0][0] = 1; m1[0][1] = 2; m1[0][2] = 3;
        m1[1][0] = 4; m1[1][1] = 5; m1[1][2] = 6;
        
        v = Vector<double>{1.0, 2.0, 3.0};
    }
};

TEST_F(MatrixTest, Construction) {
    EXPECT_EQ(m1.rows(), 2);
    EXPECT_EQ(m1.cols(), 3);
    EXPECT_EQ(m1[0][0], 1);
    EXPECT_EQ(m1[1][2], 6);
}

TEST_F(MatrixTest, MatrixVectorMultiplication) {
    Vector<double> result = m1 * v;
    EXPECT_EQ(result.size(), 2);
    EXPECT_DOUBLE_EQ(result[0], 14.0); // 1*1 + 2*2 + 3*3
    EXPECT_DOUBLE_EQ(result[1], 32.0); // 4*1 + 5*2 + 6*3
}

TEST_F(MatrixTest, ScalarMultiplication) {
    Matrix<double> result = m1 * 2.0;
    EXPECT_EQ(result[0][0], 2);
    EXPECT_EQ(result[1][1], 10);
}

TEST_F(MatrixTest, Addition) {
    m2 = Matrix<double>(2, 3);
    m2[0][0] = 1; m2[0][1] = 1; m2[0][2] = 1;
    m2[1][0] = 1; m2[1][1] = 1; m2[1][2] = 1;

    Matrix<double> result = m1 + m2;
    EXPECT_EQ(result[0][0], 2);
    EXPECT_EQ(result[1][2], 7);
}