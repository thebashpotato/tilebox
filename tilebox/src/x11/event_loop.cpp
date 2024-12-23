#include "tilebox/x11/event_loop.hpp"
#include "tilebox/x11/display.hpp"
#include "tilebox/x11/events.hpp"

#include <X11/Xlib.h>

#include <cstdint>
#include <fmt/base.h>
#include <utility>

namespace Tilebox
{

X11EventLoop::X11EventLoop(X11DisplaySharedResource dpy) noexcept : _dpy(std::move(dpy))
{
}

auto X11EventLoop::RegisterEventHandler(const X11EventType event_type, X11EventCallback callback) -> void
{
    if (!_event_handlers.contains(event_type))
    {
        if (auto [iter, was_inserted] = _event_handlers.emplace(event_type, std::move(callback)); !was_inserted)
        {
            // FIXME: Remove call to fmt::println, return Result<Void, SomeError> instead
            fmt::println("Could not insert event handler for X11 event type: {}",
                         static_cast<std::int32_t>(event_type));
        }
    }
}

auto X11EventLoop::Run(const bool &run_flag) const -> void
{
    XEvent event;
    while (run_flag && (XNextEvent(_dpy->Raw(), &event) == 0))
    {
        if (const X11EventType event_type = EventFromXlibEvent(event.type); _event_handlers.contains(event_type))
        {
            _event_handlers.at(event_type)(&event);
        }
    }
}

} // namespace Tilebox
