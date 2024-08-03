#include <gtest/gtest.h>
#include <tilebox-core/draw/cursor.hpp>
#include <tilebox-core/x11/display.hpp>
#include <utility>

using namespace tilebox::core;

TEST(TileboxCoreX11CursorTestSuite, VerifyCursorStateSize)
{
    ASSERT_EQ(X11Cursor::state_size(), 3);
}

TEST(TileboxCoreX11CursorTestSuite, VerifyCreation)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();

    const auto cursor_opt = X11Cursor::create(dpy, X11Cursor::State::Normal);
    ASSERT_EQ(cursor_opt.is_ok(), true);
}

TEST(TileboxCoreX11CursorTestSuite, VerifyMoveConstructor)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();

    auto cursor_ret = X11Cursor::create(dpy, X11Cursor::State::Normal);
    ASSERT_EQ(cursor_ret.is_ok(), true);

    auto cursor = *cursor_ret.ok();
    const auto cached_cursor_id = cursor.cursor();

    ASSERT_NE(cached_cursor_id, 0);

    auto cursor_2 = std::move(cursor);
    ASSERT_EQ(cursor_2.cursor(), cached_cursor_id);
}

TEST(TileboxCoreX11CursorTestSuite, VerifyMoveAssignment)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();

    auto cursor_ret = X11Cursor::create(dpy, X11Cursor::State::Normal);
    ASSERT_EQ(cursor_ret.is_ok(), true);

    auto cursor = *cursor_ret.ok();
    const auto cached_cursor_id = cursor.cursor();

    ASSERT_NE(cached_cursor_id, 0);

    cursor_ret = X11Cursor::create(dpy, X11Cursor::State::Resize);
    ASSERT_EQ(cursor_ret.is_ok(), true);

    auto cursor2 = *cursor_ret.ok();

    cursor2 = std::move(cursor);
    ASSERT_EQ(cursor2.cursor(), cached_cursor_id);
}
