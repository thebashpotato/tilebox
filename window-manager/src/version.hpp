#pragma once

#include <string>

namespace tilebox::wm
{

constexpr auto major_version = 0;
constexpr auto minor_version = 1;
constexpr auto patch_version = 0;

auto version() -> std::string;

} // namespace tilebox::wm
