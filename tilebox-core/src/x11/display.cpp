#include <X11/X.h>
#include <X11/Xlib.h>
#include <cstdint>
#include <memory>
#include <string>
#include <tilebox-core/x11/display.hpp>

namespace tilebox::core
{

X11Display::X11Display(const std::string &display_name) noexcept
    : _display(XOpenDisplay(display_name.empty() ? nullptr : display_name.c_str()), [](Display *display) {
          if (display != nullptr)
          {
              XCloseDisplay(display);
          }
      })
{
}

auto X11Display::create(const std::string &display_name) -> std::shared_ptr<X11Display>
{
    return std::shared_ptr<X11Display>(new X11Display(display_name));
}

auto X11Display::is_connected() const noexcept -> bool
{
    return _display != nullptr;
}

auto X11Display::get_raw() const noexcept -> Display *
{
    return _display.get();
}

auto X11Display::get_shared() -> std::shared_ptr<X11Display>
{
    return shared_from_this();
}

auto X11Display::screen_id() const noexcept -> std::int32_t
{
    return DefaultScreen(_display.get());
}

auto X11Display::screen_count() const noexcept -> std::int32_t
{
    return XScreenCount(_display.get());
}

auto X11Display::default_root_window() const noexcept -> Window
{
    return DefaultRootWindow(_display.get());
}

auto X11Display::root_window() const noexcept -> Window
{
    return RootWindow(_display.get(), this->screen_id());
}

auto X11Display::server_vendor() const noexcept -> std::string
{
    return XServerVendor(_display.get());
}

} // namespace tilebox::core
