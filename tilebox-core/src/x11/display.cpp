#include "tilebox-core/x11/display.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>

using namespace tilebox::core;

auto DisplayDeleter::operator()(Display *display) const noexcept -> void
{
    if (display != nullptr)
    {
        XCloseDisplay(display);
        display = nullptr;
    }
}

X11Display::X11Display(const std::optional<std::string> &display_name) noexcept
    : _dpy(XOpenDisplay(display_name.has_value() ? display_name.value().c_str() : nullptr), DisplayDeleter())
{
    refresh();
}

auto X11Display::create(const std::optional<std::string> &display_name) -> std::optional<X11DisplaySharedResource>
{
    std::optional<X11DisplaySharedResource> ret;
    auto display = std::shared_ptr<X11Display>(new X11Display(display_name));
    if (display->is_connected())
    {
        ret.emplace(display);
    }
    return ret;
}

auto X11Display::is_connected() const noexcept -> bool
{
    return _dpy != nullptr;
}

auto X11Display::raw() const noexcept -> Display *
{
    return _dpy.get();
}

auto X11Display::refresh() noexcept -> void
{
    if (is_connected())
    {
        _screen_id = DefaultScreen(_dpy.get());
        _screen_width = DisplayWidth(_dpy.get(), _screen_id);
        _screen_height = DisplayHeight(_dpy.get(), _screen_id);
        _screen_count = ScreenCount(_dpy.get());
        _default_root_window = DefaultRootWindow(_dpy.get());
        _root_window = RootWindow(_dpy.get(), _screen_id);
        _server_vendor = XServerVendor(_dpy.get());
    }
}

auto X11Display::screen_id() const noexcept -> std::int32_t
{
    return _screen_id;
}

auto X11Display::screen_width() const noexcept -> std::int32_t
{
    return _screen_width;
}

auto X11Display::screen_height() const noexcept -> std::int32_t
{
    return _screen_height;
}

auto X11Display::screen_count() const noexcept -> std::int32_t
{
    return _screen_count;
}

auto X11Display::default_root_window() const noexcept -> Window
{
    return _default_root_window;
}

auto X11Display::root_window() const noexcept -> Window
{
    return _root_window;
}

auto X11Display::server_vendor() const noexcept -> std::string
{
    return _server_vendor;
}

auto X11Display::sync(const bool discard) const noexcept -> void
{
    if (is_connected())
    {
        const auto xdiscard = discard ? True : False;
        XSync(_dpy.get(), xdiscard);
    }
}
