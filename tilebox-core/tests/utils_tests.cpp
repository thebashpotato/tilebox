#include <gtest/gtest.h>
#include <tilebox-core/version.hpp>

TEST(TileboxCoreUtilsTest, EnsureVersionIsCorrect)
{
    ASSERT_EQ(tilebox::core::version(), "0.1.0");
}
