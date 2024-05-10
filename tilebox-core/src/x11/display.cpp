#include "tilebox-core/x11/display.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <cstdint>
#include <memory>
#include <string>

namespace tilebox::core::x
{

X11Display::X11Display(const std::string &display_name) noexcept
    : _display(XOpenDisplay(display_name.empty() ? nullptr : display_name.c_str()), [](Display *display) {
          if (display != nullptr)
          {
              XCloseDisplay(display);
          }
      })
{
    this->refresh();
}

auto X11Display::is_connected() const noexcept -> bool
{
    return this->_display != nullptr;
}

auto X11Display::get_shared() const noexcept -> std::shared_ptr<Display>
{
    return this->_display;
}

auto X11Display::get_raw() const noexcept -> Display *
{
    return this->_display.get();
}

auto X11Display::refresh() -> void
{
    if (this->is_connected())
    {
        this->_screen_id = DefaultScreen(this->_display.get());
        this->_screen_width = DisplayWidth(this->_display.get(), this->_screen_id);
        this->_screen_height = DisplayHeight(this->_display.get(), this->_screen_id);
        this->_screen_count = ScreenCount(this->_display.get());
        this->_default_root_window = DefaultRootWindow(this->_display.get());
        this->_root_window = RootWindow(this->_display.get(), this->_screen_id);
        this->_server_vendor = XServerVendor(this->_display.get());
    }
}

auto X11Display::screen_id() const noexcept -> std::int32_t
{
    return this->_screen_id;
}

auto X11Display::screen_width() const noexcept -> std::int32_t
{
    return this->_screen_width;
}

auto X11Display::screen_height() const noexcept -> std::int32_t
{
    return this->_screen_height;
}

auto X11Display::screen_count() const noexcept -> std::int32_t
{
    return this->_screen_count;
}

auto X11Display::default_root_window() const noexcept -> Window
{
    return this->_default_root_window;
}

auto X11Display::root_window() const noexcept -> Window
{
    return this->_root_window;
}

auto X11Display::server_vendor() const noexcept -> std::string
{
    return this->_server_vendor;
}

} // namespace tilebox::core::x
