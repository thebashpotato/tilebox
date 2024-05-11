#include "tilebox-core/version.hpp"
#include <gtest/gtest.h>

TEST(TileboxCoreUtilsTest, EnsureVersionIsCorrect)
{
    ASSERT_EQ(tilebox::core::version(), "0.1.0");
}
