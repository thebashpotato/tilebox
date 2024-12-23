#include <gtest/gtest.h>

#include <tilebox/draw/cursor.hpp>
#include <tilebox/x11/display.hpp>

#include <utility>

using namespace Tilebox;

TEST(TileboxCoreX11CursorTestSuite, VerifyCursorStateSize)
{
    ASSERT_EQ(X11Cursor::GetUnderlyingSize(), 3);
}

TEST(TileboxCoreX11CursorTestSuite, VerifyCreation)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();

    const auto cursor_opt = X11Cursor::Create(dpy, X11Cursor::Type::Normal);
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

    auto cursor_ret = X11Cursor::Create(dpy, X11Cursor::Type::Normal);
    ASSERT_EQ(cursor_ret.is_ok(), true);

    auto cursor = *cursor_ret.ok();
    const auto cached_cursor_id = cursor.CursorId();

    ASSERT_NE(cached_cursor_id, 0);

    auto cursor_2 = std::move(cursor);
    ASSERT_EQ(cursor_2.CursorId(), cached_cursor_id);
}

TEST(TileboxCoreX11CursorTestSuite, VerifyMoveAssignment)
{
    auto dpy_opt = X11Display::create();

    if (!dpy_opt.has_value())
    {
        testing::AssertionFailure() << "Could not open x11 display";
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();

    auto cursor_ret = X11Cursor::Create(dpy, X11Cursor::Type::Normal);
    ASSERT_EQ(cursor_ret.is_ok(), true);

    auto cursor = *cursor_ret.ok();
    const auto cached_cursor_id = cursor.CursorId();

    ASSERT_NE(cached_cursor_id, 0);

    cursor_ret = X11Cursor::Create(dpy, X11Cursor::Type::Resize);
    ASSERT_EQ(cursor_ret.is_ok(), true);

    auto cursor2 = *cursor_ret.ok();

    cursor2 = std::move(cursor);
    ASSERT_EQ(cursor2.CursorId(), cached_cursor_id);
}
