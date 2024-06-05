#pragma once

#include <cstdio>
#include <source_location>

namespace tilebox::core
{
auto inline debug_print(const std::source_location &location = std::source_location::current()) noexcept -> void
{
    std::puts(location.function_name());
}

} // namespace tilebox::core
