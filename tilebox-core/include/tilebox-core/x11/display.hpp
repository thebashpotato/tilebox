#pragma once

#include <X11/X.h>
#include <X11/Xlib.h>
#include <cstdint>
#include <memory>
#include <string>
#include <tilebox-core/utils/attributes.hpp>

namespace tilebox::core::x
{

/// @brief Provides a RAII interface for common functionaility needed from the X11 Display.
/// @details Manages the underlying Display * via a shared_ptr.
class TILEBOX_EXPORT X11Display
{
  private:
    std::shared_ptr<Display> _display;
    std::int32_t _screen_id{};
    std::int32_t _screen_width{};
    std::int32_t _screen_height{};
    std::int32_t _screen_count{};
    Window _default_root_window{};
    Window _root_window{};
    std::string _server_vendor{};

  public:
    /// @param `display_name` Specifies the hardware display name, which determines the display and communications
    /// domain to be used. On a POSIX-conformant system, if the display_name is NULL, it defaults to the value of the
    /// DISPLAY environment variable.
    explicit X11Display(const std::string &display_name = "") noexcept;

    virtual ~X11Display() = default;

    X11Display(X11Display &&other) noexcept = default;
    auto operator=(X11Display &&other) noexcept -> X11Display & = default;
    X11Display(const X11Display &other) noexcept = default;
    auto operator=(const X11Display &other) noexcept -> X11Display & = default;

  public:
    /// @brief Check to see if the the underlying display is connected to the X server.
    ///
    /// @detail Should be used after creating the connection to check for failure.
    ///
    /// @return true if connected, false otherwise.
    [[nodiscard]] auto is_connected() const noexcept -> bool;

    /// @brief Gets a reference counted copy to the underlying managed shared display pointer
    [[nodiscard]] auto get_shared() const noexcept -> std::shared_ptr<Display>;

    /// @brief Gets the raw Display pointer
    [[nodiscard]] auto get_raw() const noexcept -> Display *;

    /// @brief Refreshes all internal display values.
    auto refresh() -> void;

    /// @brief Gets the active screen id
    [[nodiscard]] auto screen_id() const noexcept -> std::int32_t;

    /// @brief Gets the screen width
    [[nodiscard]] auto screen_width() const noexcept -> std::int32_t;

    /// @brief Gets the screen height
    [[nodiscard]] auto screen_height() const noexcept -> std::int32_t;

    /// @brief Gets the number of screens
    [[nodiscard]] auto screen_count() const noexcept -> std::int32_t;

    /// @brief Gets default root window
    [[nodiscard]] auto default_root_window() const noexcept -> Window;

    /// @brief Gets root window based on the current screen id
    [[nodiscard]] auto root_window() const noexcept -> Window;

    /// @brief Provides details about the X server implementation that is running
    [[nodiscard]] auto server_vendor() const noexcept -> std::string;
};
} // namespace tilebox::core::x