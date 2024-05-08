#pragma once

#include "tilebox-core/utils/attributes.hpp"
#include "tilebox-core/x11/display.hpp"
#include "tilebox-core/x11/events.hpp"
#include <X11/Xlib.h>
#include <functional>
#include <unordered_map>

namespace tilebox::core::x
{
using X11EventCallback = std::function<void(XEvent *event)>;

/// @brief Encapsulated, flexible abstraction of the X11 event loop
/// with the ability to register event handlers for specific events.
class TILEBOX_EXPORT X11EventDispatcher
{
  private:
    X11Display _display;
    std::unordered_map<X11EventType, X11EventCallback> _event_handlers;

  public:
    explicit X11EventDispatcher(X11Display &display) noexcept;
    virtual ~X11EventDispatcher() = default;

    X11EventDispatcher(const X11EventDispatcher &other) noexcept = default;
    auto operator=(const X11EventDispatcher &other) noexcept -> X11EventDispatcher & = default;
    X11EventDispatcher(X11EventDispatcher &&other) noexcept = default;
    auto operator=(X11EventDispatcher &&other) noexcept -> X11EventDispatcher & = default;

  public:
    /// @brief Adds an event handler to the dispatcher
    ///
    /// @param `event_type` The event type to listen for
    /// @param `handler` The callback function to call when the event is received
    auto add_event_handler(const X11EventType event_type, X11EventCallback callback) -> void;

    /// @brief Dispatches events to the registered handlers
    /// @details This function will block until an event is received, the user needs
    /// should do subtantial set up before calling this function.
    ///
    /// @param `is_running` A flag to indicate if the event loop should continue during periods of event starvation.
    auto dispatch_events(bool &is_running) -> void;
};

} // namespace tilebox::core::x
