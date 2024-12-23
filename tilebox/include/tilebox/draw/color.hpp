#pragma once

#include "tilebox/error.hpp"
#include "tilebox/utils/attributes.hpp"
#include "tilebox/vendor/etl.hpp"
#include "tilebox/x11/display.hpp"

#include <X11/Xft/Xft.h>

#include <memory>
#include <string>

namespace Tilebox
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

    [[nodiscard]] static auto Create(const X11DisplaySharedResource &dpy,
                                     const std::string &hex_code) -> etl::Result<X11Color, X11ColorError>;

    [[nodiscard]] auto Raw() const noexcept -> XftColor *;

  private:
    explicit X11Color(XftColorSharedResource &&color) noexcept;

  private:
    XftColorSharedResource m_color;
};

} // namespace Tilebox
