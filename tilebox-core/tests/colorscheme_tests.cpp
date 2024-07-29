#include <gtest/gtest.h>
#include <tilebox-core/draw/color.hpp>
#include <tilebox-core/draw/colorscheme.hpp>
#include <tilebox-core/x11/display.hpp>
#include <utility>

using namespace tilebox::core;

TEST(TileboxCoreColorschmeTestSuite, VerifyColorCreation)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();

    const auto color_res = X11Color::create(dpy, "#000000");
    ASSERT_EQ(color_res.is_ok(), true);

    const X11Color color = std::move(color_res.ok().value());
    ASSERT_NE(color.raw(), nullptr);
}

TEST(TileboxCoreColorschmeTestSuite, VerifyColorMoveContructor)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();

    const auto color_res = X11Color::create(dpy, "#000000");
    ASSERT_EQ(color_res.is_ok(), true);

    X11Color color = std::move(color_res.ok().value());

    auto color_2 = std::move(color);
    ASSERT_NE(color_2.raw(), nullptr);
}

TEST(TileboxCoreColorschmeTestSuite, VerifyColorCopyConstructor)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();

    const auto color_res = X11Color::create(dpy, "#000000");
    ASSERT_EQ(color_res.is_ok(), true);

    const X11Color color = std::move(color_res.ok().value());

    {
        const auto color_2 = color; // NOLINT
        ASSERT_EQ(color.raw(), color.raw());
    }

    ASSERT_NE(color.raw(), nullptr);
}

TEST(TileboxCoreColorschmeTestSuite, VerifyColorSchemeCreation)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();
    const auto scheme_res = X11ColorScheme::create(dpy, "#bbbbbb", "#222222", "#444444");

    ASSERT_EQ(scheme_res.is_ok(), true);
}

TEST(TileboxCoreColorschmeTestSuite, VerifyColorGetters)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();
    if (const auto scheme_res = X11ColorScheme::create(dpy, "#bbbbbb", "#222222", "#444444"); scheme_res.is_ok())
    {
        ASSERT_EQ(scheme_res.is_ok(), true);
        const X11ColorScheme scheme = scheme_res.ok().value();

        const X11Color &fg = scheme.get_color(X11ColorScheme::Foreground);
        const X11Color &bg = scheme.get_color(X11ColorScheme::Background);
        const X11Color &border = scheme.get_color(X11ColorScheme::Border);

        ASSERT_NE(fg.raw(), nullptr);
        ASSERT_NE(bg.raw(), nullptr);
        ASSERT_NE(border.raw(), nullptr);

        ASSERT_EQ(fg.raw()->pixel, 12303291);
        ASSERT_EQ(bg.raw()->pixel, 2236962);
        ASSERT_EQ(border.raw()->pixel, 4473924);
    }
    else
    {
        testing::AssertionFailure() << scheme_res.err().value().info() << '\n';
    }
}
