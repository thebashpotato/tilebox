#include "tilebox/draw/draw.hpp"
#include "tilebox/draw/colorscheme.hpp"
#include "tilebox/draw/colorscheme_config.hpp"
#include "tilebox/draw/cursor.hpp"
#include "tilebox/draw/font.hpp"
#include "tilebox/error.hpp"
#include "tilebox/geometry.hpp"
#include "tilebox/x11/display.hpp"

#include <X11/X.h>
#include <X11/Xft/Xft.h>
#include <X11/Xft/XftCompat.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <etl.hpp>

#include <algorithm>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace etl;

namespace Tilebox
{

X11Draw::X11Draw(X11DisplaySharedResource dpy, GC graphics_ctx, const Drawable drawable, Width width,
                 Height height) noexcept
    : m_dpy(std::move(dpy)), m_graphics_ctx(graphics_ctx), m_drawable(drawable), m_width(std::move(width)),
      m_height(std::move(height))
{
    m_colorschemes.reserve(ColorSchemeKindIterator::size() + 1);
}

X11Draw::~X11Draw() noexcept
{
    if (m_drawable != False && m_dpy->IsConnected())
    {
        XFreePixmap(m_dpy->Raw(), m_drawable);
        m_drawable = False;
    }

    if (m_graphics_ctx != nullptr && m_dpy->IsConnected())
    {
        XFreeGC(m_dpy->Raw(), m_graphics_ctx);
        m_graphics_ctx = nullptr;
    }
}

X11Draw::X11Draw(X11Draw &&rhs) noexcept
    : m_dpy(std::move(rhs.m_dpy)), m_fonts(std::move(rhs.m_fonts)), m_cursors(std::move(rhs.m_cursors)),
      m_colorschemes(std::move(rhs.m_colorschemes)), m_graphics_ctx(rhs.m_graphics_ctx), m_drawable(rhs.m_drawable),
      m_width(std::move(rhs.m_width)), m_height(std::move(rhs.m_height))
{
    rhs.m_drawable = False;
    rhs.m_graphics_ctx = nullptr;
}

auto X11Draw::operator=(X11Draw &&rhs) noexcept -> X11Draw &
{
    if (this != &rhs)
    {
        m_height = std::move(rhs.m_height);
        m_width = std::move(rhs.m_width);

        if (m_drawable != False)
        {
            XFreePixmap(m_dpy->Raw(), m_drawable);
        }
        m_drawable = rhs.m_drawable;
        rhs.m_drawable = False;

        if (m_graphics_ctx != nullptr)
        {
            XFreeGC(m_dpy->Raw(), m_graphics_ctx);
        }
        m_graphics_ctx = rhs.m_graphics_ctx;
        rhs.m_graphics_ctx = nullptr;

        if (!m_colorschemes.empty())
        {
            m_colorschemes.clear();
        }
        m_colorschemes = std::move(rhs.m_colorschemes);

        m_cursors = std::move(rhs.m_cursors);

        m_fonts = std::move(rhs.m_fonts);

        m_dpy = std::move(rhs.m_dpy);
    }

    return *this;
}

auto X11Draw::Create(const X11DisplaySharedResource &dpy, const Width &width,
                     const Height &height) noexcept -> Result<X11Draw, Error>
{
    GC gc = XCreateGC(dpy->Raw(), dpy->GetRootWindow(), 0, nullptr);

    if (gc == nullptr)
    {
        return Result<X11Draw, Error>({
            "Failed to create graphics context",
            RUNTIME_INFO,
        });
    }

    // FIXME: I think if XCreatePixmap fails it will generate an X11 error through the callback function, thus
    // we can't verify that here.
    const Drawable drawable = XCreatePixmap(dpy->Raw(), dpy->GetRootWindow(), width.value, height.value,
                                            DefaultDepth(dpy->Raw(), dpy->ScreenId()));

    XSetLineAttributes(dpy->Raw(), gc, 1, LineSolid, CapButt, JoinMiter);

    return Result<X11Draw, Error>({
        dpy,
        gc,
        drawable,
        width,
        height,
    });
}

auto X11Draw::InitFont(const std::string &font_name, const X11Font::Type type) noexcept -> Result<Void, X11FontError>
{

    // if the underlying type is std::nullopt, we can initialize this font.
    if (!m_fonts[X11Font::ToUnderlying(type)].type().has_value())
    {
        if (const auto result = X11Font::Create(m_dpy, font_name, type); result.is_ok())
        {
            m_fonts[X11Font::ToUnderlying(type)] = std::move(*result.ok());
        }
        else
        {
            return Result<Void, X11FontError>(std::move(*result.err()));
        }
    }
    return Result<Void, X11FontError>(Void());
}

auto X11Draw::GetFont(const X11Font::Type type) const noexcept -> std::optional<X11Font>
{
    std::optional<X11Font> ret;
    if (m_fonts[X11Font::ToUnderlying(type)].type().has_value())
    {
        ret.emplace(m_fonts[X11Font::ToUnderlying(type)]);
    }

    return ret;
}

auto X11Draw::InitColorScheme(const ColorSchemeConfig &config) noexcept -> Result<Void, X11ColorError>
{
    // check if the colorscheme kind is already defined, if it is, it will not be redefined
    const auto it = std::ranges::find_if(m_colorschemes, [&config](const X11ColorScheme &colorscheme) -> bool {
        return colorscheme.Kind() == config.kind();
    });

    // The colorscheme kind is not yet defined, we will create it and add it to the colorscheme vector.
    // otherwise just return Result<Void>
    if (it == m_colorschemes.end())
    {
        if (const auto result = X11ColorScheme::Create(m_dpy, config); result.is_ok())
        {
            m_colorschemes.emplace_back(std::move(result.ok().value()));
        }
        else
        {
            return Result<Void, X11ColorError>(std::move(result.err().value()));
        }
    }

    return Result<Void, X11ColorError>(Void());
}

auto X11Draw::RemoveColorScheme(const ColorSchemeKind kind) noexcept -> bool
{
    const auto original_size = m_colorschemes.size();
    std::erase_if(m_colorschemes, [&kind](const X11ColorScheme &colorscheme) { return colorscheme.Kind() == kind; });

    return original_size != m_colorschemes.size();
}

auto X11Draw::GetColorScheme(const ColorSchemeKind kind) const noexcept -> std::optional<X11ColorScheme>
{
    std::optional<X11ColorScheme> ret;

    const auto it = std::ranges::find_if(
        m_colorschemes, [&kind](const X11ColorScheme &colorscheme) { return colorscheme.Kind() == kind; });

    if (it != m_colorschemes.end())
    {
        ret.emplace(*it);
    }

    return ret;
}

auto X11Draw::InitCursor(const X11Cursor::Type type) noexcept -> Result<Void, X11CursorError>
{
    // if the underlying type is std::nullopt, we can initialize this font.
    if (!m_cursors[X11Cursor::ToUnderlying(type)].type().has_value())
    {
        if (auto result = X11Cursor::Create(m_dpy, type); result.is_ok())
        {
            m_cursors[X11Cursor::ToUnderlying(type)] = std::move(*result.ok());
        }
        else
        {
            return Result<Void, X11CursorError>(std::move(*result.err()));
        }
    }
    return Result<Void, X11CursorError>(Void());
}

auto X11Draw::InitCursorAll() noexcept -> Result<Void, X11CursorError>
{
    for (const auto &cursor_type : X11Cursor::TypeIterator())
    {
        if (auto res = InitCursor(cursor_type); res.is_err())
        {
            return Result<Void, X11CursorError>(std::move(res.err().value()));
        }
    }

    return Result<Void, X11CursorError>(Void());
}

auto X11Draw::GetCursor(const X11Cursor::Type type) const noexcept -> std::optional<Cursor>
{
    std::optional<Cursor> ret;
    if (m_cursors[X11Cursor::ToUnderlying(type)].type().has_value())
    {
        ret.emplace(m_cursors[X11Cursor::ToUnderlying(type)].cursor());
    }

    return ret;
}

auto X11Draw::Resize(const Width &width, const Height &height) noexcept -> void
{
    m_width = width;
    m_height = height;
    if (m_drawable != False)
    {
        XFreePixmap(m_dpy->Raw(), m_drawable);
    }
    m_drawable = XCreatePixmap(m_dpy->Raw(), m_dpy->GetRootWindow(), m_width.value, m_height.value,
                               DefaultDepth(m_dpy->Raw(), m_dpy->ScreenId()));
}

auto X11Draw::width() const noexcept -> const Width &
{
    return m_width;
}

auto X11Draw::height() const noexcept -> const Height &
{
    return m_height;
}

auto X11Draw::GetTextExtents(const X11Font &font, const std::string_view &text,
                             const uint32_t len) const noexcept -> Result<Vec2D, X11FontError>
{
    XGlyphInfo ext;

    if (text.empty())
    {
        return Result<Vec2D, X11FontError>({"Cannot get text_extents, the text is empty", RUNTIME_INFO});
    }

    XftTextExtentsUtf8(m_dpy->Raw(), font.xftfont().get(), reinterpret_cast<const XftChar8 *>(text.data()),
                       static_cast<int32_t>(len), &ext);

    return Result<Vec2D, X11FontError>({Width(static_cast<uint32_t>(ext.xOff)), font.height()});
}

} // namespace Tilebox
