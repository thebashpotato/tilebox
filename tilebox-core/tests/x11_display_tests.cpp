#include <gtest/gtest.h>
#include <optional>
#include <tilebox-core/x11/display.hpp>
#include <utility>

using namespace tilebox::core;

class Lifetime
{
  public:
    X11DisplaySharedResource _dpy;

    explicit Lifetime(X11DisplaySharedResource dpy) : _dpy(std::move(dpy))
    {
    }
};

auto build_display() -> X11DisplaySharedResource
{

    auto display_optional = X11Display::create();
    if (display_optional.has_value())
    {
        auto display = display_optional.value();
        return display;
    }

    return nullptr;
}

TEST(TileboxCoreX11TestSuite, VerifyX11DisplayLifetimeManagement)
{
    auto dpy = build_display();

    ASSERT_NE(dpy, nullptr);
    ASSERT_TRUE(dpy->is_connected());
    ASSERT_EQ(dpy.use_count(), 1);

    Lifetime l1(dpy);
    ASSERT_EQ(dpy.use_count(), 2);

    {
        const Lifetime l2(dpy);
        ASSERT_EQ(dpy.use_count(), 3);
    }

    ASSERT_EQ(dpy.use_count(), 2);
    ASSERT_TRUE(dpy->is_connected());

    l1._dpy.reset();

    ASSERT_EQ(dpy.use_count(), 1);
    ASSERT_TRUE(dpy->is_connected());

    dpy.reset();
    ASSERT_EQ(dpy, nullptr);
}
