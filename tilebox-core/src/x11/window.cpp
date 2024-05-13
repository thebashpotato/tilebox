#include "tilebox-core/x11/window.hpp"
#include "tilebox-core/geometry.hpp"
#include "tilebox-core/x11/display.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>

namespace tilebox::core::x
{

X11Window::X11Window(X11Display &display) noexcept : _dpy(display)
{
}

X11Window::~X11Window()
{
    unmap();
    if (_id != 0 && _dpy.is_connected())
    {
        // TODO: Check return codes
        XDestroyWindow(_dpy.raw(), _id);
        _id = 0;
    }
}

auto X11Window::id() const noexcept -> Window
{
    return _id;
}

auto X11Window::create_window(const Rect &r) noexcept -> bool
{
    XSetWindowAttributes wa;
    wa.background_pixel = BlackPixel(_dpy.raw(), _dpy.screen_id());
    wa.border_pixel = WhitePixel(_dpy.raw(), _dpy.screen_id());
    wa.event_mask = ButtonPress;

    if (!_is_mapped)
    {
        _id = XCreateWindow(
            _dpy.raw(), _dpy.root_window(), r.x(), r.y(), r.w(), r.h(), 0, DefaultDepth(_dpy.raw(), _dpy.screen_id()),
            InputOutput, DefaultVisual(_dpy.raw(), _dpy.screen_id()), CWBackPixel | CWBorderPixel | CWEventMask, &wa);

        return _id != BadAlloc && _id != BadMatch && _id != BadValue && _id != BadWindow;
    }

    return false;
}

auto X11Window::map() noexcept -> bool
{
    if (!_is_mapped)
    {
        if (XMapWindow(_dpy.raw(), _id) == BadWindow)
        {
            return _is_mapped;
        }
        _is_mapped = true;
    }
    return _is_mapped;
}

auto X11Window::unmap() const noexcept -> void
{
    if (_is_mapped)
    {
        XUnmapWindow(_dpy.raw(), _id);
    }
}

} // namespace tilebox::core::x
