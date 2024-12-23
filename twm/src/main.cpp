#include <spdlog/spdlog.h>

#include <cstdlib>
#include <tilebox/config.hpp>

auto main() -> int
{
    spdlog::info("{} {}", Tilebox::kTileboxName, Tilebox::kTileboxVersion);

    return EXIT_SUCCESS;
}
