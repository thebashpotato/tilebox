#include "version.hpp"
#include <cstdlib>
#include <fmt/core.h>
#include <tilebox-core/version.hpp>
#include <tilebox-core/x11/display.hpp>

auto main() -> int
{
    fmt::println("Tilebox Core Version: {}", tilebox::core::version());
    fmt::println("Tilebox Window Manager Version: {}", tilebox::wm::version());

    auto display = tilebox::core::X11Display::create();

    if (!display->is_connected())
    {
        fmt::println("Failed to open X display");
        return EXIT_FAILURE;
    }

    fmt::println("{}", display->server_vendor());

    return EXIT_SUCCESS;
}
