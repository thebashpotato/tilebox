#include "version.hpp"
#include <cstdlib>
#include <fmt/core.h>
#include <tilebox-core/version.hpp>
#include <tilebox-core/x11/display.hpp>

using namespace tilebox;

auto main() -> int
{
    fmt::println("Tilebox Core Version: {}", core::version());
    fmt::println("Tilebox Window Manager Version: {}", wm::version());

    auto display = core::x::X11Display();

    if (!display.is_connected())
    {
        fmt::println("Failed to open X display");
        return EXIT_FAILURE;
    }

    fmt::println("{}", display.server_vendor());

    return EXIT_SUCCESS;
}
