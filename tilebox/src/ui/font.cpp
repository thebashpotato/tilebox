#include <cassert>
#include <cstdint>
#include <string>
#include <utility>

#include <X11/Xft/Xft.h>
#include <X11/Xft/XftCompat.h>
#include <X11/extensions/Xrender.h>
#include <etl.hpp>
#include <fontconfig/fontconfig.h>
#include <tilebox/error.hpp>
#include <tilebox/geometry.hpp>
#include <tilebox/x11/display.hpp>

#include "font.hpp"

using namespace etl;

namespace Tilebox::Ui
{

XftFontDeleter::XftFontDeleter(X11DisplaySharedResource display) noexcept : dpy(std::move(display))
{
}

void XftFontDeleter::operator()(XftFont *font) const noexcept
{
    if (dpy && font != nullptr)
    {
        XftFontClose(dpy->Raw(), std::exchange(font, nullptr));
    }
}

X11Font::X11Font(XftFontPtr font, FcPattern *pattern, Height height) noexcept
    : m_font(std::move(font)), m_pattern(pattern), m_height(std::move(height))
{
}

X11Font::~X11Font()
{
    if (m_pattern != nullptr)
    {
        FcPatternDestroy(std::exchange(m_pattern, nullptr));
    }
}

X11Font::X11Font(X11Font &&rhs) noexcept
    : m_font(std::move(rhs.m_font)), m_pattern(std::exchange(rhs.m_pattern, nullptr)), m_height(std::move(rhs.m_height))
{
}

auto X11Font::operator=(X11Font &&rhs) noexcept -> X11Font &
{
    if (this != &rhs)
    {
        m_font = std::move(rhs.m_font);
        m_pattern = std::exchange(rhs.m_pattern, nullptr);
        m_height = std::move(rhs.m_height);
    }
    return *this;
}

auto X11Font::TryCreate(const X11DisplaySharedResource &dpy, const std::string &font_name) noexcept
    -> Result<X11Font, X11FontError>
{
    if (font_name.empty())
    {
        return Result<X11Font, X11FontError>({"Error, empty font name", RUNTIME_INFO});
    }

    // Using the pattern found at `_xftfont->pattern` does not yield the
    // same substitution results as using the pattern returned by
    // FcNameParse; using the latter results in the desired fallback
    // behaviour whereas the former just results in missing-character
    // rectangles being drawn, at least with some fonts.
    XftFont *xf = XftFontOpenName(dpy->Raw(), dpy->ScreenId(), font_name.c_str());
    if (xf == nullptr)
    {
        return Result<X11Font, X11FontError>(
            {std::string{"Error, could not load font from: "}.append(font_name), RUNTIME_INFO});
    }

    FcPattern *fcp = FcNameParse(reinterpret_cast<const FcChar8 *>(font_name.c_str()));
    if (fcp == nullptr)
    {
        XftFontClose(dpy->Raw(), xf);
        return Result<X11Font, X11FontError>(
            {std::string{"Error, could not parse font name from pattern: "}.append(font_name), RUNTIME_INFO});
    }

    Height h(static_cast<uint32_t>(xf->ascent + xf->descent));

    return Result<X11Font, X11FontError>(X11Font(XftFontPtr(xf, XftFontDeleter(dpy)), fcp, std::move(h)));
}

auto X11Font::TryCreate(const X11DisplaySharedResource &dpy, FcPattern *const fcp) noexcept
    -> Result<X11Font, X11FontError>
{
    if (fcp == nullptr)
    {
        return Result<X11Font, X11FontError>({"Error, no font type was specified", RUNTIME_INFO});
    }

    XftFont *const xf = XftFontOpenPattern(dpy->Raw(), fcp);
    if (xf == nullptr)
    {
        return Result<X11Font, X11FontError>({"Error, could not load font from pattern", RUNTIME_INFO});
    }

    Height h(static_cast<uint32_t>(xf->ascent + xf->descent));

    return Result<X11Font, X11FontError>(X11Font(XftFontPtr(xf, XftFontDeleter(dpy)), fcp, std::move(h)));
}

auto X11Font::ContainsChar(const X11DisplaySharedResource &dpy, const char32_t c) const noexcept -> bool
{
    return (XftCharExists(dpy->Raw(), m_font.get(), static_cast<FcChar32>(c)) == 1);
}

auto X11Font::GetTextExtents(const X11DisplaySharedResource &dpy, const std::string_view &text) const noexcept
    -> etl::Result<std::pair<std::uint32_t, std::uint32_t>, X11FontError>
{
    XGlyphInfo ext{};
    if (text.empty())
    {
        return Result<std::pair<std::uint32_t, std::uint32_t>, X11FontError>(
            {"Cannot get text_extents, the text is empty", RUNTIME_INFO});
    }

    XftTextExtentsUtf8(dpy->Raw(), m_font.get(), reinterpret_cast<const XftChar8 *>(text.data()),
                       static_cast<int32_t>(text.size()), &ext);

    const auto x_off = static_cast<std::uint32_t>(ext.xOff);
    return Result<std::pair<std::uint32_t, std::uint32_t>, X11FontError>({x_off, m_height.value});
}

auto X11Font::FallbackForChar(const X11DisplaySharedResource &dpy, const char32_t c) const noexcept
    -> Result<X11Font, X11FontError>
{
    if (const auto fm_result = this->FontMatch(dpy, c); fm_result.is_ok())
    {
        FcPattern *const pattern = *fm_result.ok();

        return X11Font::TryCreate(dpy, pattern);
    }
    else
    {
        return Result<X11Font, X11FontError>(std::move(fm_result.err().value()));
    }
}

auto X11Font::FontMatch(const X11DisplaySharedResource &dpy, const char32_t c) const noexcept
    -> Result<FcPattern *const, X11FontError>
{
    FcCharSet *const charset = FcCharSetCreate();
    FcCharSetAddChar(charset, static_cast<FcChar32>(c));

    FcPattern *const dup_pattern = FcPatternDuplicate(m_pattern);
    if (dup_pattern == nullptr)
    {
        FcCharSetDestroy(charset);
        return Result<FcPattern *const, X11FontError>({"FcPatternDuplicate failed", RUNTIME_INFO});
    }

    FcPatternAddCharSet(dup_pattern, FC_CHARSET, charset);
    FcPatternAddBool(dup_pattern, FC_SCALABLE, FcTrue);

    FcConfigSubstitute(nullptr, dup_pattern, FcMatchPattern);
    FcDefaultSubstitute(dup_pattern);

    auto fc_result = FcResultNoMatch;

    auto *const fc_pattern = XftFontMatch(dpy->Raw(), dpy->ScreenId(), dup_pattern, &fc_result);

    FcCharSetDestroy(charset);
    FcPatternDestroy(dup_pattern);

    if (fc_pattern == nullptr)
    {
        return Result<FcPattern *const, X11FontError>(
            {std::string{"No fallback font for character: "}.append(std::to_string(c)), RUNTIME_INFO});
    }

    return Result<FcPattern *const, X11FontError>(fc_pattern);
}

auto X11Font::IsValid() const noexcept -> bool
{
    return m_font != nullptr && m_pattern != nullptr;
}

auto X11Font::font() const noexcept -> XftFont *
{
    return m_font.get();
}

auto X11Font::height() const noexcept -> Height
{
    return m_height;
}

} // namespace Tilebox::Ui
