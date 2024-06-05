#include "peripheral_logger.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <cstdlib>
#include <fmt/core.h>
#include <optional>
#include <string>
#include <tilebox-core/geometry.hpp>
#include <tilebox-core/vendor/etl.hpp>
#include <tilebox-core/x11/display.hpp>
#include <tilebox-core/x11/events.hpp>
#include <utility>

using namespace tilebox::core;
using namespace etl;

PeripheralLogger::PeripheralLogger(X11DisplaySharedResource &&dpy, Width &&app_width, Height &&app_height)
    : _dpy(std::move(dpy)), _win(_dpy), _event_loop(_dpy), _aw(std::move(app_width)), _ah(std::move(app_height)),
      _delete_window_msg(XInternAtom(_dpy->raw(), "WM_DELETE_WINDOW", False))
{
}

auto PeripheralLogger::create(Width app_width, Height app_height) noexcept -> std::optional<PeripheralLogger>
{
    std::optional<PeripheralLogger> ret;
    auto dpy_opt = X11Display::create();
    if (dpy_opt.has_value())
    {
        ret.emplace(PeripheralLogger(std::move(dpy_opt.value()), std::move(app_width), std::move(app_height)));
    }

    return ret;
}

auto PeripheralLogger::_setup() noexcept -> Result<Void, Error>
{
    const Rect r(_aw, _ah);

    if (!_win.create(r))
    {
        return Result<Void, Error>(Error::create("Could not create the application window", RUNTIME_INFO));
    }

    if (!_win.map())
    {
        return Result<Void, Error>(Error::create("Could not map the window", RUNTIME_INFO));
    }

    // Set up the inputs we are going to listen to
    XSelectInput(_dpy->raw(), _win.id(), KeyPressMask | ButtonPressMask | StructureNotifyMask);
    // Setup window messaging protocol for deleting a window.
    XSetWMProtocols(_dpy->raw(), _win.id(), &_delete_window_msg, 1);

    return Result<Void, Error>(Void());
}

auto PeripheralLogger::run() noexcept -> Result<Void, Error>
{
    if (auto res = _setup(); res.is_err())
    {
        return res;
    }

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
        const std::string key_name(XKeysymToString(key_sym));
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

    _event_loop.register_event_handler(X11EventType::X11ClientMessage, [&](XEvent *event) -> void {
        if (static_cast<Atom>(event->xclient.data.l[0]) == _delete_window_msg)
        {
            fmt::println("Window close event requested.. Exiting");
            _run = false;
        }
    });

    _event_loop.start(_run);
    _dpy->sync();
    return Result<Void, Error>(Void());
}

auto main() -> int
{
    auto app_opt = PeripheralLogger::create(Width(640), Height(480));
    if (!app_opt.has_value())
    {
        fmt::println("Failed to start the PeripheralLogger application");
        return EXIT_FAILURE;
    }

    auto app = std::move(app_opt.value());

    if (auto res = app.run(); res.is_err())
    {
        fmt::println("PeripheralLogger app failed to run: {}", res.err().value().info());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
