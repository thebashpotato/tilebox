#include "log.hpp"
#include "wm.hpp"

#include <cstdlib>
#include <utility>

using namespace Tilebox;

auto main() -> int
{
    if (const auto create_res = Twm::WindowManager::Create(); create_res.is_ok())
    {
        const Twm::WindowManager wm = std::move(*create_res.ok());
        if (const auto start_res = wm.Start(); start_res.is_err())
        {
            Twm::Log::Error("{}", start_res.err().value()->msg());
            return EXIT_FAILURE;
        }
        Twm::Log::Info("Shutting down");
    }

    return EXIT_SUCCESS;
}
