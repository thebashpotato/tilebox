#include <gtest/gtest.h>

#include <tilebox/config.hpp>
#include <tilebox/core.hpp>

using namespace Tilebox;

TEST(FactorialTestSuite, Test1)
{
    ASSERT_EQ(Factorial(0), 1);
    ASSERT_EQ(Factorial(1), 1);
    ASSERT_EQ(Factorial(2), 2);
    ASSERT_EQ(Factorial(3), 6);
    ASSERT_EQ(Factorial(10), 3628800);
}

TEST(ConfigFileTestSuite, Test1)
{
    ASSERT_EQ(kTileboxName, "tilebox");
    ASSERT_EQ(kTileboxVersionMajor, 0);
    ASSERT_EQ(kTileboxVersionMinor, 1);
    ASSERT_EQ(kTileboxVersionPatch, 0);
}
