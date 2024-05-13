#include "tilebox-core/x11/display.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>

namespace tilebox::core::x
{

X11Display::X11Display(const std::optional<std::string> &display_name) noexcept
    : _display(XOpenDisplay(display_name.has_value() ? display_name.value().c_str() : nullptr), [](Display *display) {
          if (display != nullptr)
          {
              XCloseDisplay(display);
          }
      })
{
    refresh();
}

auto X11Display::is_connected() const noexcept -> bool
{
    return _display != nullptr;
}

auto X11Display::shared() const noexcept -> std::shared_ptr<Display>
{
    return _display;
}

auto X11Display::raw() const noexcept -> Display *
{
    return _display.get();
}

auto X11Display::refresh() -> void
{
    if (is_connected())
    {
        _screen_id = DefaultScreen(_display.get());
        _screen_width = DisplayWidth(_display.get(), _screen_id);
        _screen_height = DisplayHeight(_display.get(), _screen_id);
        _screen_count = ScreenCount(_display.get());
        _default_root_window = DefaultRootWindow(_display.get());
        _root_window = RootWindow(_display.get(), _screen_id);
        _server_vendor = XServerVendor(_display.get());
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

auto X11Display::sync(bool discard) const noexcept -> void
{
    if (is_connected())
    {
        const auto xdiscard = discard ? True : False;
        XSync(_display.get(), xdiscard);
    }
}

} // namespace tilebox::core::x
