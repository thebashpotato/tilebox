#include "version.hpp"
#include <fmt/core.h>
#include <tilebox-core/version.hpp>

auto main() -> int
{
    fmt::println("Tilebox Core Version: {}", tilebox::core::version());
    fmt::println("Tilebox Window Manager Version: {}", tilebox::wm::version());
}
