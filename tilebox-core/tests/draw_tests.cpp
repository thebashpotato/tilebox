#include <cstdint>
#include <gtest/gtest.h>
#include <tilebox-core/draw/colorscheme_config.hpp>
#include <tilebox-core/draw/cursor.hpp>
#include <tilebox-core/draw/draw.hpp>
#include <tilebox-core/geometry.hpp>
#include <tilebox-core/x11/display.hpp>
#include <utility>

using namespace tilebox::core;

TEST(TileboxCoreX11DrawTestSuite, VerifyCreate)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();
    const Width width = Width(static_cast<uint32_t>(dpy->screen_width_raw()));
    const Height height = Height(static_cast<uint32_t>(dpy->screen_height_raw()));
    auto draw_res = X11Draw::create(dpy, width, height);

    EXPECT_EQ(draw_res.is_ok(), true);
}

TEST(TileboxCoreX11DrawTestSuite, VerifyMoveConstruction)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();
    const Width mon_width = Width(static_cast<uint32_t>(dpy->screen_width_raw()));
    const Height mon_height = Height(static_cast<uint32_t>(dpy->screen_height_raw()));

    /// First draw object
    auto draw_res1 = X11Draw::create(dpy, mon_width, mon_height);
    EXPECT_EQ(draw_res1.is_ok(), true);

    auto draw1 = std::move(draw_res1.ok().value());

    EXPECT_EQ(draw1.width(), mon_width);
    EXPECT_EQ(draw1.height(), mon_height);
}

TEST(TileboxX11CoreDrawTestSuite, VerifyMoveAssignment)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();
    const Width screenw = dpy->screen_width();
    const Height screenh = dpy->screen_height();

    /// First draw object
    auto draw_res1 = X11Draw::create(dpy, screenw, screenh);
    EXPECT_EQ(draw_res1.is_ok(), true);

    auto draw1 = std::move(*draw_res1.ok());

    EXPECT_EQ(draw1.width(), screenw);
    EXPECT_EQ(draw1.height(), screenh);

    /// Second draw object
    auto draw_res2 = X11Draw::create(dpy, Width(12), Height(12));
    EXPECT_EQ(draw_res2.is_ok(), true);

    auto draw2 = std::move(*draw_res2.ok());

    EXPECT_EQ(draw2.width(), Width(12));
    EXPECT_EQ(draw2.height(), Height(12));

    // draw2 should now have all of draw1's properties
    draw2 = std::move(draw1);
    EXPECT_EQ(draw2.width(), screenw);
    EXPECT_EQ(draw2.height(), screenh);
}

TEST(TileboxCoreX11DrawTestSuite, VerifyAddFontSet)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();

    /// First draw object
    auto draw_res = X11Draw::create(dpy, dpy->screen_width(), dpy->screen_height());
    EXPECT_EQ(draw_res.is_ok(), true);

    auto draw = std::move(*draw_res.ok());
    EXPECT_EQ(draw.init_font_set("monospace:size=12").is_ok(), true);
}

TEST(TileboxCoreX11DrawTestSuite, VerifyInitColorscheme)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();
    const Width screenw = dpy->screen_width();
    const Height screenh = dpy->screen_height();

    if (auto draw_res = X11Draw::create(dpy, screenw, screenh); draw_res.is_ok())
    {
        auto draw = std::move(*draw_res.ok());
        const ColorSchemeConfig primary = ColorSchemeConfig::build(ColorSchemeKind::Primary)
                                              .foreground("#bbbbbb")
                                              .background("#222222")
                                              .border("#444444");

        const ColorSchemeConfig primary2 = ColorSchemeConfig::build(ColorSchemeKind::Primary)
                                               .foreground("#000000")
                                               .background("#ffacff")
                                               .border("#fff781");

        ASSERT_EQ(draw.init_colorscheme(primary).is_ok(), true);
        ASSERT_EQ(draw.init_colorscheme(primary2).is_ok(), true);

        const auto opt = draw.get_colorscheme(ColorSchemeKind::Primary);
        ASSERT_EQ(opt.has_value(), true);
        const auto &colorscheme = opt.value();
        ASSERT_EQ(colorscheme.get_color(X11ColorScheme::Index::Foreground).raw()->pixel, 12303291);

        ASSERT_EQ(opt.has_value(), true);
    }
    else
    {
        testing::AssertionFailure() << draw_res.err()->msg();
    }
}

TEST(TileboxCoreX11DrawTestSuite, VerifyRemoveColorScheme)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();
    const Width screenw = dpy->screen_width();
    const Height screenh = dpy->screen_height();

    if (auto draw_res = X11Draw::create(dpy, screenw, screenh); draw_res.is_ok())
    {
        auto draw = std::move(*draw_res.ok());

        const ColorSchemeConfig primary = ColorSchemeConfig::build(ColorSchemeKind::Primary)
                                              .foreground("#bbbbbb")
                                              .background("#222222")
                                              .border("#444444");

        const ColorSchemeConfig secondary = ColorSchemeConfig::build(ColorSchemeKind::Secondary)
                                                .foreground("#000000")
                                                .background("#ffacff")
                                                .border("#fff781");

        const ColorSchemeConfig tertiary = ColorSchemeConfig::build(ColorSchemeKind::Tertiary)
                                               .foreground("#23451a")
                                               .background("#333333")
                                               .border("#444444");

        ASSERT_EQ(draw.init_colorscheme(primary).is_ok(), true);
        ASSERT_EQ(draw.init_colorscheme(secondary).is_ok(), true);
        ASSERT_EQ(draw.init_colorscheme(tertiary).is_ok(), true);

        ASSERT_EQ(draw.remove_colorscheme(ColorSchemeKind::Tertiary), true);
    }
    else
    {
        testing::AssertionFailure() << draw_res.err()->msg();
    }
}

TEST(TileboxCoreX11DrawTestSuite, VerifyInitCursor)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();
    const Width screenw = dpy->screen_width();
    const Height screenh = dpy->screen_height();

    if (auto draw_res = X11Draw::create(dpy, screenw, screenh); draw_res.is_ok())
    {
        auto draw = std::move(*draw_res.ok());

        ASSERT_EQ(draw.init_cursor(X11Cursor::Type::Normal).is_ok(), true);
        ASSERT_EQ(draw.init_cursor(X11Cursor::Type::Resize).is_ok(), true);
        ASSERT_EQ(draw.init_cursor(X11Cursor::Type::Move).is_ok(), true);

        ASSERT_EQ(draw.init_cursor(X11Cursor::Type::Normal).is_ok(), true);
        ASSERT_EQ(draw.init_cursor(X11Cursor::Type::Resize).is_ok(), true);
        ASSERT_EQ(draw.init_cursor(X11Cursor::Type::Move).is_ok(), true);
    }
    else
    {
        testing::AssertionFailure() << draw_res.err()->msg();
    }
}

TEST(TileboxCoreX11DrawTestSuite, VerifyGetCursor)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();
    const Width screenw = dpy->screen_width();
    const Height screenh = dpy->screen_height();

    if (auto draw_res = X11Draw::create(dpy, screenw, screenh); draw_res.is_ok())
    {
        auto draw = std::move(*draw_res.ok());

        ASSERT_EQ(draw.init_cursor(X11Cursor::Type::Normal).is_ok(), true);

        ASSERT_EQ(draw.get_cursor(X11Cursor::Type::Normal).has_value(), true);
        ASSERT_EQ(draw.get_cursor(X11Cursor::Type::Move).has_value(), false);
        ASSERT_EQ(draw.get_cursor(X11Cursor::Type::Resize).has_value(), false);
    }
    else
    {
        testing::AssertionFailure() << draw_res.err()->msg();
    }
}
