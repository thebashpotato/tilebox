#include "log.hpp"
#include "wm.hpp"

#include <cstdlib>
#include <utility>

auto main() -> int
{
    auto tbwm_create_result = Tbwm::WindowManager::Create("tbwm");
    if (tbwm_create_result.is_err())
    {
        Tbwm::Log::Error("{}", (*tbwm_create_result.err()).info());
        return EXIT_FAILURE;
    }

    Tbwm::WindowManager wm = std::move(*tbwm_create_result.ok());
    if (auto tbwm_start_result = wm.Start(); tbwm_start_result.is_err())
    {
        Tbwm::Log::Error("{}", (*tbwm_start_result.err())->info());
        return EXIT_FAILURE;
    }
    Tbwm::Log::Info("Shutting down");

    return EXIT_SUCCESS;
}
