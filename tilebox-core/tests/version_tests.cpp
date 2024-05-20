#include "tilebox-core/version.hpp"
#include <gtest/gtest.h>

TEST(TileboxCoreVersionTest, EnsureVersionStringIsCorrect)
{
    ASSERT_EQ(tilebox::core::VERSION_STRING, "0.1.0");
}

TEST(TileboxCoreVersionTest, EnsureVersionIntegerIsCorrect)
{
    ASSERT_EQ(tilebox::core::VERSION_MAJOR, 0);
    ASSERT_EQ(tilebox::core::VERSION_MINOR, 1);
    ASSERT_EQ(tilebox::core::VERSION_PATCH, 0);
}
