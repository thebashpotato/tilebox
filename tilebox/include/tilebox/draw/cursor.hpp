#pragma once

#include "tilebox/error.hpp"
#include "tilebox/utils/attributes.hpp"
#include "tilebox/x11/display.hpp"

#include <X11/X.h>
#include <X11/cursorfont.h>
#include <etl.hpp>

#include <cstdint>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

namespace Tilebox
{

/// @brief RAII memory managed move only interface to the Xlib Cursor type.
///
/// @details After constructing this object the X Server can generate the following error codes,
/// which must be checked by client code.
///
/// BadAlloc: The server failed to allocate the requested source or server memory
///
/// BadValue: Some numeric value falls outside the range of values accepted by the request. (This won't happen due to
/// constrained Type enum)
class TILEBOX_EXPORT X11Cursor
{
  public:
    /// @brief Supported cursor types
    enum class Type : std::uint8_t
    {
        Normal,
        Resize,
        Move, // Should remain the last value
    };

    using TypeIterator = etl::EnumerationIterator<Type, Type::Normal, Type::Move>;

  public:
    X11Cursor() = default;

    /// @brief Converts the underlying Type enum class to its integer equivalent
    [[nodiscard]] constexpr static auto ToUnderlying(const Type type) noexcept -> std::uint8_t
    {
        return std::underlying_type_t<Type>(type);
    }

    /// @brief Translates an X11Cursor::Type to a valid Xlib font cursor value
    [[nodiscard]] constexpr static auto ToCursorFont(const Type type) noexcept -> std::uint32_t
    {
        std::uint32_t cursor_font = 0;

        switch (type)
        {
        case Type::Normal: {
            cursor_font = XC_left_ptr;
            break;
        }
        case Type::Resize: {
            cursor_font = XC_sizing;
            break;
        }
        case Type::Move: {
            cursor_font = XC_fleur;
            break;
        }
        }

        return cursor_font;
    }

    /// @brief Creates a new X11Cursor.
    ///
    /// @details If the underlying XID is False after the XServer attempts its allocation, an X11CursorError
    /// will be returned. Although client side code will need to check their error callback that they registered with
    /// the X Server.
    [[nodiscard]] static auto Create(const X11DisplaySharedResource &dpy,
                                     Type type) noexcept -> etl::Result<X11Cursor, X11CursorError>;

    /// @brief Translates an X11Cursor::type to a valid Xlib font cursor value
    [[nodiscard]] constexpr static auto ToString(const Type type) noexcept -> std::string
    {
        std::string ret("X11Cursor::Type::");
        switch (type)
        {
        case Type::Normal: {
            ret.append("Normal");
            break;
        }
        case Type::Resize: {
            ret.append("Resize");
            break;
        }
        case Type::Move: {
            ret.append("Move");
            break;
        }
        }

        return ret;
    }

    /// @brief Gets the underlying Cursor XID
    [[nodiscard]] auto cursor() const noexcept -> Cursor;

    /// @brief Gets the underlying type this cursor was initialized with.
    [[nodiscard]] auto type() const noexcept -> std::optional<Type>;

  public:
    ~X11Cursor();
    X11Cursor(X11Cursor &&rhs) noexcept;
    X11Cursor(const X11Cursor &rhs) noexcept = delete;

  public:
    auto operator=(X11Cursor &&rhs) noexcept -> X11Cursor &;
    auto operator=(const X11Cursor &rhs) noexcept -> X11Cursor & = delete;

  private:
    X11Cursor(X11DisplaySharedResource dpy, Type type, Cursor cursor) noexcept;

  private:
    X11DisplaySharedResource m_dpy;
    std::optional<Type> m_type;
    Cursor m_cursor{};
};

} // namespace Tilebox

/// @brief Hi-jack the etl namespace to add a custom template specialization for Tilebox:X11Cursor:
template <typename ErrType> class etl::Result<Tilebox::X11Cursor, ErrType>
{
  public:
    Result() noexcept = default;

    explicit Result(Tilebox::X11Cursor &&value) noexcept : m_result(std::move(value)), m_is_ok(true)
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
    /// @details The use should always use isOk() before using ok()
    ///
    /// @return std::optional<Tilebox::X11Cursor> for safety, incase the user did not call
    /// isOk() before using this method.
    [[nodiscard]] auto ok() noexcept -> std::optional<Tilebox::X11Cursor>
    {
        std::optional<Tilebox::X11Cursor> ret;
        if (m_is_ok)
        {
            if (auto *value = std::get_if<Tilebox::X11Cursor>(&m_result))
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
    std::variant<Tilebox::X11Cursor, ErrType> m_result;
    bool m_is_ok{};
}; // namespace etl
