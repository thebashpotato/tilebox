#include "gtest/gtest.h"
#include <tilebox-core/x11/display.hpp>

using namespace tilebox::core;

TEST(TileboxCoreX11TestSuite, VerifyX11DisplayLifetimeManagement)
{
    auto display = X11Display::create();
    ASSERT_EQ(display->is_connected(), true);

    {
        auto display_copy = display->get_shared();
        ASSERT_EQ(display.use_count(), 2);
    }

    ASSERT_EQ(display.use_count(), 1);
    ASSERT_EQ(display->is_connected(), true);
    ASSERT_EQ(display->server_vendor().empty(), false);

    display.reset();
    ASSERT_EQ(display, nullptr);
}
