#include "tilebox-core/x11/display.hpp"
#include "tilebox-core/x11/even_dispatcher.hpp"
#include "tilebox-core/x11/events.hpp"
#include <X11/Xlib.h>
#include <utility>

namespace tilebox::core::x
{

X11EventDispatcher::X11EventDispatcher(X11Display &display) noexcept : _display(display)
{
}

auto X11EventDispatcher::add_event_handler(const X11EventType event_type, X11EventCallback callback) -> void
{
    _event_handlers[event_type] = std::move(callback);
}

auto X11EventDispatcher::dispatch_events(bool &is_running) -> void
{
    XEvent event;
    while (is_running && (XNextEvent(_display.get_raw(), &event) == 0))
    {
        const X11EventType x11event = tilebox_event_from_xlib_event(event.type);
        if (_event_handlers.contains(x11event))
        {
            _event_handlers[x11event](&event);
        }
    }
}

} // namespace tilebox::core::x