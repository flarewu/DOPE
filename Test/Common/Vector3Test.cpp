#include <Dope/Common/Math.h>
#include "gtest/gtest.h"
// ¹¹Ôìº¯Êý
TEST(Vector3Test, Ctor0)
{
    dp::Vector3 v;
}
TEST(Vector3Test, Ctor1)
{
    dp::Real scalars[3] = { dp::random(),dp::random(),dp::random() };
    dp::Vector3 v(scalars);
    EXPECT_EQ(v.x(), scalars[0]);
    EXPECT_EQ(v.y(), scalars[1]);
    EXPECT_EQ(v.z(), scalars[2]);

    EXPECT_EQ(v[0], scalars[0]);
    EXPECT_EQ(v[1], scalars[1]);
    EXPECT_EQ(v[2], scalars[2]);
}
TEST(Vector3Test, Ctor2)
{
    dp::Real scalars[3] = { dp::random(),dp::random(),dp::random() };
    dp::Vector3 v(scalars[0], scalars[1], scalars[2]);
    EXPECT_EQ(v.x(), scalars[0]);
    EXPECT_EQ(v.y(), scalars[1]);
    EXPECT_EQ(v.z(), scalars[2]);

    EXPECT_EQ(v[0], scalars[0]);
    EXPECT_EQ(v[1], scalars[1]);
    EXPECT_EQ(v[2], scalars[2]);
}
