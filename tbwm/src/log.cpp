#include "log.hpp"

#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <string_view>

#include <memory>

namespace Tbwm
{

std::shared_ptr<spdlog::logger> Logging::s_logger{};

void Logging::Init(const std::string_view &name) noexcept
{
    spdlog::set_pattern("%^[%T]::[%n]: %v%$");
    s_logger = spdlog::stdout_color_mt(std::string(name));
    s_logger->set_level(spdlog::level::trace);
}

} // namespace Tbwm
