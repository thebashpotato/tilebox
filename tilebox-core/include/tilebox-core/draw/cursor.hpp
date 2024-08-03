#pragma once

#include "tilebox-core/error.hpp"
#include "tilebox-core/utils/attributes.hpp"
#include "tilebox-core/vendor/etl.hpp"
#include "tilebox-core/x11/display.hpp"
#include <X11/X.h>
#include <X11/cursorfont.h>
#include <cstddef>
#include <cstdint>
#include <string>

namespace tilebox::core
{

/// @brief RAII memory managed move only interface to the Xlib Cursor type.
///
/// @details After constructing this object the X Server can generate the following error codes,
/// which must be checked by client code.
///
/// BadAlloc: The server failed to allocate the requested source or server memory
///
/// BadValue: Some numeric value falls outside the range of values accepted by the request. (This won't happen due to
/// constrained State enum)
class TILEBOX_EXPORT X11Cursor
{
  public:
    /// @brief Supported cursor states
    enum class State
    {
        Normal,
        Resize,
        Move // Should remain the last item in this enum at all times.
    };

  public:
    /// @brief Returns the size of the X11Cursor::State enum for the purposes of statically
    /// defining stack allocated containers.
    [[nodiscard]] constexpr static inline auto state_size() -> std::size_t
    {
        return static_cast<std::size_t>(static_cast<std::size_t>(X11Cursor::State::Move) + 1);
    }

    /// @brief Creates a new X11Cursor.
    ///
    /// @details If the underlying XID is False after the XServer attempts its allocation, an X11CursorError
    /// will be returned. Although client side code will need to check their error callback that they registered with
    /// the X Server.
    [[nodiscard]] static auto create(const X11DisplaySharedResource &dpy,
                                     const X11Cursor::State state) noexcept -> etl::Result<X11Cursor, X11CursorError>;

    /// @brief Translates an X11Cursor::State to a valid Xlib fontcursor value
    [[nodiscard]] constexpr static inline auto to_cursor_font(const X11Cursor::State state) noexcept -> std::uint32_t
    {
        switch (state)
        {
        case X11Cursor::State::Normal: {
            return XC_left_ptr;
        }
        case X11Cursor::State::Resize: {
            return XC_sizing;
        }
        case X11Cursor::State::Move: {
            return XC_fleur;
        }
        }
    }

    /// @brief Translates an X11Cursor::State to a valid Xlib fontcursor value
    [[nodiscard]] constexpr static inline auto to_string(const X11Cursor::State state) noexcept -> std::string
    {
        std::string ret("X11Cursor::State::");
        switch (state)
        {
        case X11Cursor::State::Normal: {
            ret.append("Normal");
        }
        case X11Cursor::State::Resize: {
            ret.append("Resize");
        }
        case X11Cursor::State::Move: {
            ret.append("Move");
        }
        }

        return ret;
    }

    /// @brief Gets the underlying Cursor XID
    [[nodiscard]] auto cursor() const noexcept -> Cursor;

  public:
    ~X11Cursor();
    X11Cursor(X11Cursor &&rhs) noexcept;
    X11Cursor(const X11Cursor &rhs) noexcept = delete;

  public:
    auto operator=(X11Cursor &&rhs) noexcept -> X11Cursor &;
    auto operator=(const X11Cursor &rhs) noexcept -> X11Cursor & = delete;

  private:
    X11Cursor(X11DisplaySharedResource dpy, const Cursor cursor) noexcept;

  private:
    X11DisplaySharedResource _dpy;
    Cursor _cursor;
};

} // namespace tilebox::core

/// @brief Hi-jack the etl namespace to add a custom template specialization for tilebox::core:X11Cursor:
namespace etl
{

template <typename ErrType> class Result<tilebox::core::X11Cursor, ErrType>
{
  public:
    Result() noexcept = default;

    explicit Result(tilebox::core::X11Cursor &&value) noexcept : _result(std::move(value)), _is_ok(true)
    {
    }

    explicit Result(const ErrType &error) noexcept : _result(error)
    {
    }

    explicit Result(ErrType &&error) noexcept : _result(std::move(error))
    {
    }

  public:
    /// @brief Check if the union value is of the ok type
    [[nodiscard]] inline auto is_ok() const noexcept -> bool
    {
        return _is_ok;
    }

    /// @brief Check if the union value is of the error type
    [[nodiscard]] inline auto is_err() const noexcept -> bool
    {
        return !_is_ok;
    }

    /// @brief Check if the union value is of the error type
    ///
    /// @details The use should always use isOk() before using ok()
    ///
    /// @return std::optional<tilebox::core::X11Cursor> for safety, incase the user did not call
    /// isOk() before using this method.
    [[nodiscard]] inline auto ok() noexcept -> std::optional<tilebox::core::X11Cursor>
    {
        std::optional<tilebox::core::X11Cursor> ret;
        if (_is_ok)
        {
            if (auto *value = std::get_if<tilebox::core::X11Cursor>(&_result))
            {
                ret.emplace(std::move(*value));
            }
        }
        return ret;
    }

    /// @brief Check if the union value is of the error type
    ///
    /// @details The use should always use isErr() before using err()
    ///
    /// @return std::optional<ErrType> for safety, incase the user did not call
    /// isErr() before using this method.
    [[nodiscard]] inline auto err() const noexcept -> std::optional<ErrType>
    {
        std::optional<ErrType> ret;
        if (!_is_ok)
        {
            if (auto *err = std::get_if<ErrType>(&_result))
            {
                ret.emplace(*err);
            }
        }
        return ret;
    }

  private:
    std::variant<tilebox::core::X11Cursor, ErrType> _result;
    bool _is_ok{};
};

} // namespace etl
