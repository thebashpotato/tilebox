#include <gtest/gtest.h>
#include <tilebox-core/draw/font.hpp>
#include <tilebox-core/x11/display.hpp>
#include <utility>

/// TODO: A Gtest Setup() and TearDown() should be implemented for the X11DisplaySharedResource

using namespace tilebox::core;

TEST(TileboxCoreX11FontTestSuite, VerifyFontCreation)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();
    const auto font_res = X11Font::create(dpy, "monospace:size=12");

    ASSERT_EQ(font_res.is_ok(), true);
    const auto font = font_res.ok().value();

    // The ascent + decent of monospace at size 12 should always be 23;
    ASSERT_EQ(font.height(), 23);
}

TEST(TileboxCoreX11FontTestSuite, VerifyMoveConstructor)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();

    const auto font_res1 = X11Font::create(dpy, "monospace:size=12");

    ASSERT_EQ(font_res1.is_ok(), true);

    auto font1 = font_res1.ok().value();

    ASSERT_EQ(font1.height(), 23);
    ASSERT_NE(font1.xftfont()->pattern, nullptr);
    ASSERT_NE(font1.pattern(), nullptr);

    auto font2 = std::move(font1);

    ASSERT_EQ(font2.height(), 23);
    ASSERT_NE(font2.xftfont()->pattern, nullptr);
    ASSERT_NE(font2.pattern(), nullptr);
}

TEST(TileboxCoreX11FontTestSuite, VerifyCopyConstructor)
{

    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();

    const auto font_res1 = X11Font::create(dpy, "monospace:size=12");

    ASSERT_EQ(font_res1.is_ok(), true);

    const auto font1 = font_res1.ok().value();

    ASSERT_EQ(font1.height(), 23);
    ASSERT_NE(font1.xftfont()->pattern, nullptr);
    ASSERT_NE(font1.pattern(), nullptr);

    {
        const auto font2 = font1; // NOLINT

        ASSERT_EQ(font2.height(), 23);
        ASSERT_NE(font2.xftfont()->pattern, nullptr);
        ASSERT_NE(font2.pattern(), nullptr);
    }

    ASSERT_EQ(font1.height(), 23);
    ASSERT_NE(font1.xftfont()->pattern, nullptr);
    ASSERT_NE(font1.pattern(), nullptr);
}

TEST(TileboxCoreX11FontTestSuite, VerifyMoveAssignment)
{

    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();

    const auto font_res1 = X11Font::create(dpy, "monospace:size=12");
    const auto font_res2 = X11Font::create(dpy, "monospace:size=16");

    ASSERT_EQ(font_res1.is_ok(), true);
    ASSERT_EQ(font_res2.is_ok(), true);

    auto font1 = font_res1.ok().value();
    auto font2 = font_res2.ok().value();

    ASSERT_EQ(font1.height(), 23);

    font1 = std::move(font2);

    ASSERT_EQ(font1.height(), 30);
}

TEST(TileboxCoreX11FontTestSuite, VerifyCopyAssignment)
{

    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();

    const auto font_res1 = X11Font::create(dpy, "monospace:size=12");
    const auto font_res2 = X11Font::create(dpy, "monospace:size=16");

    ASSERT_EQ(font_res1.is_ok(), true);
    ASSERT_EQ(font_res2.is_ok(), true);

    auto font1 = font_res1.ok().value();
    const auto font2 = font_res2.ok().value();

    ASSERT_EQ(font1.height(), 23);

    font1 = font2;

    ASSERT_EQ(font1.height(), 30);
    ASSERT_EQ(font2.height(), 30);
}
