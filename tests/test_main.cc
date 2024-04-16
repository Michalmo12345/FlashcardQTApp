#include <gtest/gtest.h>

TEST(sample, dosuccesspls)
{
    EXPECT_EQ(1u, 1u);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}