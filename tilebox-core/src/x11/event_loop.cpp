#include "tilebox-core/x11/event_loop.hpp"
#include "tilebox-core/x11/display.hpp"
#include "tilebox-core/x11/events.hpp"
#include <X11/Xlib.h>
#include <cstdint>
#include <fmt/core.h>
#include <utility>

namespace tilebox::core::x
{

X11EventLoop::X11EventLoop(X11Display &display) noexcept : _display(display)
{
}

auto X11EventLoop::register_event_handler(const X11EventType event_type, X11EventCallback callback) -> void
{
    if (!this->_event_handlers.contains(event_type))
    {
        auto insertion_pair = this->_event_handlers.emplace(event_type, std::move(callback));
        if (!insertion_pair.second)
        {
            fmt::println("Could not insert event handler for X11 event type: {}",
                         static_cast<std::int32_t>(event_type));
        }
    }
}

auto X11EventLoop::start(bool &dispatch) -> void
{
    XEvent event;
    while (dispatch && (XNextEvent(_display.get_raw(), &event) == 0))
    {
        const X11EventType event_type = tilebox_event_from_xlib_event(event.type);
        if (this->_event_handlers.contains(event_type))
        {
            auto callback = this->_event_handlers.at(event_type);
            callback(&event);
        }
    }
}

} // namespace tilebox::core::x
