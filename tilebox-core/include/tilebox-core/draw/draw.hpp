#pragma once

#include "tilebox-core/draw/font.hpp"
#include "tilebox-core/geometry.hpp"
#include "tilebox-core/utils/attributes.hpp"
#include "tilebox-core/x11/display.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <vector>

namespace tilebox::core
{

/// @brief The main class of the drawing library, it handles drawing text, and colors to rectangles.
class TILEBOX_EXPORT X11Draw
{
  public:
    X11Draw(X11DisplaySharedResource dpy, Width width, Height height) noexcept;
    ~X11Draw() noexcept;
    X11Draw(X11Draw &&rhs) noexcept;
    X11Draw(const X11Draw &rhs) noexcept = delete;

  public:
    auto operator=(X11Draw &&rhs) noexcept -> X11Draw &;
    auto operator=(const X11Draw &rhs) noexcept -> X11Draw & = delete;

  public:
    auto resize(const Width &width, const Height &height) noexcept -> void;

  private:
    X11DisplaySharedResource _dpy;
    std::vector<X11Font> _fonts;
    Drawable _drawable;
    GC _graphics_ctx;
    Width _width;
    Height _height;
};

} // namespace tilebox::core
