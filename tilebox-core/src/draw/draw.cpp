#include "tilebox-core/draw/draw.hpp"
#include "tilebox-core/draw/colorscheme.hpp"
#include "tilebox-core/draw/colorscheme_config.hpp"
#include "tilebox-core/draw/font.hpp"
#include "tilebox-core/error.hpp"
#include "tilebox-core/geometry.hpp"
#include "tilebox-core/vendor/etl.hpp"
#include "tilebox-core/x11/display.hpp"
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

using namespace etl;
using namespace tilebox::core;

X11Draw::X11Draw(X11DisplaySharedResource dpy, GC graphics_ctx, Drawable drawable, Width width, Height height) noexcept
    : _dpy(std::move(dpy)), _graphics_ctx(graphics_ctx), _drawable(drawable), _width(std::move(width)),
      _height(std::move(height))
{
    _fonts.reserve(2);
    _colorschemes.reserve(2);
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
    : _dpy(std::move(rhs._dpy)), _fonts(std::move(rhs._fonts)), _colorschemes(std::move(rhs._colorschemes)),
      _graphics_ctx(rhs._graphics_ctx), _drawable(rhs._drawable), _width(std::move(rhs._width)),
      _height(std::move(rhs._height))
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

        if (!_fonts.empty())
        {
            _fonts.clear();
        }

        _fonts = std::move(rhs._fonts);

        if (!_colorschemes.empty())
        {
            _colorschemes.clear();
        }

        _colorschemes = std::move(rhs._colorschemes);

        _dpy = std::move(rhs._dpy);
    }

    return *this;
}

auto X11Draw::create(const X11DisplaySharedResource &dpy, const Width &width,
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

auto X11Draw::add_font_set(const std::string &font_name) noexcept -> Result<Void, X11FontError>
{
    if (const auto font_res = X11Font::create(_dpy, font_name); font_res.is_ok())
    {
        _fonts.emplace_back(std::move(font_res.ok().value()));
        return Result<Void, X11FontError>(Void());
    }
    else
    {
        return Result<Void, X11FontError>(font_res.err().value());
    }
}

auto X11Draw::add_colorscheme(const ColorSchemeConfig &config) noexcept -> Result<Void, X11ColorError>
{
    // check if the colorscheme kind is already defined, if it is, it will not be redefined
    const auto it =
        std::find_if(_colorschemes.begin(), _colorschemes.end(), [&config](const X11ColorScheme &colorscheme) -> bool {
            return colorscheme.kind() == config.kind();
        });

    // The colorscheme kind is not yet defined, we will create it and add it to the colorscheme vector.
    // otherwise just return Result<Void>
    if (it == _colorschemes.end())
    {
        if (const auto colorscheme_res = X11ColorScheme::create(_dpy, config); colorscheme_res.is_ok())
        {
            _colorschemes.emplace_back(std::move(colorscheme_res.ok().value()));
        }
        else
        {
            return Result<Void, X11ColorError>(std::move(colorscheme_res.err().value()));
        }
    }

    return Result<Void, X11ColorError>(Void());
}

auto X11Draw::remove_colorscheme(const ColorSchemeKind kind) noexcept -> void
{
    _colorschemes.erase(
        std::remove_if(_colorschemes.begin(), _colorschemes.end(),
                       [&kind](const X11ColorScheme &colorscheme) { return colorscheme.kind() == kind; }),
        _colorschemes.end());
}

auto X11Draw::get_colorscheme(const ColorSchemeKind kind) const noexcept -> std::optional<X11ColorScheme>
{
    std::optional<X11ColorScheme> ret;

    const auto it = std::find_if(_colorschemes.begin(), _colorschemes.end(),
                                 [&kind](const X11ColorScheme &colorscheme) { return colorscheme.kind() == kind; });

    if (it != _colorschemes.end())
    {
        ret.emplace(*it);
    }

    return ret;
}

auto X11Draw::resize(const Width &width, const Height &height) noexcept -> void
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

auto X11Draw::_get_text_extents(const X11Font &font, const std::string_view &text,
                                const uint32_t len) noexcept -> Result<Vec2D, X11FontError>
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
