#pragma once

#include <X11/X.h>
#include <X11/Xlib.h>
#include <cstdint>
#include <memory>
#include <string>
#include <tilebox-core/utils/attributes.hpp>

namespace tilebox::core
{

/// @brief Provides a RAII interface for common functionaility needed from the X11 Display
class TILEBOX_EXPORT X11Display : public std::enable_shared_from_this<X11Display>
{
  private:
    std::shared_ptr<Display> _display;

  private:
    explicit X11Display(const std::string &display_name = "") noexcept;

  public:
    virtual ~X11Display() = default;

    X11Display(X11Display &&other) noexcept = default;
    auto operator=(X11Display &&other) noexcept -> X11Display & = default;
    X11Display(const X11Display &other) noexcept = default;
    auto operator=(const X11Display &other) noexcept -> X11Display & = default;

  public:
    /// @brief Returns a managed shared pointer to a X11 display.
    ///
    /// @param `display_name` Specifies the hardware display name, which determines the display and communications
    /// domain to be used. On a POSIX-conformant system, if the display_name is NULL, it defaults to the value of the
    /// DISPLAY environment variable.
    [[nodiscard]] static auto create(const std::string &display_name = "") -> std::shared_ptr<X11Display>;

    /// @brief Check to see if the the underlying display is connected to the X server.
    ///
    /// @detail Should be used after creating the connection to check for failure.
    ///
    /// @return true if connected, false otherwise.
    [[nodiscard]] auto is_connected() const noexcept -> bool;

    /// @brief Gets a shared pointer to this X11 display wrapper.
    [[nodiscard]] auto get_shared() -> std::shared_ptr<X11Display>;

    /// @brief Gets the raw Display pointer
    [[nodiscard]] auto get_raw() const noexcept -> Display *;

    /// @brief Gets the active screen id
    [[nodiscard]] auto screen_id() const noexcept -> std::int32_t;

    /// @brief Gets the number of screens
    [[nodiscard]] auto screen_count() const noexcept -> std::int32_t;

    /// @brief Gets default root window
    [[nodiscard]] auto default_root_window() const noexcept -> Window;

    /// @brief Gets root window based on the current screen id
    [[nodiscard]] auto root_window() const noexcept -> Window;

    /// @brief Provides details about the X server implementation that is running
    [[nodiscard]] auto server_vendor() const noexcept -> std::string;
};
} // namespace tilebox::core
