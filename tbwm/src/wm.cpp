#include "wm.hpp"
#include "log.hpp"
#include "xerror_handler.hpp"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <etl.hpp>
#include <tilebox/config.hpp>
#include <tilebox/draw/draw.hpp>
#include <tilebox/draw/font.hpp>
#include <tilebox/error.hpp>
#include <tilebox/x11/display.hpp>

#include <cstdlib>
#include <memory>
#include <signal.h> // NOLINT
#include <sys/wait.h>
#include <utility>

using namespace etl;

namespace Tilebox::Twm
{

auto WindowManager::Create() noexcept -> Result<WindowManager, Error>
{
    Logging::Init();

    auto x11_display_opt = X11Display::Create();
    if (!x11_display_opt.has_value())
    {
        return Result<WindowManager, Error>({"Failed to create X11 Display", RUNTIME_INFO});
    }

    X11DisplaySharedResource shared_display = std::move(x11_display_opt.value());

    auto draw_res = X11Draw::Create(shared_display, shared_display->ScreenWidth(), shared_display->ScreenHeight());
    if (draw_res.is_err())
    {
        return Result<WindowManager, Error>(std::move(draw_res.err().value()));
    }

    X11Draw draw = std::move(draw_res.ok().value());
    return Result<WindowManager, Error>(WindowManager(std::move(shared_display), std::move(draw)));
}

auto WindowManager::Start() noexcept -> Result<Void, DynError>
{
    if (IsOtherWmRunning())
    {
        return Result<Void, DynError>(std::make_shared<Error>("Another Window Manager is already running"));
    }
    ProcessCleanup();
    Log::Debug("Running lib{} version {}", Tilebox::kTileboxName, Tilebox::kTileboxVersion);
    Log::Info("Starting tbwm");

    if (const auto res = Initialize(); res.is_err())
    {
        return Result<Void, DynError>(std::move(res.err().value()));
    }

    return Result<Void, DynError>(Void());
}

//////////////////////////////////////////
/// Private
//////////////////////////////////////////

WindowManager::WindowManager(X11DisplaySharedResource &&dpy, X11Draw &&draw) noexcept
    : m_dpy(std::move(dpy)), m_draw(std::move(draw)), m_event_loop(m_dpy)
{
}

auto WindowManager::IsOtherWmRunning() const noexcept -> bool
{
    // Register startup error handler callback with the X server
    g_error_handler_callback = XSetErrorHandler(WmStartupErrorHandler);

    // This causes an error if some other window manager is currently running.
    // This works because only one client at a time can select SubstructureRedirectMask on the Root window.
    //
    // If another which will trigger the WmStartupErrorHandler callback, which will set
    // g_another_window_manager_is_running to true.
    XSelectInput(m_dpy->Raw(), m_dpy->GetDefaultRootWindow(), SubstructureRedirectMask);
    m_dpy->Sync();

    if (g_another_window_manager_is_running)
    {
        return true;
    }

    // If we make it here no startup error was triggered, swap out startup error handler with the runtime error handler
    XSetErrorHandler(WmRuntimeErrorHandler);
    m_dpy->Sync();
    return false;
}

void WindowManager::ProcessCleanup() noexcept
{
    struct sigaction sa
    {
    };

    // Do not transform children into zombies when they terminate,
    // this way child processes disappear automatically and no zombie is left behind for
    // the parent to clean up.
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NOCLDSTOP | SA_NOCLDWAIT | SA_RESTART;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGCHLD, &sa, nullptr);

    while (waitpid(-1, nullptr, WNOHANG) > 0) // NOLINT
    {
    }
}

auto WindowManager::Initialize() noexcept -> Result<Void, DynError>
{
    // Initialize all supported atoms
    m_atom_manager.Init(m_dpy);

    // Initialize all cursors
    if (auto res = m_draw.InitCursorAll(); res.is_err())
    {
        return Result<Void, DynError>(std::make_shared<X11CursorError>(std::move(res.err().value())));
    }

    // Initialize a font
    if (auto res = m_draw.InitFont("monospace:size=14", X11Font::Type::Primary); res.is_err())
    {
        return Result<Void, DynError>(std::make_shared<X11FontError>(std::move(res.err().value())));
    }


    return Result<Void, DynError>(Void());
}

} // namespace Tilebox::Twm
