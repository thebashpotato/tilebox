#pragma once

#include "tilebox-core/geometry.hpp"
#include "tilebox-core/utils/attributes.hpp"
#include "tilebox-core/x11/display.hpp"
#include <X11/X.h>
#include <X11/Xft/Xft.h>
#include <fontconfig/fontconfig.h>
#include <ft2build.h>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace tilebox::core
{

using XftFontUniqueResource = std::unique_ptr<XftFont, std::function<void(XftFont *)>>;

class TILEBOX_INTERNAL X11Font
{
  private:
    XftFontUniqueResource _xftfont;
    // we can't have a smart pointer, Keith Packard forward declared
    // the FcPattern type, thus the call to sizeof() in the unique_ptr constructor will
    // fail at compile time due to the incomplete type.
    FcPattern *_pattern;
    Height _height;

  private:
    X11Font(XftFontUniqueResource &&xft_font, FcPattern *pattern) noexcept;

  public:
    virtual ~X11Font();
    X11Font(const X11Font &rhs) noexcept = delete;
    X11Font(X11Font &&rhs) noexcept = default;

  public:
    auto operator=(const X11Font &rhs) noexcept -> X11Font & = delete;
    auto operator=(X11Font &&rhs) noexcept -> X11Font & = default;

  public:
    /// @brief Creates a font based on the name of the font.
    [[nodiscard]] static auto create(const X11DisplaySharedResource &dpy, const std::string &font_name) noexcept
        -> std::optional<X11Font>;

    /// @brief Creates a font based on the pattern of the font.
    [[nodiscard]] static auto create(const X11DisplaySharedResource &dpy, FcPattern *font_pattern) noexcept
        -> std::optional<X11Font>;
};

class TILEBOX_INTERNAL X11FontManager
{
  private:
    X11DisplaySharedResource _dpy;
    std::vector<X11Font> _fonts;

  public:
    explicit X11FontManager(X11DisplaySharedResource dpy) noexcept;
};

} // namespace tilebox::core
