#include <gtest/gtest.h>
#include "motor_control.h"

// Demonstrate some basic assertions.
TEST(Google_tests, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}