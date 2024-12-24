#pragma once

#include <etl.hpp>
#include <tilebox/draw/draw.hpp>
#include <tilebox/error.hpp>
#include <tilebox/x11/display.hpp>
#include <tilebox/x11/event_loop.hpp>

namespace Tilebox::Twm
{

class WindowManager
{
  public:
    ~WindowManager() = default;
    WindowManager(const WindowManager &rhs) = delete;
    WindowManager(WindowManager &&rhs) = default;

  public:
    auto operator=(const WindowManager &rhs) -> WindowManager & = delete;
    auto operator=(WindowManager &&rhs) -> WindowManager & = default;

  public:
    [[nodiscard]] static auto Create() noexcept -> etl::Result<WindowManager, Error>;
    [[nodiscard]] auto Start() const noexcept -> etl::Result<etl::Void, etl::DynError>;

  private:
    /// @brief Check to see if another window manager is already running
    ///
    /// @returns true if another wm is running, false otherwise
    [[nodiscard]] auto IsOtherWmRunning() const noexcept -> bool;

    /// @brief Clean up all zombie processes inherited from xinitrc, allows the kernel to reap all child processes
    static void ProcessCleanup() noexcept;

    /// @brief Initialize supported WM Atoms and EWMH Atoms, Fonts, Cursors and Color Schemes
    void Initialize() noexcept;

  private:
    explicit WindowManager(X11DisplaySharedResource &&dpy, X11Draw &&draw) noexcept;

  private:
    X11DisplaySharedResource m_dpy;
    X11Draw m_draw;
    X11EventLoop m_event_loop;
    bool m_run{true};
};

} // namespace Tilebox::Twm

/// @brief Hi-jack the etl namespace to add a custom template specialization for Tilebox::Twm::WindowManager
template <typename ErrType> class etl::Result<Tilebox::Twm::WindowManager, ErrType>
{
  public:
    Result() noexcept = default;

    explicit Result(Tilebox::Twm::WindowManager &&value) noexcept : m_result(std::move(value)), m_is_ok(true)
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
    /// @return std::optional<Tilebox::Twm::WindowManager> for safety, incase the user did not call
    /// is_ok() before using this method.
    [[nodiscard]] auto ok() noexcept -> std::optional<Tilebox::Twm::WindowManager>
    {
        std::optional<Tilebox::Twm::WindowManager> ret;
        if (m_is_ok)
        {
            if (auto *value = std::get_if<Tilebox::Twm::WindowManager>(&m_result))
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
    std::variant<Tilebox::Twm::WindowManager, ErrType> m_result;
    bool m_is_ok{};
}; // namespace etl
