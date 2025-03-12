#include "wm.hpp"
#include "log.hpp"
#include "xerror_handler.hpp"

#include <X11/X.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <etl.hpp>
#include <tilebox/config.hpp>
#include <tilebox/draw/colorscheme_config.hpp>
#include <tilebox/draw/draw.hpp>
#include <tilebox/draw/font.hpp>
#include <tilebox/error.hpp>
#include <tilebox/x11/display.hpp>

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <signal.h> // NOLINT
#include <string_view>
#include <sys/wait.h>
#include <utility>

using namespace etl;

namespace Tilebox::Twm
{

auto WindowManager::Create(std::string_view wm_name) noexcept -> Result<WindowManager, Error>
{
    Logging::Init(wm_name);

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
    return Result<WindowManager, Error>(WindowManager(std::move(shared_display), std::move(draw), wm_name));
}

auto WindowManager::Start() noexcept -> Result<Void, DynError>
{
    if (!m_running)
    {
        if (IsOtherWmRunning())
        {
            return Result<Void, DynError>(std::make_shared<Error>("Another Window Manager is already running"));
        }
        ProcessCleanup();

        if (auto res = Initialize(); res.is_err())
        {
            return res;
        }
        Log::Debug("Running lib{} version {}", Tilebox::kTileboxName, Tilebox::kTileboxVersion);
        m_running = true;
    }
    return Result<Void, DynError>(Void());
}

//////////////////////////////////////////
/// Private
//////////////////////////////////////////

WindowManager::WindowManager(X11DisplaySharedResource &&dpy, X11Draw &&draw, std::string_view name) noexcept
    : m_dpy(std::move(dpy)), m_draw(std::move(draw)), m_event_loop(m_dpy), m_atom_manager(m_dpy), m_name(name)
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
    struct sigaction sa{};

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

void WindowManager::AdvertiseAsEWMHCapable() noexcept
{
    const Atom utf8_string = m_atom_manager.GetUtf8Atom();
    const Atom net_supporting_wm_check = m_atom_manager.GetNetAtom(AtomManager::Net::SupportingWmCheck);
    const Atom net_wm_name = m_atom_manager.GetNetAtom(AtomManager::Net::WmName);
    const Atom net_supported = m_atom_manager.GetNetAtom(AtomManager::Net::Supported);
    const Atom net_client_list = m_atom_manager.GetNetAtom(AtomManager::Net::ClientList);
    const auto [net_atom_array_data, net_atom_array_size] = m_atom_manager.GetNetAtomArrayInfo();

    // Create a small, hidden window (1×1 pixels).
    // EWMH requires the window manager to have a special “check”
    // window to advertise itself. It is usually off-screen or of zero
    // size so the user never sees it.
    m_ewmh_check_win = XCreateSimpleWindow(m_dpy->Raw(), m_dpy->GetRootWindow(), 0, 0, 1, 1, 0, 0, 0);

    // One part of the EWMH spec says the “check” window should have a property pointing to itself
    // (_NET_SUPPORTING_WM_CHECK), which some applications use to confirm which window manager is running.
    XChangeProperty(m_dpy->Raw(), m_ewmh_check_win, net_supporting_wm_check, XA_WINDOW, 32, PropModeReplace,
                    reinterpret_cast<const unsigned char *>(&m_ewmh_check_win), 1);

    // Set the _NET_WM_NAME property of "check" window to "tbwm".
    // This is simply the human-readable name of the window manager, stored as UTF-8 text.
    XChangeProperty(m_dpy->Raw(), m_ewmh_check_win, net_wm_name, utf8_string, 8, PropModeReplace,
                    reinterpret_cast<const unsigned char *>(m_name.data()), static_cast<std::int32_t>(m_name.size()));

    // Set the _NET_SUPPORTING_WM_CHECK property on the root window to the same m_ewmh_check_win.
    // This tells other clients on the system that _NET_SUPPORTING_WM_CHECK (on the root window) points to tbwm’s hidden
    // check window, completing the link required by EWMH.
    XChangeProperty(m_dpy->Raw(), m_dpy->GetRootWindow(), net_supporting_wm_check, XA_WINDOW, 32, PropModeReplace,
                    reinterpret_cast<const unsigned char *>(&m_ewmh_check_win), 1);

    // Advertise which EWMH features tbwm supports
    XChangeProperty(m_dpy->Raw(), m_dpy->GetRootWindow(), net_supported, XA_ATOM, 32, PropModeReplace,
                    reinterpret_cast<const unsigned char *>(net_atom_array_data),
                    static_cast<std::int32_t>(net_atom_array_size));

    // Clear out the _NET_CLIENT_LIST property on the root window.
    // If there’s any leftover data from a previous window manager session, tbwm removes it. This property is later
    // updated dynamically as windows open or close.
    XDeleteProperty(m_dpy->Raw(), m_dpy->GetRootWindow(), net_client_list);
}

auto WindowManager::Initialize() noexcept -> Result<Void, DynError>
{
    Log::Info("Initializing {}", m_name);

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

    // Initialize colorschemes

    // TODO: Colorscheme config code will be moved into the user config code
    const ColorSchemeConfig primary = ColorSchemeConfig::Build(ColorSchemeKind::Primary)
                                          .foreground("#dde1e6")
                                          .background("#1a1b26")
                                          .border("#393939");

    const ColorSchemeConfig secondary = ColorSchemeConfig::Build(ColorSchemeKind::Secondary)
                                            .foreground("#24283b")
                                            .background("#7aa2f7")
                                            .border("#7aa2f7");

    if (auto res = m_draw.InitColorScheme(primary); res.is_err())
    {
        return Result<Void, DynError>(std::make_shared<X11ColorError>(std::move(res.err().value())));
    }

    if (auto res = m_draw.InitColorScheme(secondary); res.is_err())
    {
        return Result<Void, DynError>(std::make_shared<X11ColorError>(std::move(res.err().value())));
    }

    AdvertiseAsEWMHCapable();

    return Result<Void, DynError>(Void());
}

} // namespace Tilebox::Twm
