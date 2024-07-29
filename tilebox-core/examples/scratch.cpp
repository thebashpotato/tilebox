#include <cstdlib>
#include <fmt/core.h>
#include <tilebox-core/draw/colorscheme.hpp>
#include <tilebox-core/version.hpp>
#include <tilebox-core/x11/display.hpp>

using namespace tilebox::core;

auto main() -> int
{
    fmt::println("Tilebox Core Version: {}", tilebox::core::VERSION_STRING);

    auto dpy_opt = X11Display::create();
    if (!dpy_opt.has_value())
    {
        fmt::println("Couldn't open X11Display");
        return EXIT_FAILURE;
    }

    const X11DisplaySharedResource dpy = dpy_opt.value();
    if (const auto scheme_res = X11ColorScheme::create(dpy, "#bbbbbb", "#222222", "#444444"); scheme_res.is_ok())
    {
        X11ColorScheme scheme = scheme_res.ok().value();

        const X11Color &fg = scheme.get_color(X11ColorScheme::Foreground);
        const X11Color &bg = scheme.get_color(X11ColorScheme::Background);
        const X11Color &border = scheme.get_color(X11ColorScheme::Border);

        fmt::println("Fg Pixel {}", fg.raw()->pixel);
        fmt::println("Bg Pixel {}", bg.raw()->pixel);
        fmt::println("Border Pixel {}", border.raw()->pixel);
    }
    else
    {
        fmt::println("{}", scheme_res.err().value().info());
    }

    return EXIT_SUCCESS;
}
