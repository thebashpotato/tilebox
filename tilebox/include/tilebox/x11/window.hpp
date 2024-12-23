#pragma once

#include "tilebox/utils/attributes.hpp"
#include "tilebox/x11/display.hpp"

#include <X11/X.h>

namespace Tilebox
{

class Rect;

class TILEBOX_EXPORT X11Window
{
  public:
    explicit X11Window(X11DisplaySharedResource dpy) noexcept;
    ~X11Window();
    X11Window(const X11Window &rhs) noexcept = default;
    X11Window(X11Window &&rhs) noexcept = default;

  public:
    auto operator=(const X11Window &rhs) noexcept -> X11Window & = default;
    auto operator=(X11Window &&rhs) noexcept -> X11Window & = default;

  public:
    /// @brief Fetch the underlying window id
    [[nodiscard]] auto id() const noexcept -> Window;

    /// @brief Creates a new window using the Xlib `XCreateWindow` function.
    ///
    /// @details This will create an `X11CreateNotify` even.
    ///
    /// @error The following Xlib errors can cause failure.
    ///     `BadAlloc`, `BadMatch`, `BadValue`, and `BadWindow`.
    [[nodiscard]] auto Create(const Rect &r) noexcept -> bool;

    /// @brief Maps a window using the Xlib `XMapWindow` function.
    ///
    /// @details Only maps the window iff it is not already mapped, and if it is not
    /// the screen_id window.
    ///
    /// @returns false if `BadWindow` error code is received from `XMapWindow`,
    /// true if Success
    [[nodiscard]] auto Map() noexcept -> bool;

    /// @brief Wrapper around XUnmapWindow
    ///
    /// @details Does not free any memory e.g. XDestroyWindow is not called,
    /// that happens in the Destructor
    auto Unmap() const noexcept -> void;

  private:
    X11DisplaySharedResource m_dpy;
    bool m_is_mapped{};
    Window m_id{};
};

} // namespace Tilebox
