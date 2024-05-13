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
class TILEBOX_EXPORT X11EventLoop
{
  private:
    X11Display _dpy;
    std::unordered_map<X11EventType, X11EventCallback> _event_handlers;

  public:
    explicit X11EventLoop(X11Display &dpy) noexcept;
    virtual ~X11EventLoop() = default;

    X11EventLoop(const X11EventLoop &other) noexcept = default;
    auto operator=(const X11EventLoop &other) noexcept -> X11EventLoop & = default;
    X11EventLoop(X11EventLoop &&other) noexcept = default;
    auto operator=(X11EventLoop &&other) noexcept -> X11EventLoop & = default;

  public:
    /// @brief Register a handler callback function for a specific event.
    ///
    /// @param `event_type` The event type to listen for
    /// @param `handler` The callback function to call when the event is received
    auto register_event_handler(const X11EventType event_type, X11EventCallback callback) -> void;

    /// @brief Starts the event loop
    /// @details This function will block until an event is received, the user needs
    /// should do subtantial set up before calling this function.
    ///
    /// @param `dispatch` A flag to keep the loop running, change to false to shutdown the event loop
    auto start(bool &dispatch) -> void;
};

} // namespace tilebox::core::x
