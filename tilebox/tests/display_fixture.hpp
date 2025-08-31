#include <gtest/gtest.h>
#include <tilebox/x11/display.hpp>
#include <utility>

namespace Tilebox
{

class X11DisplayFixture : public ::testing::Test
{
  public:
    void SetupDisplay()
    {
        auto dpy_opt = X11Display::Create();
        ASSERT_TRUE(dpy_opt.has_value()) << "Could not open X11 display";

        display = std::move(dpy_opt.value());
    }

    void TeardownDisplay()
    {
        display.reset();
    }

    [[nodiscard]] auto dpy() const -> const X11DisplaySharedResource &
    {
        return display;
    }

  protected:
    X11DisplaySharedResource display;
};

} // namespace Tilebox
