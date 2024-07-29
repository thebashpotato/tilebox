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

TEST(TileboxCoreColorschmeTestSuite, VerifyColorCopySemantics)
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
