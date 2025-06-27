#include <../gtest/gtest.h>
#include "../include/vector.h"

using namespace mapra;

class VectorTest : public ::testing::Test {
protected:
    Vector<double> v1, v2, v3;
    
    void SetUp() override {
        v1 = Vector<double>{1.0, 2.0, 3.0};
        v2 = Vector<double>{4.0, 5.0, 6.0};
        v3 = Vector<double>(3, 0.0);
    }
};

TEST_F(VectorTest, Construction) {
    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ(v1[0], 1.0);
    EXPECT_EQ(v1[1], 2.0);
    EXPECT_EQ(v1[2], 3.0);
}

TEST_F(VectorTest, Addition) {
    Vector<double> result = v1 + v2;
    EXPECT_EQ(result[0], 5.0);
    EXPECT_EQ(result[1], 7.0);
    EXPECT_EQ(result[2], 9.0);
}

TEST_F(VectorTest, Subtraction) {
    Vector<double> result = v2 - v1;
    EXPECT_EQ(result[0], 3.0);
    EXPECT_EQ(result[1], 3.0);
    EXPECT_EQ(result[2], 3.0);
}

TEST_F(VectorTest, ScalarMultiplication) {
    Vector<double> result = v1 * 2.0;
    EXPECT_EQ(result[0], 2.0);
    EXPECT_EQ(result[1], 4.0);
    EXPECT_EQ(result[2], 6.0);
}

TEST_F(VectorTest, Norm) {
    Vector<double> v{3.0, 4.0};
    EXPECT_DOUBLE_EQ(v.norm(), 5.0);
}

TEST_F(VectorTest, IsZero) {
    EXPECT_TRUE(v3.is_zero());
    EXPECT_FALSE(v1.is_zero());
}
