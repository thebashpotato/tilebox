#include <gtest/gtest.h>
#include <tilebox-core/version.hpp>

TEST(TileboxCoreVersionTest, EnsureVersionIsCorrect)
{
    ASSERT_EQ(tilebox::core::version(), "0.1.0");
}
