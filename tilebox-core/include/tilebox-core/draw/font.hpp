#pragma once

#include "tilebox-core/error.hpp"
#include "tilebox-core/geometry.hpp"
#include "tilebox-core/utils/attributes.hpp"
#include "tilebox-core/vendor/etl.hpp"
#include "tilebox-core/x11/display.hpp"
#include <X11/X.h>
#include <X11/Xft/Xft.h>
#include <fontconfig/fontconfig.h>
#include <ft2build.h>
#include <memory>
#include <string>

namespace tilebox::core
{

struct TILEBOX_INTERNAL XftFontDeleter
{
    X11DisplaySharedResource dpy;

    explicit XftFontDeleter(X11DisplaySharedResource display) noexcept;

    auto operator()(XftFont *font) const noexcept -> void;
};

using XftFontSharedResource = std::shared_ptr<XftFont>;

/// @brief Provides a RAII wrapper around an XftFont and fontconfig.
class TILEBOX_EXPORT X11Font
{
  public:
    ~X11Font();
    X11Font(X11Font &&rhs) noexcept;
    X11Font(const X11Font &rhs) noexcept;

  public:
    auto operator=(X11Font &&rhs) noexcept -> X11Font &;
    auto operator=(const X11Font &rhs) noexcept -> X11Font &;

  public:
    /// @brief Creates a font based on the name of the font.
    [[nodiscard]] static auto create(const X11DisplaySharedResource &dpy, const std::string &font_name) noexcept
        -> etl::Result<X11Font, X11FontError>;

    /// @brief Creates a font based on the pattern of the font.
    [[nodiscard]] static auto create(const X11DisplaySharedResource &dpy, FcPattern *font_pattern) noexcept
        -> etl::Result<X11Font, X11FontError>;

    [[nodiscard]] auto xftfont() const noexcept -> const XftFontSharedResource &;
    [[nodiscard]] auto pattern() const noexcept -> FcPattern *;
    [[nodiscard]] auto height() const noexcept -> Height;

  private:
    X11Font(XftFontSharedResource &&xft_font, FcPattern *pattern, Height height) noexcept;

  private:
    XftFontSharedResource _xftfont;
    // we can't have a smart pointer, Keith Packard forward declared
    // the FcPattern type, thus the call to sizeof() in the unique_ptr constructor will
    // fail at compile time due to the incomplete type.
    FcPattern *_pattern;
    Height _height;
};

} // namespace tilebox::core
