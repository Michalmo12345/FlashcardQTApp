#include "../src/users/User.h"

#include <gtest/gtest.h>

TEST(UserTest, BasicAssertions) {
  User user("TestUser");
  EXPECT_EQ(user.getUsername(), "TestUser");
  user.setUsername("NewName");
  EXPECT_EQ(user.getUsername(), "NewName");
}