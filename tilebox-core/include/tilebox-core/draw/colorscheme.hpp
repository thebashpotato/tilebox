#pragma once

#include "tilebox-core/draw/color.hpp"
#include "tilebox-core/error.hpp"
#include "tilebox-core/utils/attributes.hpp"
#include "tilebox-core/vendor/etl.hpp"
#include "tilebox-core/x11/display.hpp"
#include <array>
#include <string>

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
    [[nodiscard]] static auto create(const X11DisplaySharedResource &dpy, const std::string &fg, const std::string &bg,
                                     const std::string &border) noexcept -> etl::Result<X11ColorScheme, X11ColorError>;

    [[nodiscard]] auto get_color(const X11ColorScheme::Index color_index) noexcept -> const X11Color &;

  private:
    using ColorSchemeArray = std::array<X11Color, Index::Border + 1>;

  private:
    explicit X11ColorScheme(ColorSchemeArray &&colors) noexcept;

  private:
    ColorSchemeArray _colors;
};

} // namespace tilebox::core
