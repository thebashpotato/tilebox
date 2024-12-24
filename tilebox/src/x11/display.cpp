#include "tilebox/x11/display.hpp"
#include "tilebox/geometry.hpp"

#include <X11/X.h>
#include <X11/Xlib.h>

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <utility>

namespace Tilebox
{

auto DisplayDeleter::operator()(Display *display) const noexcept -> void
{
    if (display != nullptr)
    {
        XCloseDisplay(display);
    }
}

X11Display::X11Display(const std::optional<std::string> &display_name) noexcept
    : m_dpy(XOpenDisplay(display_name.has_value() ? display_name->c_str() : nullptr), DisplayDeleter())
{
    Refresh();
}

auto X11Display::Create(const std::optional<std::string> &display_name) -> std::optional<X11DisplaySharedResource>
{
    std::optional<X11DisplaySharedResource> ret;
    if (auto display = std::shared_ptr<X11Display>(new X11Display(display_name)); display->IsConnected())
    {
        ret.emplace(std::move(display));
    }
    return ret;
}

auto X11Display::IsConnected() const noexcept -> bool
{
    return m_dpy != nullptr;
}

auto X11Display::Raw() const noexcept -> Display *
{
    return m_dpy.get();
}

auto X11Display::Refresh() noexcept -> void
{
    if (IsConnected())
    {
        m_screen_id = DefaultScreen(m_dpy.get());
        m_screen_width = DisplayWidth(m_dpy.get(), m_screen_id);
        m_screen_height = DisplayHeight(m_dpy.get(), m_screen_id);
        m_screen_count = ScreenCount(m_dpy.get());
        m_default_root_window = DefaultRootWindow(m_dpy.get());
        m_root_window = RootWindow(m_dpy.get(), m_screen_id);
        m_server_vendor = XServerVendor(m_dpy.get());
    }
}

auto X11Display::ScreenId() const noexcept -> std::int32_t
{
    return m_screen_id;
}

auto X11Display::ScreenWidthRaw() const noexcept -> std::int32_t
{
    return m_screen_width;
}

auto X11Display::ScreenHeightRaw() const noexcept -> std::int32_t
{
    return m_screen_height;
}

auto X11Display::ScreenWidth() const noexcept -> Width
{
    return Width(static_cast<uint32_t>(m_screen_width));
}

auto X11Display::ScreenHeight() const noexcept -> Height
{
    return Height(static_cast<uint32_t>(m_screen_height));
}

auto X11Display::GetScreenCount() const noexcept -> std::int32_t
{
    return m_screen_count;
}

auto X11Display::GetDefaultRootWindow() const noexcept -> Window
{
    return m_default_root_window;
}

auto X11Display::GetRootWindow() const noexcept -> Window
{
    return m_root_window;
}

auto X11Display::GetServerVendor() const noexcept -> std::string
{
    return m_server_vendor;
}

auto X11Display::Sync(const bool discard) const noexcept -> void
{
    if (IsConnected())
    {
        const auto xdiscard = discard ? True : False;
        XSync(m_dpy.get(), xdiscard);
    }
}

} // namespace Tilebox
