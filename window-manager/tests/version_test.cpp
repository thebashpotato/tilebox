#include "../src/version.hpp"
#include <gtest/gtest.h>

TEST(TileboxWindowManagerVersionTest, EnsureVersionIsCorrect)
{
    ASSERT_EQ(tilebox::wm::version(), "0.1.0");
}
