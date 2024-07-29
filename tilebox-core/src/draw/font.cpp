#include "tilebox-core/draw/font.hpp"
#include "tilebox-core/error.hpp"
#include "tilebox-core/geometry.hpp"
#include "tilebox-core/vendor/etl.hpp"
#include "tilebox-core/x11/display.hpp"
#include <X11/Xft/Xft.h>
#include <cstdint>
#include <fontconfig/fontconfig.h>
#include <string>
#include <utility>

using namespace tilebox::core;
using namespace etl;

XftFontDeleter::XftFontDeleter(X11DisplaySharedResource display) noexcept : dpy(std::move(display))
{
}

auto XftFontDeleter::operator()(XftFont *font) const noexcept -> void
{
    if (font != nullptr && dpy->is_connected())
    {
        XftFontClose(dpy->raw(), font);
        font = nullptr;
    }
}

X11Font::X11Font(XftFontSharedResource &&xft_font, FcPattern *pattern, Height height) noexcept
    : _xftfont(std::move(xft_font)), _pattern(pattern), _height(std::move(height))
{
}

X11Font::~X11Font()
{
    if (_pattern != nullptr)
    {
        FcPatternDestroy(_pattern);
        _pattern = nullptr;
    }
}

X11Font::X11Font(X11Font &&rhs) noexcept
    : _xftfont(std::move(rhs._xftfont)), _pattern(rhs._pattern), _height(std::move(rhs._height))
{
    rhs._pattern = nullptr;
}

X11Font::X11Font(const X11Font &rhs) noexcept : _xftfont(rhs._xftfont), _height(rhs._height)
{
    if (rhs._pattern != nullptr)
    {
        _pattern = FcPatternDuplicate(rhs._pattern);
    }
    else
    {
        _pattern = nullptr;
    }
}

auto X11Font::operator=(X11Font &&rhs) noexcept -> X11Font &
{
    if (this != &rhs)
    {
        if (_pattern != nullptr)
        {
            FcPatternDestroy(_pattern);
        }
        _pattern = rhs._pattern;
        rhs._pattern = nullptr;

        _height = std::move(rhs._height);
        _xftfont = std::move(rhs._xftfont);
    }
    return *this;
}

auto X11Font::operator=(const X11Font &rhs) noexcept -> X11Font &
{
    if (this != &rhs)
    {
        if (_pattern != nullptr)
        {
            FcPatternDestroy(_pattern);
        }
        if (rhs._pattern != nullptr)
        {
            _pattern = FcPatternDuplicate(rhs._pattern);
        }
        else
        {
            _pattern = nullptr;
        }

        _height = rhs._height;
        _xftfont = rhs._xftfont;
    }
    return *this;
}

auto X11Font::create(const X11DisplaySharedResource &dpy,
                     const std::string &font_name) noexcept -> Result<X11Font, X11FontError>
{
    if (font_name.empty())
    {
        return Result<X11Font, X11FontError>(X11FontError("Error, empty font name", RUNTIME_INFO));
    }

    // Using the pattern found at `_xftfont->pattern` does not yield the
    // same substitution results as using the pattern returned by
    // FcNameParse; using the latter results in the desired fallback
    // behaviour whereas the former just results in missing-character
    // rectangles being drawn, at least with some fonts.
    XftFont *raw_font = XftFontOpenName(dpy->raw(), dpy->screen_id(), font_name.c_str());
    if (raw_font == nullptr)
    {
        return Result<X11Font, X11FontError>(
            X11FontError(std::string("Error, could not load font from: ").append(font_name), RUNTIME_INFO));
    }

    FcPattern *pattern = FcNameParse(reinterpret_cast<const FcChar8 *>(font_name.c_str()));
    if (pattern == nullptr)
    {
        XftFontClose(dpy->raw(), raw_font);
        return Result<X11Font, X11FontError>(X11FontError(
            std::string("Error, could not parse font name from pattern: ").append(font_name), RUNTIME_INFO));
    }

    Height height(static_cast<uint32_t>(raw_font->ascent + raw_font->descent));

    return Result<X11Font, X11FontError>(
        X11Font(XftFontSharedResource(raw_font, XftFontDeleter(dpy)), pattern, std::move(height)));
}

auto X11Font::create(const X11DisplaySharedResource &dpy,
                     FcPattern *font_pattern) noexcept -> Result<X11Font, X11FontError>
{
    if (font_pattern == nullptr)
    {
        return Result<X11Font, X11FontError>({"Error, no font name was specified", RUNTIME_INFO});
    }

    XftFont *raw_font = XftFontOpenPattern(dpy->raw(), font_pattern);
    if (raw_font == nullptr)
    {
        return Result<X11Font, X11FontError>({"Error, could not load font from pattern", RUNTIME_INFO});
    }

    Height height(static_cast<uint32_t>(raw_font->ascent + raw_font->descent));

    return Result<X11Font, X11FontError>(
        X11Font(XftFontSharedResource(raw_font, XftFontDeleter(dpy)), font_pattern, std::move(height)));
}

auto X11Font::xftfont() const noexcept -> const XftFontSharedResource &
{
    return _xftfont;
}

auto X11Font::height() const noexcept -> Height
{
    return _height;
}

auto X11Font::pattern() const noexcept -> FcPattern *
{
    return _pattern;
}
