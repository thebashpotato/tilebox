#pragma once

#include <etl.hpp>
#include <tilebox/error.hpp>
#include <tilebox/x11/display.hpp>
#include <tilebox/x11/event_loop.hpp>

namespace Tilebox::Twm
{

class WindowManager
{
  public:
    [[nodiscard]] static auto Create() noexcept -> etl::Result<WindowManager, Error>;
    [[nodiscard]] auto Start() const noexcept -> etl::Result<etl::Void, etl::DynError>;

  private:
    void CheckOtherWm() const noexcept;

  private:
    explicit WindowManager(X11DisplaySharedResource &&dpy) noexcept;

  private:
    X11DisplaySharedResource m_dpy;
    X11EventLoop m_event_loop;
    bool m_run{true};
};

} // namespace Tilebox::Twm
