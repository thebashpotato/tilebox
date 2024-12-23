#include "tilebox/draw/draw.hpp"
#include "tilebox/draw/colorscheme.hpp"
#include "tilebox/draw/colorscheme_config.hpp"
#include "tilebox/draw/cursor.hpp"
#include "tilebox/draw/font.hpp"
#include "tilebox/error.hpp"
#include "tilebox/geometry.hpp"
#include "tilebox/vendor/etl.hpp"
#include "tilebox/x11/display.hpp"

#include <X11/X.h>
#include <X11/Xft/Xft.h>
#include <X11/Xft/XftCompat.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>

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

// ReSharper disable once CppParameterMayBeConst
X11Draw::X11Draw(X11DisplaySharedResource dpy, GC graphics_ctx, const Drawable drawable, Width width,
                 Height height) noexcept
    : _dpy(std::move(dpy)), _graphics_ctx(graphics_ctx), _drawable(drawable), _width(std::move(width)),
      _height(std::move(height))
{
    _colorschemes.reserve(ColorSchemeKindSize());
}

X11Draw::~X11Draw() noexcept
{
    if (_drawable != False && _dpy->is_connected())
    {
        XFreePixmap(_dpy->raw(), _drawable);
        _drawable = False;
    }

    if (_graphics_ctx != nullptr && _dpy->is_connected())
    {
        XFreeGC(_dpy->raw(), _graphics_ctx);
        _graphics_ctx = nullptr;
    }
}

X11Draw::X11Draw(X11Draw &&rhs) noexcept
    : _dpy(std::move(rhs._dpy)), _fonts(std::move(rhs._fonts)), _cursors(std::move(rhs._cursors)),
      _colorschemes(std::move(rhs._colorschemes)), _graphics_ctx(rhs._graphics_ctx), _drawable(rhs._drawable),
      _width(std::move(rhs._width)), _height(std::move(rhs._height))
{
    rhs._drawable = False;
    rhs._graphics_ctx = nullptr;
}

auto X11Draw::operator=(X11Draw &&rhs) noexcept -> X11Draw &
{
    if (this != &rhs)
    {
        _height = std::move(rhs._height);
        _width = std::move(rhs._width);

        if (_drawable != False)
        {
            XFreePixmap(_dpy->raw(), _drawable);
        }
        _drawable = rhs._drawable;
        rhs._drawable = False;

        if (_graphics_ctx != nullptr)
        {
            XFreeGC(_dpy->raw(), _graphics_ctx);
        }
        _graphics_ctx = rhs._graphics_ctx;
        rhs._graphics_ctx = nullptr;

        if (!_colorschemes.empty())
        {
            _colorschemes.clear();
        }
        _colorschemes = std::move(rhs._colorschemes);

        _cursors = std::move(rhs._cursors);

        _fonts = std::move(rhs._fonts);

        _dpy = std::move(rhs._dpy);
    }

    return *this;
}

auto X11Draw::Create(const X11DisplaySharedResource &dpy, const Width &width,
                     const Height &height) noexcept -> Result<X11Draw, CoreError>
{
    GC gc = XCreateGC(dpy->raw(), dpy->root_window(), 0, nullptr);

    if (gc == nullptr)
    {
        return Result<X11Draw, CoreError>({
            "Failed to create graphics context",
            RUNTIME_INFO,
        });
    }

    // FIXME: I think if XCreatePixmap fails it will generate an X11 error through the callback function, thus
    // we can't verify that here.
    const Drawable drawable = XCreatePixmap(dpy->raw(), dpy->root_window(), width.value, height.value,
                                            DefaultDepth(dpy->raw(), dpy->screen_id()));

    XSetLineAttributes(dpy->raw(), gc, 1, LineSolid, CapButt, JoinMiter);

    return Result<X11Draw, CoreError>({
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
    if (!_fonts[X11Font::ToUnderlying(type)].type().has_value())
    {
        if (const auto result = X11Font::Create(_dpy, font_name, type); result.is_ok())
        {
            _fonts[X11Font::ToUnderlying(type)] = std::move(*result.ok());
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
    if (_fonts[X11Font::ToUnderlying(type)].type().has_value())
    {
        ret.emplace(_fonts[X11Font::ToUnderlying(type)]);
    }

    return ret;
}

auto X11Draw::InitColorScheme(const ColorSchemeConfig &config) noexcept -> Result<Void, X11ColorError>
{
    // check if the colorscheme kind is already defined, if it is, it will not be redefined
    const auto it = std::ranges::find_if(_colorschemes, [&config](const X11ColorScheme &colorscheme) -> bool {
        return colorscheme.Kind() == config.kind();
    });

    // The colorscheme kind is not yet defined, we will create it and add it to the colorscheme vector.
    // otherwise just return Result<Void>
    if (it == _colorschemes.end())
    {
        if (const auto result = X11ColorScheme::Create(_dpy, config); result.is_ok())
        {
            _colorschemes.emplace_back(std::move(result.ok().value()));
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
    const auto original_size = _colorschemes.size();
    std::erase_if(_colorschemes, [&kind](const X11ColorScheme &colorscheme) { return colorscheme.Kind() == kind; });

    return original_size != _colorschemes.size();
}

auto X11Draw::GetColorScheme(const ColorSchemeKind kind) const noexcept -> std::optional<X11ColorScheme>
{
    std::optional<X11ColorScheme> ret;

    const auto it = std::ranges::find_if(
        _colorschemes, [&kind](const X11ColorScheme &colorscheme) { return colorscheme.Kind() == kind; });

    if (it != _colorschemes.end())
    {
        ret.emplace(*it);
    }

    return ret;
}

auto X11Draw::InitCursor(const X11Cursor::Type type) noexcept -> etl::Result<etl::Void, X11CursorError>
{
    // if the underlying type is std::nullopt, we can initialize this font.
    if (!_cursors[X11Cursor::ToUnderlying(type)].type().has_value())
    {
        if (auto result = X11Cursor::Create(_dpy, type); result.is_ok())
        {
            _cursors[X11Cursor::ToUnderlying(type)] = std::move(*result.ok());
        }
        else
        {
            return Result<Void, X11CursorError>(std::move(*result.err()));
        }
    }
    return Result<Void, X11CursorError>(Void());
}

auto X11Draw::GetCursor(const X11Cursor::Type type) const noexcept -> std::optional<Cursor>
{
    std::optional<Cursor> ret;
    if (_cursors[X11Cursor::ToUnderlying(type)].type().has_value())
    {
        ret.emplace(_cursors[X11Cursor::ToUnderlying(type)].CursorId());
    }

    return ret;
}

auto X11Draw::Resize(const Width &width, const Height &height) noexcept -> void
{
    _width = width;
    _height = height;
    if (_drawable != False)
    {
        XFreePixmap(_dpy->raw(), _drawable);
    }
    _drawable = XCreatePixmap(_dpy->raw(), _dpy->root_window(), _width.value, _height.value,
                              DefaultDepth(_dpy->raw(), _dpy->screen_id()));
}

auto X11Draw::width() const noexcept -> const Width &
{
    return _width;
}

auto X11Draw::height() const noexcept -> const Height &
{
    return _height;
}

auto X11Draw::GetTextExtents(const X11Font &font, const std::string_view &text,
                             const uint32_t len) const noexcept -> Result<Vec2D, X11FontError>
{
    XGlyphInfo ext;

    if (text.empty())
    {
        return Result<Vec2D, X11FontError>({"Cannot get text_extents, the text is empty", RUNTIME_INFO});
    }

    XftTextExtentsUtf8(_dpy->raw(), font.xftfont().get(), reinterpret_cast<const XftChar8 *>(text.data()),
                       static_cast<int32_t>(len), &ext);

    return Result<Vec2D, X11FontError>({Width(static_cast<uint32_t>(ext.xOff)), font.height()});
}

} // namespace Tilebox
