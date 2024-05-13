#include <gtest/gtest.h>
#include <optional>
#include <tilebox-core/x11/display.hpp>

using namespace tilebox::core::x;

TEST(TileboxCoreX11TestSuite, VerifyX11DisplayLifetimeManagement)
{
    auto display = X11Display(std::nullopt);
    auto shared_display = display.shared();
    ASSERT_TRUE(display.is_connected());

    {
        auto display_copy = display.shared();
        ASSERT_EQ(shared_display.use_count(), 3);
    }

    ASSERT_EQ(shared_display.use_count(), 2);
    ASSERT_TRUE(display.is_connected());
    ASSERT_FALSE(display.server_vendor().empty());

    shared_display.reset();
    ASSERT_EQ(shared_display, nullptr);
}
