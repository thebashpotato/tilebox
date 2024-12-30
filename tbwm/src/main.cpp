#include "log.hpp"
#include "wm.hpp"

#include <cstdlib>
#include <utility>

using namespace Tilebox;

auto main() -> int
{
    auto twm_create_result = Twm::WindowManager::Create("tbwm");
    if (twm_create_result.is_err())
    {
        Twm::Log::Error("{}", twm_create_result.err().value().info());
        return EXIT_FAILURE;
    }

    Twm::WindowManager wm = std::move(*twm_create_result.ok());
    if (auto twm_start_result = wm.Start(); twm_start_result.is_err())
    {
        Twm::Log::Error("{}", twm_start_result.err().value()->info());
        return EXIT_FAILURE;
    }
    Twm::Log::Info("Shutting down");

    return EXIT_SUCCESS;
}
