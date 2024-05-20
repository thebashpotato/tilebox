#pragma once

#include <string_view>

namespace tilebox
{

constexpr auto VERSION_MAJOR = 0;
constexpr auto VERSION_MINOR = 1;
constexpr auto VERSION_PATCH = 0;
constexpr auto VERSION = (VERSION_MAJOR * 10000) + (VERSION_MINOR * 100) + VERSION_PATCH;
constexpr std::string_view VERSION_STRING = "0.1.0";

} // namespace tilebox
