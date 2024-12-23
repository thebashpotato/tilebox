#pragma once

#include "tilebox/error.hpp"
#include "tilebox/vendor/etl.hpp"

#include <string>

namespace Tilebox
{

[[nodiscard]] auto Utf8Decode(const std::string &input) noexcept -> etl::Result<std::u32string, X11FontError>;

[[nodiscard]] auto Utf8Encode(const std::u32string &input) noexcept -> etl::Result<std::string, X11FontError>;

} // namespace Tilebox
