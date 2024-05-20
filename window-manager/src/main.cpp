#include "version.hpp"
#include <cstdlib>
#include <fmt/core.h>
#include <optional>
#include <tilebox-core/version.hpp>
#include <tilebox-core/x11/display.hpp>

using namespace tilebox;

auto main() -> int
{
    fmt::println("Tilebox Core Version: {}", core::VERSION_STRING);
    fmt::println("Tilebox Window Manager Version: {}", VERSION_STRING);

    auto dpy_opt = core::X11Display::create();
    if (!dpy_opt.has_value())
    {
        fmt::println("Failed to open X display");
        return EXIT_FAILURE;
    }

    auto display = dpy_opt.value();
    fmt::println("{}", display->server_vendor());

    return EXIT_SUCCESS;
}
