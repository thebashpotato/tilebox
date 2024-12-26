#pragma once

#include "tilebox/draw/colorscheme.hpp"
#include "tilebox/draw/colorscheme_config.hpp"
#include "tilebox/draw/cursor.hpp"
#include "tilebox/draw/font.hpp"
#include "tilebox/error.hpp"
#include "tilebox/geometry.hpp"
#include "tilebox/utils/attributes.hpp"
#include "tilebox/x11/display.hpp"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <etl.hpp>

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace Tilebox
{

/// @brief The main class of the drawing library, it handles drawing text and colors to an X window as well as rendering
/// cursors.
///
/// @details X11Draw offers Font, Colorscheme, and Cursor management. Everything you need to build simple
/// yet powerful X client applications without the bloat of popular *Nix font/color rendering libraries.
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
    [[nodiscard]] static auto Create(const X11DisplaySharedResource &dpy, const Width &width,
                                     const Height &height) noexcept -> etl::Result<X11Draw, Error>;

    ///////////////////////////////////////
    /// Font Management
    //////////////////////////////////////

    /// @brief Initializes a font set based on an XftFont compatible string.
    ///
    /// @details If the type is already found to be initialized in the underlying array,
    /// it will not be reinitialized and success will be returned.
    ///
    /// @param font_name "monospace:size=14"
    /// @param type The font type enum to initialize
    ///
    /// @returns A result type containing Void if success, or an error if the underlying XftFont or FcPattern could not
    /// be allocated.
    [[nodiscard]] auto InitFont(const std::string &font_name,
                                X11Font::Type type) noexcept -> etl::Result<etl::Void, X11FontError>;

    /// @brief Gets a font by enum type
    ///
    /// @details Runtime complexity is constant time and stack allocated.
    ///
    /// @param type enum
    ///
    /// @returns optional containing the X11Font implementation for the specified type iff the type was
    /// initialized.
    [[nodiscard]] auto GetFont(X11Font::Type type) const noexcept -> std::optional<X11Font>;

    ///////////////////////////////////////
    /// Colorscheme Management
    ///////////////////////////////////////

    /// @brief Initializes a colorscheme based off a ColorSchemeConfig
    ///
    /// @details If the ColorSchemeKind is already initialized it will be treated as a success.
    /// For example to replace the ColorSchemeKind::Primary, it must first be removed.
    ///
    /// @param config colorscheme config to initialize
    ///
    /// @returns Void meaning success, X11ColorError otherwise.
    [[nodiscard]] auto InitColorScheme(const ColorSchemeConfig &config) noexcept
        -> etl::Result<etl::Void, X11ColorError>;

    /// @brief Removes a colorscheme
    ///
    /// @param kind Specified scheme kind
    ///
    /// @returns true if the scheme kind was removed successfully, false if not.
    [[nodiscard]] auto RemoveColorScheme(ColorSchemeKind kind) noexcept -> bool;

    /// @brief Gets a colorscheme by enum type
    ///
    /// @details Runtime complexity is constant time and stack allocated.
    ///
    /// @param kind enum
    ///
    /// @returns optional containing the X11ColorScheme implementation for the specified type iff the type was
    /// initialized.
    [[nodiscard]] auto GetColorScheme(ColorSchemeKind kind) const noexcept -> std::optional<X11ColorScheme>;

    ///////////////////////////////////////
    /// Cursor Management
    ///////////////////////////////////////

    /// @brief Initializes a cursor type
    ///
    /// @details If the specified type is already found to be initialized success will still be returned
    /// but a reinitialization will not happen.
    ///
    /// @param type enum
    ///
    /// @returns Void is an empty type denoting success, otherwise X11CursorError
    [[nodiscard]] auto InitCursor(X11Cursor::Type type) noexcept -> etl::Result<etl::Void, X11CursorError>;

    /// @brief Gets the underlying X Cursor id
    ///
    /// @details Runtime complexity is constant time and stack allocated.
    ///
    /// @param type enum
    ///
    /// @returns optional containing the X11Cursor implementation for the specified type iff the type was initialized.
    [[nodiscard]] auto GetCursor(X11Cursor::Type type) const noexcept -> std::optional<Cursor>;

    void Resize(const Width &width, const Height &height) noexcept;

    [[nodiscard]] auto width() const noexcept -> const Width &;

    [[nodiscard]] auto height() const noexcept -> const Height &;

  private:
    [[nodiscard]] auto GetTextExtents(const X11Font &font, const std::string_view &text,
                                      std::uint32_t len) const noexcept -> etl::Result<Vec2D, X11FontError>;

  private:
    X11Draw(X11DisplaySharedResource dpy, GC graphics_ctx, Drawable drawable, Width width, Height height) noexcept;

  private:
    X11DisplaySharedResource m_dpy;
    std::array<X11Font, X11Font::GetUnderlyingSize()> m_fonts;
    std::array<X11Cursor, X11Cursor::GetUnderlyingSize()> m_cursors;
    std::vector<X11ColorScheme> m_colorschemes;
    GC m_graphics_ctx;
    Drawable m_drawable;
    Width m_width;
    Height m_height;
};

} // namespace Tilebox

/// @brief Hi-jack the etl namespace to add a custom template specialization for Tilebox::X11Draw
template <typename ErrType> class etl::Result<Tilebox::X11Draw, ErrType>
{
  public:
    Result() noexcept = default;

    explicit Result(Tilebox::X11Draw &&value) noexcept : m_result(std::move(value)), m_is_ok(true)
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
    /// @return std::optional<Tilebox::X11Draw> for safety, incase the user did not call
    /// is_ok() before using this method.
    [[nodiscard]] auto ok() noexcept -> std::optional<Tilebox::X11Draw>
    {
        std::optional<Tilebox::X11Draw> ret;
        if (m_is_ok)
        {
            if (auto *value = std::get_if<Tilebox::X11Draw>(&m_result))
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
    std::variant<Tilebox::X11Draw, ErrType> m_result;
    bool m_is_ok{};
}; // namespace etl
