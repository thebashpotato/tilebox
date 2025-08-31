#include <cstdlib>
#include <iostream>
#include <optional>
#include <string_view>
#include <utility>

#include <tilebox/error.hpp>
#include <tilebox/geometry.hpp>
#include <tilebox/x11/display.hpp>
#include <tilebox/x11/events.hpp>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <etl.hpp>

#include "peripheral_logger.hpp"

using namespace Tilebox;
using namespace etl;

PeripheralLogger::PeripheralLogger(X11DisplaySharedResource &&dpy, Width &&app_width, Height &&app_height)
    : m_dpy(std::move(dpy)), m_win(m_dpy), m_event_loop(m_dpy), m_app_width(std::move(app_width)),
      m_app_height(std::move(app_height)), m_delete_window_msg(XInternAtom(m_dpy->Raw(), "WM_DELETE_WINDOW", False))
{
}

auto PeripheralLogger::Create(Width app_width, Height app_height) noexcept -> std::optional<PeripheralLogger>
{
    std::optional<PeripheralLogger> ret;
    if (auto dpy_opt = X11Display::Create(); dpy_opt.has_value())
    {
        ret.emplace(PeripheralLogger(std::move(dpy_opt.value()), std::move(app_width), std::move(app_height)));
    }

    return ret;
}

auto PeripheralLogger::Setup() noexcept -> Result<Void, Error>
{

    if (const Rect r(m_app_width, m_app_height); !m_win.Create(r))
    {
        return Result<Void, Error>(Error("Could not create the application window", RUNTIME_INFO));
    }

    if (!m_win.Map())
    {
        return Result<Void, Error>(Error("Could not map the window", RUNTIME_INFO));
    }

    // Set up the inputs we are going to listen to
    XSelectInput(m_dpy->Raw(), m_win.id(), KeyPressMask | ButtonPressMask | StructureNotifyMask);
    // Setup window messaging protocol for deleting a window.
    XSetWMProtocols(m_dpy->Raw(), m_win.id(), &m_delete_window_msg, 1);

    return Result<Void, Error>(Void());
}

auto PeripheralLogger::Run() noexcept -> Result<Void, Error>
{
    if (auto res = Setup(); res.is_err())
    {
        return res;
    }

    m_event_loop.RegisterEventHandler(X11EventType::X11ButtonPress, [&](const XEvent *const event) -> void {
        switch (event->xbutton.button)
        {
        case Button1:
            std::cout << "Left mouse button pressed!" << '\n';
            break;
        case Button2:
            std::cout << "Middle mouse button pressed!" << '\n';
            break;
        case Button3:
            std::cout << "Right mouse button pressed!" << '\n';
            break;
        case Button4:
            std::cout << "Scrolling up!" << '\n';
            break;
        case Button5:
            std::cout << "Scrolling down!" << '\n';
            break;
        default:
            std::cout << "Unkown event: " << event->xbutton.button << '\n';
        }
    });

    m_event_loop.RegisterEventHandler(X11EventType::X11KeyPress, [&](XEvent *const event) -> void {
        const KeySym key_sym = XLookupKeysym(&event->xkey, 0);

        if (const std::string_view key_name(XKeysymToString(key_sym)); key_sym == XK_Escape)
        {
            std::cout << "Escape key pressed .. Exiting" << '\n';
            m_run = false;
        }
        else
        {
            std::cout << "Key pressed: " << key_name << '\n';
        }
    });

    m_event_loop.RegisterEventHandler(X11EventType::X11ClientMessage, [&](const XEvent *const event) -> void {
        if (std::cmp_equal(event->xclient.data.l[0], m_delete_window_msg))
        {
            std::cout << "Window close event requested.. Exiting" << '\n';
            m_run = false;
        }
    });

    m_event_loop.Run(m_run);
    m_dpy->Sync();
    return Result<Void, Error>(Void());
}

auto main() -> int
{
    auto app_opt = PeripheralLogger::Create(Width(640), Height(480));
    if (!app_opt.has_value())
    {
        std::cout << "Failed to start the PeripheralLogger application" << '\n';
        return EXIT_FAILURE;
    }

    auto app = std::move(app_opt.value());

    if (auto res = app.Run(); res.is_err())
    {
        std::cout << "PeripheralLogger app failed to run: " << res.err()->info() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
