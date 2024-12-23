#include "tilebox/draw/cursor.hpp"
#include "tilebox/error.hpp"
#include "tilebox/vendor/etl.hpp"
#include "tilebox/x11/display.hpp"

#include <X11/X.h>
#include <X11/Xlib.h>

#include <optional>
#include <string>
#include <utility>

using namespace etl;

namespace Tilebox
{

auto X11Cursor::Create(const X11DisplaySharedResource &dpy,
                       const Type type) noexcept -> Result<X11Cursor, X11CursorError>
{
    // Cursor is a typedef of XID, which is just an uint64_t. In general, it should never be 0 or False.
    // Although this is not how the X server actually handles errors, it does let us know that something non-ideal
    // has happened.
    if (const Cursor cursor = XCreateFontCursor(dpy->raw(), ToCursorFont(type)); cursor == False)
    {
        return Result<X11Cursor, X11CursorError>(X11CursorError(
            std::string("XCreateFontCursor: Could not create cursor ").append(ToString(type)), RUNTIME_INFO));
    }
    else
    {
        return Result<X11Cursor, X11CursorError>(X11Cursor(dpy, type, cursor));
    }
}

auto X11Cursor::CursorId() const noexcept -> Cursor
{
    return _cursor;
}

auto X11Cursor::type() const noexcept -> std::optional<Type>
{
    return _type;
}

X11Cursor::X11Cursor(X11DisplaySharedResource dpy, const Type type, const Cursor cursor) noexcept
    : _dpy(std::move(dpy)), _type(type), _cursor(cursor)
{
}

X11Cursor::~X11Cursor()
{
    if (_cursor != False && _dpy->is_connected())
    {
        XFreeCursor(_dpy->raw(), _cursor);
        _type.reset();
        _cursor = False;
    }
}

X11Cursor::X11Cursor(X11Cursor &&rhs) noexcept : _dpy(std::move(rhs._dpy)), _type(rhs._type), _cursor(rhs._cursor)
{
    rhs._type.reset();
    rhs._cursor = False;
}

auto X11Cursor::operator=(X11Cursor &&rhs) noexcept -> X11Cursor &
{
    if (this != &rhs)
    {
        _dpy = std::move(rhs._dpy);

        _type = rhs._type;
        rhs._type.reset();

        _cursor = rhs._cursor;
        rhs._cursor = False;
    }
    return *this;
}

} // namespace Tilebox
