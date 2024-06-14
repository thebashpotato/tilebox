#include <cstdlib>
#include <fmt/core.h>
#include <tilebox-core/draw/font.hpp>
#include <tilebox-core/version.hpp>
#include <tilebox-core/x11/display.hpp>
#include <utility>

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

    const auto x11font_res1 = X11Font::create(dpy, "monospace:size=16");
    const auto x11font_res2 = X11Font::create(dpy, "monospace:size=12");

    if (x11font_res1.is_ok() && x11font_res2.is_ok())
    {
        X11Font font_1 = x11font_res1.ok().value();
        X11Font font_2 = x11font_res2.ok().value();
        fmt::println("Font Height: {}", font_1.height().value);
        fmt::println("Font Height 2: {}", font_2.height().value);
        font_2 = std::move(font_1);
        fmt::println("Font Height 2 after move: {}", font_2.height().value);
    }
    else
    {
        fmt::println("{}", x11font_res1.err().value().info());
    }
    dpy->sync();

    fmt::println("Done...");

    return EXIT_SUCCESS;
}
