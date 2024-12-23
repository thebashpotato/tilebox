#include <gtest/gtest.h>

#include <tilebox/x11/display.hpp>

#include <optional>
#include <utility>

using namespace Tilebox;

class Lifetime
{
  public:
    X11DisplaySharedResource dpy;

    explicit Lifetime(X11DisplaySharedResource dpy_) : dpy(std::move(dpy_))
    {
    }
};

namespace
{
auto BuildDisplay() -> X11DisplaySharedResource
{

    if (const auto display_optional = X11Display::Create(); display_optional.has_value())
    {
        auto display = display_optional.value();
        return display;
    }

    return nullptr;
}
} // namespace

TEST(TileboxCoreX11TestSuite, VerifyX11DisplayLifetimeManagement)
{
    auto dpy = BuildDisplay();

    ASSERT_NE(dpy, nullptr);
    ASSERT_TRUE(dpy->IsConnected());
    ASSERT_EQ(dpy.use_count(), 1);

    Lifetime l1(dpy);
    ASSERT_EQ(dpy.use_count(), 2);

    {
        const Lifetime l2(dpy);
        ASSERT_EQ(dpy.use_count(), 3);
    }

    ASSERT_EQ(dpy.use_count(), 2);
    ASSERT_TRUE(dpy->IsConnected());

    l1.dpy.reset();

    ASSERT_EQ(dpy.use_count(), 1);
    ASSERT_TRUE(dpy->IsConnected());

    dpy.reset();
    ASSERT_EQ(dpy, nullptr);
}
