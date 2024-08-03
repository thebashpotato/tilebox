#pragma once

#include <cstdio>
#include <source_location>
#include <string>

namespace tilebox::core
{

auto inline debug_print(const std::source_location &location = std::source_location::current()) noexcept -> void
{
    const auto info = std::string{location.function_name()}.append(": ").append(std::to_string(location.line()));
    std::puts(info.c_str());
}

} // namespace tilebox::core
