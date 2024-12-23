#pragma once

#include <tilebox/error.hpp>
#include <tilebox/geometry.hpp>
#include <tilebox/x11/display.hpp>
#include <tilebox/x11/event_loop.hpp>
#include <tilebox/x11/events.hpp>
#include <tilebox/x11/window.hpp>

#include <X11/X.h>
#include <etl.hpp>

#include <optional>

class PeripheralLogger
{
  public:
    [[nodiscard]] static auto Create(Tilebox::Width app_width,
                                     Tilebox::Height app_height) noexcept -> std::optional<PeripheralLogger>;

    [[nodiscard]] auto Run() noexcept -> etl::Result<etl::Void, Tilebox::Error>;

  private:
    PeripheralLogger(Tilebox::X11DisplaySharedResource &&dpy, Tilebox::Width &&app_width, Tilebox::Height &&app_height);

  private:
    [[nodiscard]] auto Setup() noexcept -> etl::Result<etl::Void, Tilebox::Error>;

  private:
    Tilebox::X11DisplaySharedResource m_dpy;
    Tilebox::X11Window m_win;
    Tilebox::X11EventLoop m_event_loop;
    Tilebox::Width m_app_width;
    Tilebox::Height m_app_height;
    Atom m_delete_window_msg;
    bool m_run{true};
};
