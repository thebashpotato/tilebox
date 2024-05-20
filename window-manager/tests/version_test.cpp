#include "../src/version.hpp"
#include <gtest/gtest.h>

TEST(TileboxWindowManagerVersionTest, EnsureVersionStringIsCorrect)
{
    ASSERT_EQ(tilebox::VERSION_STRING, "0.1.0");
}

TEST(TileboxWindowManagerVersionTest, EnsureVersionIntegerIsCorrect)
{
    ASSERT_EQ(tilebox::VERSION_MAJOR, 0);
    ASSERT_EQ(tilebox::VERSION_MINOR, 1);
    ASSERT_EQ(tilebox::VERSION_PATCH, 0);
}
