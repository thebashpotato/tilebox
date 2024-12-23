#include "tilebox/draw/colorscheme.hpp"
#include "tilebox/draw/color.hpp"
#include "tilebox/draw/colorscheme_config.hpp"
#include "tilebox/error.hpp"
#include "tilebox/vendor/etl.hpp"
#include "tilebox/x11/display.hpp"

#include <optional>
#include <stdexcept>
#include <utility>

using namespace etl;

namespace Tilebox
{

X11ColorScheme::X11ColorScheme(ColorSchemeArray &&colors, const ColorSchemeKind kind) noexcept
    : m_colors(std::move(colors)), m_kind(kind)
{
}

auto X11ColorScheme::Create(const X11DisplaySharedResource &dpy,
                            const ColorSchemeConfig &config) noexcept -> Result<X11ColorScheme, X11ColorError>
{
    ColorSchemeArray colors;

    try
    {
        // create the foreground color
        if (const auto fg_res = X11Color::Create(dpy, config.foreground()); fg_res.is_ok())
        {
            colors.at(Index::Foreground) = std::move(fg_res.ok().value());
        }
        else
        {
            return Result<X11ColorScheme, X11ColorError>(fg_res.err().value());
        }

        // create the background color
        if (const auto bg_res = X11Color::Create(dpy, config.background()); bg_res.is_ok())
        {
            colors.at(Index::Background) = std::move(bg_res.ok().value());
        }
        else
        {
            return Result<X11ColorScheme, X11ColorError>(bg_res.err().value());
        }

        // create the border color
        if (const auto border_res = X11Color::Create(dpy, config.border()); border_res.is_ok())
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

auto X11ColorScheme::Kind() const noexcept -> ColorSchemeKind
{
    return m_kind;
}

auto X11ColorScheme::GetColor(const Index color_index) const noexcept -> const X11Color &
{
    return this->m_colors[color_index];
}

} // namespace Tilebox
