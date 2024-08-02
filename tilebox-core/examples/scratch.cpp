#include <cstdint>
#include <cstdlib>
#include <fmt/core.h>
#include <tilebox-core/draw/draw.hpp>
#include <tilebox-core/geometry.hpp>
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
    auto draw_res = X11Draw::create(dpy, Width(static_cast<uint32_t>(dpy->screen_width())),
                                    Height(static_cast<uint32_t>(dpy->screen_height())));

    if (draw_res.is_err())
    {
        fmt::println("Couldn't instantiate draw object");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
