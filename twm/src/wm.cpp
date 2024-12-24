#include "wm.hpp"
#include "log.hpp"
#include "xerror_handler.hpp"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <etl.hpp>
#include <tilebox/config.hpp>
#include <tilebox/error.hpp>
#include <tilebox/x11/display.hpp>

#include <utility>

namespace Tilebox::Twm
{

auto WindowManager::Create() noexcept -> etl::Result<WindowManager, Error>
{
    Logging::Init();

    if (auto x11_display_opt = X11Display::Create(); x11_display_opt.has_value())
    {
        X11DisplaySharedResource shared_display = std::move(x11_display_opt.value());
        return etl::Result<WindowManager, Error>(WindowManager(std::move(shared_display)));
    }
    return etl::Result<WindowManager, Error>(Error("Failed to create X11 display"));
}

auto WindowManager::Start() const noexcept -> etl::Result<etl::Void, etl::DynError>
{
    // This will call std::exit if another wm is running,
    // so call it now since the only heap memory we have is X11DisplaySharedResource
    CheckOtherWm();

    Log::Debug("Running lib{} version {}", Tilebox::kTileboxName, Tilebox::kTileboxVersion);
    Log::Info("Starting twm");

    return etl::Result<etl::Void, etl::DynError>(etl::Void());
}

//////////////////////////////////////////
/// Private
//////////////////////////////////////////

WindowManager::WindowManager(X11DisplaySharedResource &&dpy) noexcept : m_dpy(std::move(dpy)), m_event_loop(m_dpy)
{
}

void WindowManager::CheckOtherWm() const noexcept
{
    g_error_handler_callback = XSetErrorHandler(WmStartupErrorHandler);

    // This causes an error if some other window manager is currently running
    XSelectInput(m_dpy->Raw(), m_dpy->GetDefaultRootWindow(), SubstructureRedirectMask);
    m_dpy->Sync(false);

    // If we make it here no startup error was triggered, swap out startup error handler with the runtime error handler
    XSetErrorHandler(WmRuntimeErrorHandler);
    m_dpy->Sync(false);
}

} // namespace Tilebox::Twm
