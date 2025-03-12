#pragma once

#include "atom_manager.hpp"

#include <etl.hpp>
#include <tilebox/draw/draw.hpp>
#include <tilebox/error.hpp>
#include <tilebox/x11/display.hpp>
#include <tilebox/x11/event_loop.hpp>

#include <string_view>

namespace Tbwm
{

class WindowManager
{
  public:
    ~WindowManager() = default;
    WindowManager(WindowManager &&rhs) = default;
    WindowManager(const WindowManager &rhs) = delete;

  public:
    auto operator=(const WindowManager &rhs) -> WindowManager & = delete;
    auto operator=(WindowManager &&rhs) -> WindowManager & = default;

  public:
    [[nodiscard]] static auto Create(std::string_view wm_name) noexcept -> etl::Result<WindowManager, Tilebox::Error>;
    [[nodiscard]] auto Start() noexcept -> etl::Result<etl::Void, etl::DynError>;

  private:
    /// @brief Check to see if another window manager is already running
    ///
    /// @returns true if another wm is running, false otherwise
    [[nodiscard]] auto IsOtherWmRunning() const noexcept -> bool;

    /// @brief Clean up all zombie processes inherited from xinitrc, allows the kernel to reap all child processes
    static void ProcessCleanup() noexcept;

    /// @brief Announces "I'm an EWMH compatible window manager" to the X11 ecosystem
    ///
    /// @details Responsible for advertising tbwm’s presence and capabilities in accordance
    /// with the Extended Window Manager Hints (EWMH) specification. In other words,
    /// tbwm is declaring itself as the window manager and telling other clients
    /// which EWMH features it supports.
    ///
    /// @link https://specifications.freedesktop.org/wm-spec/latest/
    void AdvertiseAsEWMHCapable() noexcept;

    /// @brief Initialize supported WM Atoms and EWMH Atoms, Fonts, Cursors and Color Schemes
    [[nodiscard]] auto Initialize() noexcept -> etl::Result<etl::Void, etl::DynError>;

  private:
    explicit WindowManager(Tilebox::X11DisplaySharedResource &&dpy, Tilebox::X11Draw &&draw,
                           std::string_view name) noexcept;

  private:
    Tilebox::X11DisplaySharedResource m_dpy;
    Tilebox::X11Draw m_draw;
    Tilebox::X11EventLoop m_event_loop;
    AtomManager m_atom_manager;
    Window m_ewmh_check_win{};
    bool m_running{};
    std::string_view m_name;
};

} // namespace Tbwm

/// @brief Hi-jack the etl namespace to add a custom template specialization for Tilebox::Tbwm::WindowManager
template <typename ErrType> class etl::Result<Tbwm::WindowManager, ErrType>
{
  public:
    Result() noexcept = default;

    explicit Result(Tbwm::WindowManager &&value) noexcept : m_result(std::move(value)), m_is_ok(true)
    {
    }

    explicit Result(const ErrType &error) noexcept : m_result(error)
    {
    }

    explicit Result(ErrType &&error) noexcept : m_result(std::move(error))
    {
    }

  public:
    /// @brief Check if the union value is of the ok type
    [[nodiscard]] auto is_ok() const noexcept -> bool
    {
        return m_is_ok;
    }

    /// @brief Check if the union value is of the error type
    [[nodiscard]] auto is_err() const noexcept -> bool
    {
        return !m_is_ok;
    }

    /// @brief Check if the union value is of the error type
    ///
    /// @details The use should always use is_ok() before using ok()
    ///
    /// @return std::optional<Tilebox::Tbwm::WindowManager> for safety, incase the user did not call
    /// is_ok() before using this method.
    [[nodiscard]] auto ok() noexcept -> std::optional<Tbwm::WindowManager>
    {
        std::optional<Tbwm::WindowManager> ret;
        if (m_is_ok)
        {
            if (auto *value = std::get_if<Tbwm::WindowManager>(&m_result))
            {
                ret.emplace(std::move(*value));
            }
        }
        return ret;
    }

    /// @brief Check if the union value is of the error type
    ///
    /// @details The use should always use is_err() before using err()
    ///
    /// @return std::optional<ErrType> for safety, incase the user did not call
    /// is_err() before using this method.
    [[nodiscard]] auto err() const noexcept -> std::optional<ErrType>
    {
        std::optional<ErrType> ret;
        if (!m_is_ok)
        {
            if (auto *err = std::get_if<ErrType>(&m_result))
            {
                ret.emplace(*err);
            }
        }
        return ret;
    }

  private:
    std::variant<Tbwm::WindowManager, ErrType> m_result;
    bool m_is_ok{};
}; // namespace etl
