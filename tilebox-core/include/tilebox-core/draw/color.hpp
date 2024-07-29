#pragma once

#include "tilebox-core/error.hpp"
#include "tilebox-core/utils/attributes.hpp"
#include "tilebox-core/vendor/etl.hpp"
#include "tilebox-core/x11/display.hpp"
#include <X11/Xft/Xft.h>
#include <memory>
#include <string>

namespace tilebox::core
{

struct TILEBOX_INTERNAL XftColorDeleter
{
    X11DisplaySharedResource dpy;

    explicit XftColorDeleter(X11DisplaySharedResource display) noexcept;

    auto operator()(XftColor *color) const noexcept -> void;
};

using XftColorSharedResource = std::shared_ptr<XftColor>;

/// @brief Provides an RAII interface to XftColor
class TILEBOX_INTERNAL X11Color
{
  public:
    X11Color() noexcept = default;

    [[nodiscard]] static auto create(const X11DisplaySharedResource &dpy,
                                     const std::string &hex_code) -> etl::Result<X11Color, X11ColorError>;

    [[nodiscard]] auto raw() const noexcept -> XftColor *;

  private:
    explicit X11Color(XftColorSharedResource &&color) noexcept;

  private:
    XftColorSharedResource _color;
};

} // namespace tilebox::core
