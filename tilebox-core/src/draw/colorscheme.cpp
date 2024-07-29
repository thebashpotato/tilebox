#include "tilebox-core/draw/colorscheme.hpp"
#include "tilebox-core/draw/color.hpp"
#include "tilebox-core/error.hpp"
#include "tilebox-core/vendor/etl.hpp"
#include <optional>
#include <stdexcept>
#include <utility>

using namespace tilebox::core;
using namespace etl;

X11ColorScheme::X11ColorScheme(ColorSchemeArray &&colors) noexcept : _colors(std::move(colors))
{
}

auto X11ColorScheme::create(const X11DisplaySharedResource &dpy, const std::string &fg, const std::string &bg, // NOLINT
                            const std::string &border) noexcept -> Result<X11ColorScheme, X11ColorError>
{
    ColorSchemeArray colors;

    try
    {
        // create the foreground color
        if (const auto fg_res = X11Color::create(dpy, fg); fg_res.is_ok())
        {
            colors.at(Index::Foreground) = std::move(fg_res.ok().value());
        }
        else
        {
            return Result<X11ColorScheme, X11ColorError>(fg_res.err().value());
        }

        // create the background color
        if (const auto bg_res = X11Color::create(dpy, bg); bg_res.is_ok())
        {
            colors.at(Index::Background) = std::move(bg_res.ok().value());
        }
        else
        {
            return Result<X11ColorScheme, X11ColorError>(bg_res.err().value());
        }

        // create the border color
        if (const auto border_res = X11Color::create(dpy, border); border_res.is_ok())
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

    return Result<X11ColorScheme, X11ColorError>(X11ColorScheme(std::move(colors)));
}

auto X11ColorScheme::get_color(const X11ColorScheme::Index color_index) noexcept -> const X11Color &
{
    return this->_colors[color_index];
}
