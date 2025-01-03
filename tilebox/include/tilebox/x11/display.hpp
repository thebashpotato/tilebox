#pragma once

#include "tilebox/geometry.hpp"
#include "tilebox/utils/attributes.hpp"

#include <X11/X.h>
#include <X11/Xlib.h>

#include <memory>
#include <optional>
#include <string>

namespace Tilebox
{

struct TILEBOX_INTERNAL DisplayDeleter
{
    auto operator()(Display *display) const noexcept -> void;
};

class X11Display;
using X11DisplaySharedResource = std::shared_ptr<X11Display>;

/// @brief Provides a RAII interface for common functionality needed from the X11 Display.
/// @details Manages the underlying Display * via a shared_ptr.
class TILEBOX_EXPORT X11Display
{
  public:
    ~X11Display() = default;
    X11Display(X11Display &&rhs) noexcept = default;
    X11Display(const X11Display &rsh) noexcept = default;

  public:
    auto operator=(X11Display &&rhs) noexcept -> X11Display & = default;
    auto operator=(const X11Display &rhs) noexcept -> X11Display & = default;

  public:
    /// @brief Static creation method for the shared display resource, this is to guarantee the resource is built
    /// correctly, and shared with all other objects that need a display connection (there are many) as a shallow
    /// pointer copy.
    ///
    /// @param display_name Specifies the hardware display name, which determines the display and communications
    /// domain to be used. On a POSIX-conformant system, if the display_name is NULL, it defaults to the value of the
    /// DISPLAY environment variable.
    ///
    /// @return std::nullopt if the display was unable to connect.
    [[nodiscard]] static auto Create(const std::optional<std::string> &display_name = std::nullopt)
        -> std::optional<X11DisplaySharedResource>;

    /// @brief Check to see if the underlying display is connected to the X server.
    ///
    /// @details Should be used after creating the connection to check for failure.
    ///
    /// @return true if connected, false otherwise.
    [[nodiscard]] auto IsConnected() const noexcept -> bool;

    /// @brief Gets the raw Display pointer
    [[nodiscard]] auto Raw() const noexcept -> Display *;

    /// @brief Refreshes all internal display values.
    auto Refresh() noexcept -> void;

    /// @brief Gets the active screen id
    [[nodiscard]] auto ScreenId() const noexcept -> std::int32_t;

    /// @brief Gets the raw screen width as reported from the X server
    [[nodiscard]] auto ScreenWidthRaw() const noexcept -> std::int32_t;

    /// @brief Gets the raw screen height as reported from the X server
    [[nodiscard]] auto ScreenHeightRaw() const noexcept -> std::int32_t;

    /// @brief Gets the screen width for use in geometry objects
    [[nodiscard]] auto ScreenWidth() const noexcept -> Width;

    /// @brief Gets the screen height for use in geometry objects
    [[nodiscard]] auto ScreenHeight() const noexcept -> Height;

    /// @brief Gets the number of screens
    [[nodiscard]] auto GetScreenCount() const noexcept -> std::int32_t;

    /// @brief Gets default root window
    [[nodiscard]] auto GetDefaultRootWindow() const noexcept -> Window;

    /// @brief Gets root window based on the current screen id
    [[nodiscard]] auto GetRootWindow() const noexcept -> Window;

    /// @brief Provides details about the X server implementation that is running
    [[nodiscard]] auto GetServerVendor() const noexcept -> std::string;

    /// @brief flushes the output buffer and then waits until all requests have been received and processed by the X
    /// server.
    ///
    /// @details Should be called before shutdown, calls `XSync`.
    ///
    /// @param discard Specifies whether the X server should discard all events in the event queue.
    auto Sync(const bool discard = false) const noexcept -> void;

  private:
    explicit X11Display(const std::optional<std::string> &display_name) noexcept;

  private:
    std::shared_ptr<Display> m_dpy;
    std::int32_t m_screen_id{};
    std::int32_t m_screen_width{};
    std::int32_t m_screen_height{};
    std::int32_t m_screen_count{};
    Window m_default_root_window{};
    Window m_root_window{};
    std::string m_server_vendor;
};

} // namespace Tilebox
