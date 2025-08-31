#pragma once

// #include <unordered_map>
#include <vector>

#include <tilebox/x11/display.hpp>

#include "font.hpp"

namespace Tilebox::Ui
{

class X11Fontset
{
  public:
    ~X11Fontset() = default;
    X11Fontset(X11Fontset &&) = default;
    X11Fontset(const X11Fontset &) = delete;
    auto operator=(X11Fontset &&) -> X11Fontset & = default;
    auto operator=(const X11Fontset &) -> X11Fontset & = delete;

  private:
    X11DisplaySharedResource m_dpy;
    X11Font m_primary;
    std::vector<X11Font> m_fallback;
};

} // namespace Tilebox::Ui
