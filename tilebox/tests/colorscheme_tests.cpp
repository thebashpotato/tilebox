#include <gtest/gtest.h>

#include <tilebox/draw/color.hpp>
#include <tilebox/draw/colorscheme.hpp>
#include <tilebox/draw/colorscheme_config.hpp>
#include <tilebox/x11/display.hpp>

#include <utility>

using namespace Tilebox;

TEST(TileboxCoreColorschemeTestSuite, VerifyColorCreation)
{
    auto dpy_opt = X11Display::Create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource &dpy = std::move(dpy_opt.value());

    const auto color_res = X11Color::Create(dpy, "#000000");
    ASSERT_EQ(color_res.is_ok(), true);

    const X11Color color = std::move(*color_res.ok());
    ASSERT_NE(color.Raw(), nullptr);
}

TEST(TileboxCoreColorschemeTestSuite, VerifyColorSchemeSizeFunction)
{
    ASSERT_EQ(ColorSchemeKindIterator::size(), 3);
}

TEST(TileboxCoreColorschemeTestSuite, VerifyColorMoveContructor)
{
    auto dpy_opt = X11Display::Create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = std::move(dpy_opt.value());

    const auto color_res = X11Color::Create(dpy, "#000000");
    ASSERT_EQ(color_res.is_ok(), true);

    X11Color color = std::move(*color_res.ok());

    auto color_2 = std::move(color);
    ASSERT_NE(color_2.Raw(), nullptr);
}

TEST(TileboxCoreColorschemeTestSuite, VerifyColorCopyConstructor)
{
    auto dpy_opt = X11Display::Create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = std::move(dpy_opt.value());

    const auto color_res = X11Color::Create(dpy, "#000000");
    ASSERT_EQ(color_res.is_ok(), true);

    const X11Color color = std::move(*color_res.ok());

    {
        const auto color_2 = color; // NOLINT
        ASSERT_EQ(color.Raw(), color.Raw());
    }

    ASSERT_NE(color.Raw(), nullptr);
}

TEST(TileboxCoreColorschemeTestSuite, VerifyColorSchemeConfigBuilder)
{
    const ColorSchemeConfig primary = ColorSchemeConfig::Build(ColorSchemeKind::Primary)
                                          .foreground("#bbbbbb")
                                          .background("#222222")
                                          .border("#444444");

    EXPECT_EQ(primary.kind(), ColorSchemeKind::Primary);
    EXPECT_EQ(primary.foreground(), "#bbbbbb");
    EXPECT_EQ(primary.background(), "#222222");
    EXPECT_EQ(primary.border(), "#444444");
}

TEST(TileboxCoreColorschemeTestSuite, VerifyColorSchemeCreation)
{
    auto dpy_opt = X11Display::Create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = std::move(dpy_opt.value());

    const ColorSchemeConfig primary = ColorSchemeConfig::Build(ColorSchemeKind::Primary)
                                          .foreground("#bbbbbb")
                                          .background("#222222")
                                          .border("#444444");

    const auto scheme_res = X11ColorScheme::Create(dpy, primary);

    ASSERT_EQ(scheme_res.is_ok(), true);
}

TEST(TileboxCoreColorschemeTestSuite, VerifyColorAccessors)
{
    auto dpy_opt = X11Display::Create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = std::move(dpy_opt.value());

    const ColorSchemeConfig primary = ColorSchemeConfig::Build(ColorSchemeKind::Primary)
                                          .foreground("#bbbbbb")
                                          .background("#222222")
                                          .border("#444444");

    if (const auto scheme_res = X11ColorScheme::Create(dpy, primary); scheme_res.is_ok())
    {
        const X11ColorScheme scheme = *scheme_res.ok();

        const X11Color &fg = scheme.GetColor(X11ColorScheme::Type::Foreground);
        const X11Color &bg = scheme.GetColor(X11ColorScheme::Type::Background);
        const X11Color &border = scheme.GetColor(X11ColorScheme::Type::Border);

        ASSERT_NE(fg.Raw(), nullptr);
        ASSERT_NE(bg.Raw(), nullptr);
        ASSERT_NE(border.Raw(), nullptr);

        ASSERT_EQ(fg.Raw()->pixel, 12303291);
        ASSERT_EQ(bg.Raw()->pixel, 2236962);
        ASSERT_EQ(border.Raw()->pixel, 4473924);
    }
    else
    {
        testing::AssertionFailure() << scheme_res.err()->info() << '\n';
    }
}
