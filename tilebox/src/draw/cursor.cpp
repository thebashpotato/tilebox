#include "tilebox/draw/cursor.hpp"
#include "tilebox/error.hpp"
#include "tilebox/x11/display.hpp"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <etl.hpp>

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
    if (const Cursor cursor = XCreateFontCursor(dpy->Raw(), ToCursorFont(type)); cursor == False)
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
    return m_cursor;
}

auto X11Cursor::type() const noexcept -> std::optional<Type>
{
    return m_type;
}

X11Cursor::X11Cursor(X11DisplaySharedResource dpy, const Type type, const Cursor cursor) noexcept
    : m_dpy(std::move(dpy)), m_type(type), m_cursor(cursor)
{
}

X11Cursor::~X11Cursor()
{
    if (m_cursor != False && m_dpy->IsConnected())
    {
        XFreeCursor(m_dpy->Raw(), m_cursor);
        m_type.reset();
        m_cursor = False;
    }
}

X11Cursor::X11Cursor(X11Cursor &&rhs) noexcept : m_dpy(std::move(rhs.m_dpy)), m_type(rhs.m_type), m_cursor(rhs.m_cursor)
{
    rhs.m_type.reset();
    rhs.m_cursor = False;
}

auto X11Cursor::operator=(X11Cursor &&rhs) noexcept -> X11Cursor &
{
    if (this != &rhs)
    {
        m_dpy = std::move(rhs.m_dpy);

        m_type = rhs.m_type;
        rhs.m_type.reset();

        m_cursor = rhs.m_cursor;
        rhs.m_cursor = False;
    }
    return *this;
}

} // namespace Tilebox
