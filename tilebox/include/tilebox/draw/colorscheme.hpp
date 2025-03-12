#pragma once

#include "tilebox/draw/color.hpp"
#include "tilebox/draw/colorscheme_config.hpp"
#include "tilebox/error.hpp"
#include "tilebox/utils/attributes.hpp"
#include "tilebox/x11/display.hpp"

#include <etl.hpp>

#include <array>
#include <cstdint>
#include <type_traits>

namespace Tilebox
{

/// @brief Represents a color scheme to draw to an X11 surface.
class TILEBOX_EXPORT X11ColorScheme
{
  public:
    // Each color scheme will support 3 different colors
    enum class Type : std::uint8_t
    {
        Foreground,
        Background,
        Border, // Must remain the last element in the enum
    };

    using TypeIterator = etl::EnumerationIterator<Type, Type::Foreground, Type::Border>;

    /// @brief Converts the underlying Type enum class to its integer equivalent
    [[nodiscard]] constexpr static auto ToUnderlying(const Type type) noexcept -> std::uint8_t
    {
        return std::underlying_type_t<Type>(type);
    }

  public:
    /// @brief Allocates all supported colors for this scheme. All strings must be valid hex codes.
    ///
    /// @details The X11ColorError can contain an error from the XftColor allocation function or std::out_of_range for
    /// invalid array access. However, the latter will not happen, it's just for safety.
    [[nodiscard]] static auto Create(const X11DisplaySharedResource &dpy, const ColorSchemeConfig &config) noexcept
        -> etl::Result<X11ColorScheme, X11ColorError>;

    /// @brief Gets the kind of color scheme this is
    [[nodiscard]] auto Kind() const noexcept -> ColorSchemeKind;

    /// @brief Gets the specified color scheme index
    ///
    /// @details The underlying array is indexed into directly, which is technically undefined behaviour if the index
    /// was to be out of range. But since an enum is being used as the index this is not possible. Any other type being
    /// passed through will cause a compile time error.
    [[nodiscard]] auto GetColor(Type color_index) const noexcept -> const X11Color &;

  private:
    using ColorSchemeArray = std::array<X11Color, TypeIterator::size()>;

  private:
    explicit X11ColorScheme(ColorSchemeArray &&colors, ColorSchemeKind kind) noexcept;

  private:
    ColorSchemeArray m_colors;
    ColorSchemeKind m_kind{};
};

} // namespace Tilebox
