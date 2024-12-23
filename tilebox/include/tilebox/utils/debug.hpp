#pragma once

#include <cstdio>
#include <source_location>
#include <string>

namespace Tilebox
{

inline void debug_print(const std::source_location &location = std::source_location::current()) noexcept
{
    const auto info = std::string{location.function_name()}.append(": ").append(std::to_string(location.line()));
    std::puts(info.c_str());
}

} // namespace Tilebox
