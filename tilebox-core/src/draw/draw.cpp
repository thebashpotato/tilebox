#include "tilebox-core/draw/draw.hpp"
#include "tilebox-core/geometry.hpp"
#include "tilebox-core/x11/display.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <utility>

using namespace etl;
using namespace tilebox::core;

X11Draw::X11Draw(X11DisplaySharedResource dpy, Width width, Height height) noexcept
    : _dpy(std::move(dpy)), _graphics_ctx(XCreateGC(_dpy->raw(), _dpy->root_window(), 0, nullptr)),
      _width(std::move(width)), _height(std::move(height))
{
    _drawable = XCreatePixmap(_dpy->raw(), _dpy->root_window(), _width.value, _height.value,
                              DefaultDepth(_dpy->raw(), _dpy->screen_id()));

    XSetLineAttributes(_dpy->raw(), _graphics_ctx, 1, LineSolid, CapButt, JoinMiter);
}

X11Draw::~X11Draw() noexcept
{
    _fonts.clear();

    if (_drawable != False)
    {
        XFreePixmap(_dpy->raw(), _drawable);
        _drawable = False;
    }

    if (_graphics_ctx != nullptr)
    {
        XFreeGC(_dpy->raw(), _graphics_ctx);
        _graphics_ctx = nullptr;
    }
}

X11Draw::X11Draw(X11Draw &&rhs) noexcept
    : _dpy(std::move(rhs._dpy)), _fonts(std::move(rhs._fonts)), _drawable(rhs._drawable),
      _graphics_ctx(rhs._graphics_ctx), _width(std::move(rhs._width)), _height(std::move(rhs._height))
{
    rhs._drawable = False;
    rhs._graphics_ctx = nullptr;
}

auto X11Draw::operator=(X11Draw &&rhs) noexcept -> X11Draw &
{
    if (this != &rhs)
    {
        _height = std::move(rhs._height);
        _width = std::move(rhs._width);

        if (_graphics_ctx != nullptr)
        {
            XFreeGC(_dpy->raw(), _graphics_ctx);
        }
        _graphics_ctx = rhs._graphics_ctx;

        if (_drawable != False)
        {
            XFreePixmap(_dpy->raw(), _drawable);
        }
        _drawable = rhs._drawable;

        if (!_fonts.empty())
        {
            _fonts.clear();
        }

        _fonts = std::move(rhs._fonts);
        _dpy = std::move(rhs._dpy);
    }

    return *this;
}

auto X11Draw::resize(const Width &width, const Height &height) noexcept -> void
{
    _width = width;
    _height = height;
    if (_drawable != False)
    {
        XFreePixmap(_dpy->raw(), _drawable);
    }
    _drawable = XCreatePixmap(_dpy->raw(), _dpy->root_window(), width.value, height.value,
                              DefaultDepth(_dpy->raw(), _dpy->screen_id()));
}
