#pragma once

#include <fmt/base.h>
#include <memory>
#include <spdlog/logger.h>
#include <string_view>
#include <utility>

namespace Tilebox::Twm
{

class Logging
{
  public:
    static void Init(const std::string_view &name) noexcept;

    static inline auto logger() noexcept -> std::shared_ptr<spdlog::logger> &
    {
        return s_logger;
    }

  private:
    static std::shared_ptr<spdlog::logger> s_logger;
};

namespace Log
{

template <typename... Args> void Debug(fmt::format_string<Args...> fmt, Args &&...args)
{
    if (Logging::logger())
    {
        Logging::logger()->debug(fmt, std::forward<Args>(args)...);
    }
}

template <typename... Args> void Info(fmt::format_string<Args...> fmt, Args &&...args)
{
    if (Logging::logger())
    {
        Logging::logger()->info(fmt, std::forward<Args>(args)...);
    }
}

template <typename... Args> void Warn(fmt::format_string<Args...> fmt, Args &&...args)
{
    if (Logging::logger())
    {
        Logging::logger()->warn(fmt, std::forward<Args>(args)...);
    }
}

template <typename... Args> void Error(fmt::format_string<Args...> fmt, Args &&...args)
{
    if (Logging::logger())
    {
        Logging::logger()->error(fmt, std::forward<Args>(args)...);
    }
}

} // namespace Log

} // namespace Tilebox::Twm
