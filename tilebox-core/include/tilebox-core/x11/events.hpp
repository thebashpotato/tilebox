#pragma once

#include <cstdint>
#include <tilebox-core/utils/attributes.hpp>

namespace tilebox::core::x
{
////////////////////////////////////
// X11 Event Mask Definitions
////////////////////////////////////

/// @brief Input event-masks which map directly to Xlib event masks
///
/// @note Used as event-mask window attribute and as arguments to Grab requests. Not to be confused with event names.
enum class TILEBOX_EXPORT X11InputEventMask : std::int64_t
{
    X11NoEventMask = 0,
    X11KeyPressMask = 1 << 0,
    X11KeyReleaseMask = 1 << 1,
    X11ButtonPressMask = 1 << 2,
    X11ButtonReleaseMask = 1 << 3,
    X11EnterWindowMask = 1 << 4,
    X11LeaveWindowMask = 1 << 5,
    X11PointerMotionMask = 1 << 6,
    X11PointerMotionHintMask = 1 << 7,
    X11Button1MotionMask = 1 << 8,
    X11Button2MotionMask = 1 << 9,
    X11Button3MotionMask = 1 << 10,
    X11Button4MotionMask = 1 << 11,
    X11Button5MotionMask = 1 << 12,
    X11ButtonMotionMask = 1 << 13,
    X11KeymapStateMask = 1 << 14,
    X11ExposureMask = 1 << 15,
    X11VisibilityChangeMask = 1 << 16,
    X11StructureNotifyMask = 1 << 17,
    X11ResizeRedirectMask = 1 << 18,
    X11SubstructureNotifyMask = 1 << 19,
    X11SubstructureRedirectMask = 1 << 20,
    X11FocusChangeMask = 1 << 21,
    X11PropertyChangeMask = 1 << 22,
    X11ColormapChangeMask = 1 << 23,
    X11OwnerGrabButtonMask = 1 << 24,
};

/// @brief Converts the Xlib input event mask to a tilebox input event mask.
TILEBOX_EXPORT [[nodiscard]] auto tilebox_event_mask_from_xlib_eventmask(const std::int64_t event_mask)
    -> X11InputEventMask;

/// @brief Conversts a tilebox input event mask to a Xlib input event mask
TILEBOX_EXPORT [[nodiscard]] auto tilebox_event_mask_to_xlib_eventmask(const X11InputEventMask event_mask)
    -> std::int64_t;

////////////////////////////////////
// X11 Event Types/Names
////////////////////////////////////

/// @brief Event names which map directly to xlib events
///
/// @note These are used in the "type" field in the xlib XEvent structures.
/// @note They start from 2 because 0 and 1 are reserved in the X11 protocol for errors and replies.
enum class TILEBOX_EXPORT X11EventType : std::int32_t
{
    X11KeyPress = 2,
    X11KeyRelease = 3,
    X11ButtonPress = 4,
    X11ButtonRelease = 5,
    X11MotionNotify = 6,
    X11EnterNotify = 7,
    X11LeaveNotify = 8,
    X11FocusIn = 9,
    X11FocusOut = 10,
    X11KeymapNotify = 11,
    X11Expose = 12,
    X11GraphicsExpose = 13,
    X11NoExpose = 14,
    X11VisibilityNotify = 15,
    X11CreateNotify = 16,
    X11DestroyNotify = 17,
    X11UnmapNotify = 18,
    X11MapNotify = 19,
    X11MapRequest = 20,
    X11ReparentNotify = 21,
    X11ConfigureNotify = 22,
    X11ConfigureRequest = 23,
    X11GravityNotify = 24,
    X11ResizeRequest = 25,
    X11CirculateNotify = 26,
    X11CirculateRequest = 27,
    X11PropertyNotify = 28,
    X11SelectionClear = 29,
    X11SelectionRequest = 30,
    X11SelectionNotify = 31,
    X11ColormapNotify = 32,
    X11ClientMessage = 33,
    X11MappingNotify = 34,
    X11GenericEvent = 35,
    X11LASTEvent = 36,
};

TILEBOX_EXPORT [[nodiscard]] auto tilebox_event_from_xlib_event(const std::int32_t event_type) noexcept -> X11EventType;

TILEBOX_EXPORT [[nodiscard]] auto tilebox_event_to_xlib_event(const X11EventType event_type) noexcept -> std::int32_t;

} // namespace tilebox::core::x
