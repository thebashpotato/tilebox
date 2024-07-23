#pragma once

#include <X11/X.h>
#include <optional>
#include <tilebox-core/geometry.hpp>
#include <tilebox-core/x11/display.hpp>
#include <tilebox-core/x11/event_loop.hpp>
#include <tilebox-core/x11/events.hpp>
#include <tilebox-core/x11/window.hpp>

class PeripheralLogger
{
  private:
    tilebox::core::X11DisplaySharedResource _dpy;
    tilebox::core::X11Window _win;
    tilebox::core::X11EventLoop _event_loop;
    tilebox::core::Width _aw;
    tilebox::core::Height _ah;
    Atom _delete_window_msg;

    bool _run{true};

  private:
    PeripheralLogger(tilebox::core::X11DisplaySharedResource &&dpy, tilebox::core::Width &&app_width,
                     tilebox::core::Height &&app_height);

  private:
    [[nodiscard]] auto _setup() noexcept -> etl::Result<etl::Void, etl::Error>;

  public:
    [[nodiscard]] static auto create(tilebox::core::Width app_width,
                                     tilebox::core::Height app_height) noexcept -> std::optional<PeripheralLogger>;

    [[nodiscard]] auto run() noexcept -> etl::Result<etl::Void, etl::Error>;
};
