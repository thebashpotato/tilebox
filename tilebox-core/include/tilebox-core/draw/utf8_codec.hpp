#pragma once

#include "tilebox-core/error.hpp"
#include "tilebox-core/vendor/etl.hpp"
#include <string>

namespace tilebox::core
{

[[nodiscard]] auto utf8_decode(const std::string &input) noexcept -> etl::Result<std::u32string, X11FontError>;

[[nodiscard]] auto utf8_encode(const std::u32string &input) noexcept -> etl::Result<std::string, X11FontError>;

} // namespace tilebox::core
