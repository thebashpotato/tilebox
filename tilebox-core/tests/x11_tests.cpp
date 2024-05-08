#include "gtest/gtest.h"
#include <tilebox-core/x11/display.hpp>

using namespace tilebox::core::x;

TEST(TileboxCoreX11TestSuite, VerifyX11DisplayLifetimeManagement)
{
    auto display = X11Display();
    auto shared_display = display.get_shared();
    ASSERT_EQ(display.is_connected(), true);

    {
        auto display_copy = display.get_shared();
        ASSERT_EQ(shared_display.use_count(), 3);
    }

    ASSERT_EQ(shared_display.use_count(), 2);
    ASSERT_EQ(display.is_connected(), true);
    ASSERT_EQ(display.server_vendor().empty(), false);

    shared_display.reset();
    ASSERT_EQ(shared_display, nullptr);
}
