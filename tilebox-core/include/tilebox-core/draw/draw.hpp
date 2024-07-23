#pragma once

#include "tilebox-core/draw/font.hpp"
#include "tilebox-core/error.hpp"
#include "tilebox-core/geometry.hpp"
#include "tilebox-core/utils/attributes.hpp"
#include "tilebox-core/vendor/etl.hpp"
#include "tilebox-core/x11/display.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <string_view>
#include <vector>

namespace tilebox::core
{

/// @brief The main class of the drawing library, it handles drawing text, and colors to rectangles.
class TILEBOX_EXPORT X11Draw
{
  public:
    ~X11Draw() noexcept;
    X11Draw(X11Draw &&rhs) noexcept;
    X11Draw(const X11Draw &rhs) noexcept = delete;

  public:
    auto operator=(X11Draw &&rhs) noexcept -> X11Draw &;
    auto operator=(const X11Draw &rhs) noexcept -> X11Draw & = delete;

  public:
    [[nodiscard]] static auto create(const X11DisplaySharedResource &dpy, const Width &width,
                                     const Height &height) noexcept -> etl::Result<X11Draw, CoreError>;

    [[nodiscard]] auto add_font(const std::string_view &font) noexcept -> etl::Result<etl::Void, X11FontError>;

    auto resize(const Width &width, const Height &height) noexcept -> void;

  private:
    auto text_extents(const X11Font &font, const std::string_view &text,
                      uint32_t len) noexcept -> etl::Result<Vec2D, X11FontError>;

  private:
    X11Draw(X11DisplaySharedResource dpy, GC graphics_ctx, Drawable drawable, Width width, Height height) noexcept;

  private:
    X11DisplaySharedResource _dpy;
    std::vector<X11Font> _fonts;
    GC _graphics_ctx;
    Drawable _drawable;
    Width _width;
    Height _height;
};

} // namespace tilebox::core
