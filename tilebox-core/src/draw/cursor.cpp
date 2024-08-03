#include "tilebox-core/draw/cursor.hpp"
#include "tilebox-core/error.hpp"
#include "tilebox-core/vendor/etl.hpp"
#include "tilebox-core/x11/display.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <string>
#include <utility>

using namespace tilebox::core;
using namespace etl;

auto X11Cursor::create(const X11DisplaySharedResource &dpy,
                       const X11Cursor::State state) noexcept -> Result<X11Cursor, X11CursorError>
{
    // Cursor is a typedef of XID, which is just a uint64_t. In general it should never be 0 or False.
    // Although this is not how the X server actually handles errors, it does let us know that something non ideal
    // has happend.
    if (const Cursor cursor = XCreateFontCursor(dpy->raw(), X11Cursor::to_cursor_font(state)); cursor == False)
    {
        return Result<X11Cursor, X11CursorError>(X11CursorError(
            std::string("XCreateFontCursor: Could not create cursor ").append(X11Cursor::to_string(state)),
            RUNTIME_INFO));
    }
    else
    {
        return Result<X11Cursor, X11CursorError>(X11Cursor(dpy, cursor));
    }
}

auto X11Cursor::cursor() const noexcept -> Cursor
{
    return _cursor;
}

X11Cursor::X11Cursor(X11DisplaySharedResource dpy, const Cursor cursor) noexcept : _dpy(std::move(dpy)), _cursor(cursor)
{
}

X11Cursor::~X11Cursor()
{
    if (_cursor != False && _dpy->is_connected())
    {
        XFreeCursor(_dpy->raw(), _cursor);
        _cursor = False;
    }
}

X11Cursor::X11Cursor(X11Cursor &&rhs) noexcept : _dpy(std::move(rhs._dpy)), _cursor(rhs._cursor)
{
    rhs._cursor = False;
}

auto X11Cursor::operator=(X11Cursor &&rhs) noexcept -> X11Cursor &
{
    if (this != &rhs)
    {
        _dpy = std::move(rhs._dpy);
        _cursor = rhs._cursor;
        rhs._cursor = False;
    }
    return *this;
}
