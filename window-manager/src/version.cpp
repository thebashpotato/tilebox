#include "version.hpp"
#include <string>

namespace tilebox::wm
{

auto version() -> std::string
{
    return std::to_string(major_version)
        .append(".")
        .append(std::to_string(minor_version))
        .append(".")
        .append(std::to_string(patch_version));
}

} // namespace tilebox::wm
