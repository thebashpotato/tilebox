#include "tilebox-core/draw/colorscheme.hpp"
#include "tilebox-core/draw/color.hpp"
#include "tilebox-core/draw/colorscheme_config.hpp"
#include "tilebox-core/error.hpp"
#include "tilebox-core/vendor/etl.hpp"
#include "tilebox-core/x11/display.hpp"
#include <optional>
#include <stdexcept>
#include <utility>

using namespace tilebox::core;
using namespace etl;

X11ColorScheme::X11ColorScheme(ColorSchemeArray &&colors, const ColorSchemeKind kind) noexcept
    : _colors(std::move(colors)), _kind(kind)
{
}

auto X11ColorScheme::create(const X11DisplaySharedResource &dpy,
                            const ColorSchemeConfig &config) noexcept -> Result<X11ColorScheme, X11ColorError>
{
    ColorSchemeArray colors;

    try
    {
        // create the foreground color
        if (const auto fg_res = X11Color::create(dpy, config.foreground()); fg_res.is_ok())
        {
            colors.at(Index::Foreground) = std::move(fg_res.ok().value());
        }
        else
        {
            return Result<X11ColorScheme, X11ColorError>(fg_res.err().value());
        }

        // create the background color
        if (const auto bg_res = X11Color::create(dpy, config.background()); bg_res.is_ok())
        {
            colors.at(Index::Background) = std::move(bg_res.ok().value());
        }
        else
        {
            return Result<X11ColorScheme, X11ColorError>(bg_res.err().value());
        }

        // create the border color
        if (const auto border_res = X11Color::create(dpy, config.border()); border_res.is_ok())
        {
            colors.at(Index::Border) = std::move(border_res.ok().value());
        }
        else
        {
            return Result<X11ColorScheme, X11ColorError>(border_res.err().value());
        }
    }
    catch (const std::out_of_range &error)
    {
        return Result<X11ColorScheme, X11ColorError>(X11ColorError(error.what(), RUNTIME_INFO));
    }

    return Result<X11ColorScheme, X11ColorError>(X11ColorScheme(std::move(colors), config.kind()));
}

auto X11ColorScheme::kind() const noexcept -> ColorSchemeKind
{
    return _kind;
}

auto X11ColorScheme::get_color(const X11ColorScheme::Index color_index) const noexcept -> const X11Color &
{
    return this->_colors[color_index];
}
