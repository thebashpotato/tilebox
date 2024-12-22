#include <spdlog/spdlog.h>

#include <cstdlib>
#include <tilebox/config.hpp>
#include <tilebox/core.hpp>

auto main() -> int
{
    spdlog::info("{} {}", Tilebox::kTileboxName, Tilebox::kTileboxVersion);
    spdlog::info("Factorial 10 = {}", Tilebox::Factorial(10));

    return EXIT_SUCCESS;
}
