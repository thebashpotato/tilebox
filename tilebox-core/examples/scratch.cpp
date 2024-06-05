#include <cstdlib>
#include <fmt/core.h>
#include <tilebox-core/draw/font.hpp>
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

    auto x11_font_result = X11Font::create(dpy, "JetBrainsMono Nerd Font Mono:size=16");

    if (x11_font_result.is_ok())
    {
        const X11Font font = x11_font_result.ok().value();
        fmt::println("Font Height: {}", font.height().value);
    }
    else
    {
        fmt::println("{}", x11_font_result.err().value().info());
    }
    dpy->sync();

    fmt::println("Done...");

    return EXIT_SUCCESS;
}
