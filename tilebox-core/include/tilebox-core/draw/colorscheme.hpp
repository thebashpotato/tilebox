#pragma once

#include "tilebox-core/draw/color.hpp"
#include "tilebox-core/draw/colorscheme_config.hpp"
#include "tilebox-core/error.hpp"
#include "tilebox-core/utils/attributes.hpp"
#include "tilebox-core/vendor/etl.hpp"
#include "tilebox-core/x11/display.hpp"
#include <array>

namespace tilebox::core
{

/// @brief Represents a color scheme to draw to an X11 surface.
class TILEBOX_EXPORT X11ColorScheme
{

  public:
    // Each color scheme will support 3 different colors
    enum Index
    {
        Foreground,
        Background,
        Border, // Must remain the last element in the enum
    };

  public:
    /// @brief Allocates all supported colors for this scheme. All strings must be valid hex codes.
    ///
    /// @details The X11ColorError can contain an error from the XftColor allocation function or std::out_of_range for
    /// invalid array access. However the latter will not happen, it's just for safety.
    [[nodiscard]] static auto create(const X11DisplaySharedResource &dpy, const ColorSchemeConfig &config) noexcept
        -> etl::Result<X11ColorScheme, X11ColorError>;

    /// @brief Gets the kind of color scheme this is
    [[nodiscard]] auto kind() const noexcept -> ColorSchemeKind;

    /// @brief Gets the specified color scheme index
    ///
    /// @details The underlying array is indexed into directly, which is technically undefined behaviour if the index
    /// was to be out of range. But since an enum is being used as the index this is not possible. Any other type being
    /// passed through will cause a compile time error.
    [[nodiscard]] auto get_color(const X11ColorScheme::Index color_index) const noexcept -> const X11Color &;

  private:
    using ColorSchemeArray = std::array<X11Color, Index::Border + 1>;

  private:
    explicit X11ColorScheme(ColorSchemeArray &&colors, const ColorSchemeKind kind) noexcept;

  private:
    ColorSchemeArray _colors;
    ColorSchemeKind _kind{};
};

} // namespace tilebox::core
