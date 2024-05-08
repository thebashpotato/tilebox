#include <X11/X.h>
#include <X11/Xlib.h>
#include <cstdint>
#include <memory>
#include <string>
#include <tilebox-core/x11/display.hpp>

namespace tilebox::core::x
{

X11Display::X11Display(const std::string &display_name) noexcept
    : _display(XOpenDisplay(display_name.empty() ? nullptr : display_name.c_str()), [](Display *display) {
          if (display != nullptr)
          {
              XSync(display, False);
              XCloseDisplay(display);
          }
      })
{
    this->refresh();
}

auto X11Display::is_connected() const noexcept -> bool
{
    return _display != nullptr;
}

auto X11Display::get_shared() const noexcept -> std::shared_ptr<Display>
{
    return _display;
}

auto X11Display::get_raw() const noexcept -> Display *
{
    return _display.get();
}

auto X11Display::refresh() -> void
{
    if (this->is_connected())
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

} // namespace tilebox::core::x
