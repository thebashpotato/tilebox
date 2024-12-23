#include "tilebox/x11/window.hpp"
#include "tilebox/geometry.hpp"
#include "tilebox/x11/display.hpp"

#include <X11/X.h>
#include <X11/Xlib.h>

#include <utility>

namespace Tilebox
{

X11Window::X11Window(X11DisplaySharedResource dpy) noexcept : m_dpy(std::move(dpy))
{
}

X11Window::~X11Window()
{
    Unmap();
    if (m_id != 0 && m_dpy->IsConnected())
    {
        // TODO: Check return codes
        XDestroyWindow(m_dpy->Raw(), m_id);
        m_id = 0;
    }
}

auto X11Window::id() const noexcept -> Window
{
    return m_id;
}

auto X11Window::Create(const Rect &r) noexcept -> bool
{
    XSetWindowAttributes wa;
    wa.background_pixel = BlackPixel(m_dpy->Raw(), m_dpy->ScreenId());
    wa.border_pixel = WhitePixel(m_dpy->Raw(), m_dpy->ScreenId());
    wa.event_mask = ButtonPress | DestroyNotify;

    if (!m_is_mapped)
    {
        m_id = XCreateWindow(m_dpy->Raw(), m_dpy->GetRootWindow(), r.GetX(), r.GetY(), r.GetW(), r.GetH(), 0,
                             DefaultDepth(m_dpy->Raw(), m_dpy->ScreenId()), InputOutput,
                             DefaultVisual(m_dpy->Raw(), m_dpy->ScreenId()), CWBackPixel | CWBorderPixel | CWEventMask,
                             &wa);

        return m_id != BadAlloc && m_id != BadMatch && m_id != BadValue && m_id != BadWindow;
    }

    return false;
}

auto X11Window::Map() noexcept -> bool
{
    if (!m_is_mapped)
    {
        if (XMapWindow(m_dpy->Raw(), m_id) == BadWindow)
        {
            return m_is_mapped;
        }
        m_is_mapped = true;
    }
    return m_is_mapped;
}

auto X11Window::Unmap() const noexcept -> void
{
    if (m_is_mapped)
    {
        XUnmapWindow(m_dpy->Raw(), m_id);
    }
}

} // namespace Tilebox
