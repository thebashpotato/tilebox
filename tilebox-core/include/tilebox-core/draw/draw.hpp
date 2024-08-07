#pragma once

#include "tilebox-core/draw/colorscheme.hpp"
#include "tilebox-core/draw/colorscheme_config.hpp"
#include "tilebox-core/draw/cursor.hpp"
#include "tilebox-core/draw/font.hpp"
#include "tilebox-core/error.hpp"
#include "tilebox-core/geometry.hpp"
#include "tilebox-core/utils/attributes.hpp"
#include "tilebox-core/vendor/etl.hpp"
#include "tilebox-core/x11/display.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <array>
#include <string>
#include <string_view>
#include <vector>

namespace tilebox::core
{

/// @brief The main class of the drawing library, it handles drawing text and colors to an X window as well as rendering
/// cursors.
///
/// @details X11Draw offers Font, Colorscheme, and Cursor management. Everything you need to build simple
/// yet powerful X client applications without the bloat of populuar *Nix font/color rendering libraries.
class TILEBOX_EXPORT X11Draw
{
  public:
    ~X11Draw() noexcept;
    X11Draw(X11Draw &&rhs) noexcept;
    X11Draw(const X11Draw &rhs) noexcept = delete;

  public:
    auto operator=(X11Draw &&rhs) noexcept -> X11Draw &;
    auto operator=(const X11Draw &rhs) noexcept -> X11Draw & = delete;

  public:
    [[nodiscard]] static auto create(const X11DisplaySharedResource &dpy, const Width &width,
                                     const Height &height) noexcept -> etl::Result<X11Draw, CoreError>;

    ///////////////////////////////////////
    /// Font Management
    //////////////////////////////////////

    /// @brief Initializes a font set based on an XftFont compatible string.
    ///
    /// @details If the type is already found to be initialized in the underlying array,
    /// it will not be reinitialized and success will be returned.
    ///
    /// @param `font_name` "monospace:size=14"
    /// @param `type` The font type enum to initialize
    ///
    /// @returns A result type containing Void if success, or an error if the underlying XftFont or FcPattern could not
    /// be allocated.
    [[nodiscard]] auto font_init(const std::string &font_name,
                                 const X11Font::Type type) noexcept -> etl::Result<etl::Void, X11FontError>;

    /// @brief Gets a font by enum type
    ///
    /// @details Runtime complexity is constant time and stack allocated.
    ///
    /// @param `type` enum
    ///
    /// @returns optional containing the X11Font implementation for the specified type iff the type was
    /// initialized.
    [[nodiscard]] auto font_get(const X11Font::Type type) const noexcept -> std::optional<X11Font>;

    ///////////////////////////////////////
    /// Colorscheme Management
    ///////////////////////////////////////

    /// @brief Initializes a colorscheme based off a ColorSchemeConfig
    ///
    /// @details If the ColorSchemeKind is already initialized it will be treated as a success.
    /// For example to replace the ColorSchemeKind::Primary, it must first be removed.
    ///
    /// @param `config` colorscheme config to initialize
    ///
    /// @returns Void meaning success, X11ColorError other wise.
    [[nodiscard]] auto colorscheme_init(const ColorSchemeConfig &config) noexcept
        -> etl::Result<etl::Void, X11ColorError>;

    /// @brief Removes a colorscheme
    ///
    /// @param `kind` Specified scheme kind
    ///
    /// @returns true if the scheme kind was removed successfully, false if not.
    [[nodiscard]] auto colorscheme_remove(const ColorSchemeKind kind) noexcept -> bool;

    /// @brief Gets a colorscheme by enum type
    ///
    /// @details Runtime complexity is constant time and stack allocated.
    ///
    /// @param `kind` enum
    ///
    /// @returns optional containing the X11ColorScheme implementation for the specified type iff the type was
    /// initialized.
    [[nodiscard]] auto colorscheme_get(const ColorSchemeKind kind) const noexcept -> std::optional<X11ColorScheme>;

    ///////////////////////////////////////
    /// Cursor Management
    ///////////////////////////////////////

    /// @brief Initializes a cursor type
    ///
    /// @details If the specified type is already found to be initialized success will still be returned
    /// but a reinitialization will not happen.
    ///
    /// @param `type` enum
    ///
    /// @returns Void is an empty type denoting success, otherwise X11CursorError
    [[nodiscard]] auto cursor_init(const X11Cursor::Type type) noexcept -> etl::Result<etl::Void, X11CursorError>;

    /// @brief Gets the underlying X Cursor id
    ///
    /// @details Runtime complexity is constant time and stack allocated.
    ///
    /// @param `type` enum
    ///
    /// @returns optional containing the X11Cursor implementation for the specified type iff the type was initialized.
    [[nodiscard]] auto cursor_get(const X11Cursor::Type type) const noexcept -> std::optional<Cursor>;

  public:
    auto resize(const Width &width, const Height &height) noexcept -> void;

    [[nodiscard]] auto width() const noexcept -> const Width &;

    [[nodiscard]] auto height() const noexcept -> const Height &;

  private:
    [[nodiscard]] auto _get_text_extents(const X11Font &font, const std::string_view &text,
                                         const uint32_t len) noexcept -> etl::Result<Vec2D, X11FontError>;

  private:
    X11Draw(X11DisplaySharedResource dpy, GC graphics_ctx, Drawable drawable, Width width, Height height) noexcept;

  private:
    X11DisplaySharedResource _dpy;
    std::array<X11Font, X11Font::get_underlying_size()> _fonts;
    std::array<X11Cursor, X11Cursor::get_underlying_size()> _cursors;
    std::vector<X11ColorScheme> _colorschemes;
    GC _graphics_ctx;
    Drawable _drawable;
    Width _width;
    Height _height;
};

} // namespace tilebox::core

/// @brief Hi-jack the etl namespace to add a custom template specialization for tilebox::core::X11Draw
namespace etl
{

template <typename ErrType> class Result<tilebox::core::X11Draw, ErrType>
{
  public:
    Result() noexcept = default;

    explicit Result(tilebox::core::X11Draw &&value) noexcept : _result(std::move(value)), _is_ok(true)
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
    /// @details The use should always use is_ok() before using ok()
    ///
    /// @return std::optional<tilebox::core::X11Draw> for safety, incase the user did not call
    /// is_ok() before using this method.
    [[nodiscard]] inline auto ok() noexcept -> std::optional<tilebox::core::X11Draw>
    {
        std::optional<tilebox::core::X11Draw> ret;
        if (_is_ok)
        {
            if (auto *value = std::get_if<tilebox::core::X11Draw>(&_result))
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
    std::variant<tilebox::core::X11Draw, ErrType> _result;
    bool _is_ok{};
};

} // namespace etl
