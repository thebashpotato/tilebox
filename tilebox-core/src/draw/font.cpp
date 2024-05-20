#include "tilebox-core/draw/font.hpp"
#include "tilebox-core/error.hpp"
#include "tilebox-core/vendor/etl.hpp"
#include "tilebox-core/x11/display.hpp"
#include <X11/Xft/Xft.h>
#include <fmt/core.h>
#include <fontconfig/fontconfig.h>
#include <functional>
#include <string>
#include <utility>

using namespace etl;

namespace tilebox::core
{

X11Font::X11Font(XftFontUniqueResource &&xft_font, FcPattern *pattern) noexcept
    : _xftfont(std::move(xft_font)), _pattern(pattern)
{
}

X11Font::~X11Font()
{
    if (_pattern != nullptr)
    {
        FcPatternDestroy(_pattern);
    }
}

auto X11Font::create(const X11DisplaySharedResource &dpy, const std::string &font_name) noexcept
    -> Result<X11Font, CoreError>
{
    if (font_name.empty())
    {
        return Result<X11Font, CoreError>(CoreError::create("Error, no font name was specified", RUNTIME_INFO));
    }

    // Using the pattern found at `_xftfont->pattern` does not yield the
    // same substitution results as using the pattern returned by
    // FcNameParse; using the latter results in the desired fallback
    // behaviour whereas the former just results in missing-character
    // rectangles being drawn, at least with some fonts.
    XftFont *raw_font = XftFontOpenName(dpy->raw(), dpy->screen_id(), font_name.c_str());
    if (raw_font == nullptr)
    {
        return Result<X11Font, CoreError>(
            CoreError::create(std::string("Error, could not load font from: ").append(font_name), RUNTIME_INFO));
    }

    FcPattern *pattern = FcNameParse(reinterpret_cast<const FcChar8 *>(font_name.c_str()));
    if (pattern == nullptr)
    {
        fmt::println("Error, could not parse font name from pattern: {}", font_name);

        return Result<X11Font, CoreError>(CoreError::create(
            std::string("Error, could not parse font name from pattern: ").append(font_name), RUNTIME_INFO));
    }

    auto xftfont_deleter = [dpy](XftFont *font) {
        if (font != nullptr)
        {
            XftFontClose(dpy->raw(), font);
        }
    };

    return Result<X11Font, CoreError>(X11Font(XftFontUniqueResource(raw_font, xftfont_deleter), pattern));
}

auto X11Font::create(const X11DisplaySharedResource &dpy, FcPattern *font_pattern) noexcept
    -> Result<X11Font, CoreError>
{
    if (font_pattern == nullptr)
    {
        return Result<X11Font, CoreError>(CoreError::create("Error, no font name was specified", RUNTIME_INFO));
    }

    XftFont *raw_font = XftFontOpenPattern(dpy->raw(), font_pattern);
    if (raw_font == nullptr)
    {
        return Result<X11Font, CoreError>(CoreError::create("Error, could not load font from pattern", RUNTIME_INFO));
    }

    auto xftfont_deleter = [dpy](XftFont *font) {
        if (font != nullptr)
        {
            XftFontClose(dpy->raw(), font);
        }
    };

    return Result<X11Font, CoreError>(X11Font(XftFontUniqueResource(raw_font, xftfont_deleter), font_pattern));
}

X11FontManager::X11FontManager(X11DisplaySharedResource dpy) noexcept : _dpy(std::move(dpy))
{
    _fonts.reserve(2);
}

} // namespace tilebox::core
