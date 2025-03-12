#include "tilebox/draw/font.hpp"
#include "tilebox/error.hpp"
#include "tilebox/geometry.hpp"
#include "tilebox/x11/display.hpp"

#include <X11/Xft/Xft.h>
#include <etl.hpp>
#include <fontconfig/fontconfig.h>

#include <cstdint>
#include <optional>
#include <string>
#include <utility>

using namespace etl;

namespace Tilebox
{

XftFontDeleter::XftFontDeleter(X11DisplaySharedResource display) noexcept : dpy(std::move(display))
{
}

auto XftFontDeleter::operator()(XftFont *font) const noexcept -> void
{
    if (font != nullptr && dpy->IsConnected())
    {
        XftFontClose(dpy->Raw(), font);
    }
}

X11Font::X11Font(XftFontSharedResource &&xft_font, FcPattern *pattern, const Type type, Height height) noexcept
    : m_xftfont(std::move(xft_font)), m_pattern(pattern), m_type(type), m_height(std::move(height))
{
}

X11Font::~X11Font()
{
    if (m_pattern != nullptr)
    {
        FcPatternDestroy(m_pattern);
        m_pattern = nullptr;
    }
}

X11Font::X11Font(X11Font &&rhs) noexcept
    : m_xftfont(std::move(rhs.m_xftfont)), m_pattern(rhs.m_pattern), m_type(rhs.m_type),
      m_height(std::move(rhs.m_height))
{
    rhs.m_pattern = nullptr;
}

X11Font::X11Font(const X11Font &rhs) noexcept : m_xftfont(rhs.m_xftfont), m_type(rhs.m_type), m_height(rhs.m_height)
{
    if (rhs.m_pattern != nullptr)
    {
        m_pattern = FcPatternDuplicate(rhs.m_pattern);
    }
    else
    {
        m_pattern = nullptr;
    }
}

auto X11Font::operator=(X11Font &&rhs) noexcept -> X11Font &
{
    if (this != &rhs)
    {
        m_height = std::move(rhs.m_height);
        m_type = rhs.m_type;

        if (m_pattern != nullptr)
        {
            FcPatternDestroy(m_pattern);
        }
        m_pattern = rhs.m_pattern;
        rhs.m_pattern = nullptr;

        m_xftfont = std::move(rhs.m_xftfont);
    }
    return *this;
}

auto X11Font::operator=(const X11Font &rhs) noexcept -> X11Font &
{
    if (this != &rhs)
    {
        m_height = rhs.m_height;
        m_type = rhs.m_type;

        if (m_pattern != nullptr)
        {
            FcPatternDestroy(m_pattern);
        }

        if (rhs.m_pattern != nullptr)
        {
            m_pattern = FcPatternDuplicate(rhs.m_pattern);
        }
        else
        {
            m_pattern = nullptr;
        }

        m_xftfont = rhs.m_xftfont;
    }
    return *this;
}

auto X11Font::Create(const X11DisplaySharedResource &dpy, const std::string &font_name, const Type type) noexcept
    -> Result<X11Font, X11FontError>
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
    XftFont *raw_font = XftFontOpenName(dpy->Raw(), dpy->ScreenId(), font_name.c_str());
    if (raw_font == nullptr)
    {
        return Result<X11Font, X11FontError>(
            X11FontError(std::string("Error, could not load font from: ").append(font_name), RUNTIME_INFO));
    }

    FcPattern *pattern = FcNameParse(reinterpret_cast<const FcChar8 *>(font_name.c_str()));
    if (pattern == nullptr)
    {
        XftFontClose(dpy->Raw(), raw_font);
        return Result<X11Font, X11FontError>(X11FontError(
            std::string("Error, could not parse font name from pattern: ").append(font_name), RUNTIME_INFO));
    }

    Height height(static_cast<uint32_t>(raw_font->ascent + raw_font->descent));

    return Result<X11Font, X11FontError>(
        X11Font(XftFontSharedResource(raw_font, XftFontDeleter(dpy)), pattern, type, std::move(height)));
}

auto X11Font::Create(const X11DisplaySharedResource &dpy, FcPattern *font_pattern, const X11Font::Type type) noexcept
    -> Result<X11Font, X11FontError>
{
    if (font_pattern == nullptr)
    {
        return Result<X11Font, X11FontError>({"Error, no font name was specified", RUNTIME_INFO});
    }

    XftFont *raw_font = XftFontOpenPattern(dpy->Raw(), font_pattern);
    if (raw_font == nullptr)
    {
        return Result<X11Font, X11FontError>({"Error, could not load font from pattern", RUNTIME_INFO});
    }

    Height height(static_cast<uint32_t>(raw_font->ascent + raw_font->descent));

    return Result<X11Font, X11FontError>(
        X11Font(XftFontSharedResource(raw_font, XftFontDeleter(dpy)), font_pattern, type, std::move(height)));
}

auto X11Font::type() const noexcept -> std::optional<Type>
{
    return m_type;
}

auto X11Font::xftfont() const noexcept -> const XftFontSharedResource &
{
    return m_xftfont;
}

auto X11Font::height() const noexcept -> Height
{
    return m_height;
}

auto X11Font::pattern() const noexcept -> FcPattern *
{
    return m_pattern;
}

} // namespace Tilebox
