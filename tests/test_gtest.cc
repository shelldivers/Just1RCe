#include <gtest/gtest.h>

TEST(Hello2Test, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 5, 42);
}