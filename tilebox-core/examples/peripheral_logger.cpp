#include "peripheral_logger.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <cstdlib>
#include <fmt/core.h>
#include <optional>
#include <stdexcept>
#include <string>
#include <tilebox-core/geometry.hpp>
#include <tilebox-core/x11/events.hpp>
#include <utility>

using namespace tilebox::core;
using namespace tilebox::core::x;

PeripheralLogger::PeripheralLogger(Width &&app_width, Height &&app_height)
    : _dpy(std::nullopt), _win(_dpy), _event_loop(_dpy), _aw(std::move(app_width)), _ah(std::move(app_height))
{
    if (_dpy.is_connected())
    {
        _run = true;
    }
}

auto PeripheralLogger::_setup() -> void
{
    const Rect r(_aw, _ah);

    if (!_win.create_window(r))
    {
        throw std::runtime_error("Could not create the application window");
    }

    if (!_win.map())
    {
        throw std::runtime_error("Could not map the window");
    }

    XSelectInput(_dpy.raw(), _win.id(), KeyPressMask | ButtonPressMask);
}

auto PeripheralLogger::run() -> void
{
    _setup();

    _event_loop.register_event_handler(X11EventType::X11ButtonPress, [&](XEvent *event) -> void {
        switch (event->xbutton.button)
        {
        case Button1:
            fmt::println("Left mouse button pressed!");
            break;
        case Button2:
            fmt::println("Middle mouse button pressed!");
            break;
        case Button3:
            fmt::println("Right mouse button pressed!");
            break;
        case Button4:
            fmt::println("Scrolling up!");
            break;
        case Button5:
            fmt::println("Scrolling down!");
            break;
        default:
            fmt::println("Unknown event: {}", event->xbutton.button);
        }
    });

    _event_loop.register_event_handler(X11EventType::X11KeyPress, [&](XEvent *event) -> void {
        const KeySym key_sym = XLookupKeysym(&event->xkey, 0);
        std::string key_name(XKeysymToString(key_sym));
        if (key_sym == XK_Escape)
        {
            fmt::println("Escape key pressed.. Exiting");
            _run = false;
        }
        else
        {
            fmt::println("Key pressed: {}", key_name);
        }
    });

    _event_loop.start(_run);
}

auto main() -> int
{
    auto app = PeripheralLogger(Width(640), Height(480));

    try
    {
        app.run();
    }
    catch (const std::runtime_error &error)
    {
        fmt::println("{}", error.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
