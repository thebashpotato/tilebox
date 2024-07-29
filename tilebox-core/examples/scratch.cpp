#include <cstdlib>
#include <fmt/core.h>
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

    [[maybe_unused]] const X11DisplaySharedResource dpy = dpy_opt.value();

    return EXIT_SUCCESS;
}
