#pragma once

#include <tilebox-core/geometry.hpp>
#include <tilebox-core/x11/display.hpp>
#include <tilebox-core/x11/event_loop.hpp>
#include <tilebox-core/x11/events.hpp>
#include <tilebox-core/x11/window.hpp>

class PeripheralLogger
{
  private:
    tilebox::core::x::X11Display _dpy;
    tilebox::core::x::X11Window _win;
    tilebox::core::x::X11EventLoop _event_loop;
    tilebox::core::Width _aw;
    tilebox::core::Height _ah;
    bool _run{false};

  public:
    PeripheralLogger(tilebox::core::Width &&app_width, tilebox::core::Height &&app_height);

  private:
    auto _setup() -> void;

  public:
    auto run() -> void;
};
