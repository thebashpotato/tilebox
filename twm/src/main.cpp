#include "log.hpp"

#include <cstdlib>
#include <tilebox/config.hpp>

auto main() -> int
{
    Tilebox::Twm::Logging::Init();
    Tilebox::Twm::Log::Info("Running lib{} version {}", Tilebox::kTileboxName, Tilebox::kTileboxVersion);

    return EXIT_SUCCESS;
}
