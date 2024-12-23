#pragma once

#include "tilebox/utils/attributes.hpp"
#include "tilebox/x11/display.hpp"
#include "tilebox/x11/events.hpp"

#include <X11/Xlib.h>

#include <functional>
#include <unordered_map>

namespace Tilebox
{

using X11EventCallback = std::function<void(XEvent *event)>;

/// @brief Encapsulated, flexible abstraction of the X11 event loop
/// with the ability to register event handlers for specific events.
class TILEBOX_EXPORT X11EventLoop
{
  public:
    explicit X11EventLoop(X11DisplaySharedResource dpy) noexcept;
    ~X11EventLoop() = default;

    X11EventLoop(const X11EventLoop &other) noexcept = default;
    auto operator=(const X11EventLoop &other) noexcept -> X11EventLoop & = default;
    X11EventLoop(X11EventLoop &&other) noexcept = default;
    auto operator=(X11EventLoop &&other) noexcept -> X11EventLoop & = default;

  public:
    /// @brief Register a handler callback function for a specific event.
    ///
    /// @param event_type The event type to listen for
    /// @param callback The callback function to call when the event is received
    void RegisterEventHandler(X11EventType event_type, X11EventCallback callback);

    /// @brief Starts the event loop
    /// @details This function will block until an event is received, the user needs
    /// should do substantial set up before calling this function.
    ///
    /// @param run_flag A flag to keep the loop (program) running, change too false to shut down the event loop.
    void Run(const bool &run_flag) const;

  private:
    X11DisplaySharedResource _dpy;
    std::unordered_map<X11EventType, X11EventCallback> _event_handlers;
};

} // namespace Tilebox
