#include <tilebox/x11/events.hpp>

#include <X11/X.h>

#include <cstdint>

namespace Tilebox
{

///////////////////////////////////////////
// X11 Input Event Mask Conversion Helpers
///////////////////////////////////////////

auto EventMaskFromXlibEventMask(const std::int64_t event_mask) -> X11InputEventMask
{
    switch (event_mask)
    {
    case NoEventMask:
        return X11InputEventMask::X11NoEventMask;
    case KeyPressMask:
        return X11InputEventMask::X11KeyPressMask;
    case KeyReleaseMask:
        return X11InputEventMask::X11KeyReleaseMask;
    case ButtonPressMask:
        return X11InputEventMask::X11ButtonPressMask;
    case ButtonReleaseMask:
        return X11InputEventMask::X11ButtonReleaseMask;
    case EnterWindowMask:
        return X11InputEventMask::X11EnterWindowMask;
    case LeaveWindowMask:
        return X11InputEventMask::X11LeaveWindowMask;
    case PointerMotionMask:
        return X11InputEventMask::X11PointerMotionMask;
    case PointerMotionHintMask:
        return X11InputEventMask::X11PointerMotionHintMask;
    case Button1MotionMask:
        return X11InputEventMask::X11Button1MotionMask;
    case Button2MotionMask:
        return X11InputEventMask::X11Button2MotionMask;
    case Button3MotionMask:
        return X11InputEventMask::X11Button3MotionMask;
    case Button4MotionMask:
        return X11InputEventMask::X11Button4MotionMask;
    case Button5MotionMask:
        return X11InputEventMask::X11Button5MotionMask;
    case ButtonMotionMask:
        return X11InputEventMask::X11ButtonMotionMask;
    case KeymapStateMask:
        return X11InputEventMask::X11KeymapStateMask;
    case ExposureMask:
        return X11InputEventMask::X11ExposureMask;
    case VisibilityChangeMask:
        return X11InputEventMask::X11VisibilityChangeMask;
    case StructureNotifyMask:
        return X11InputEventMask::X11StructureNotifyMask;
    case ResizeRedirectMask:
        return X11InputEventMask::X11ResizeRedirectMask;
    case SubstructureNotifyMask:
        return X11InputEventMask::X11SubstructureNotifyMask;
    case SubstructureRedirectMask:
        return X11InputEventMask::X11SubstructureRedirectMask;
    case FocusChangeMask:
        return X11InputEventMask::X11FocusChangeMask;
    case PropertyChangeMask:
        return X11InputEventMask::X11PropertyChangeMask;
    case ColormapChangeMask:
        return X11InputEventMask::X11ColormapChangeMask;
    default:
        return X11InputEventMask::X11OwnerGrabButtonMask;
    }
}

auto EventMaskToXlibEventMask(const X11InputEventMask event_mask) -> std::int64_t
{
    switch (event_mask)
    {
    case X11InputEventMask::X11NoEventMask:
        return NoEventMask;
    case X11InputEventMask::X11KeyPressMask:
        return KeyPressMask;
    case X11InputEventMask::X11KeyReleaseMask:
        return KeyReleaseMask;
    case X11InputEventMask::X11ButtonPressMask:
        return ButtonPressMask;
    case X11InputEventMask::X11ButtonReleaseMask:
        return ButtonReleaseMask;
    case X11InputEventMask::X11EnterWindowMask:
        return EnterWindowMask;
    case X11InputEventMask::X11LeaveWindowMask:
        return LeaveWindowMask;
    case X11InputEventMask::X11PointerMotionMask:
        return PointerMotionMask;
    case X11InputEventMask::X11PointerMotionHintMask:
        return PointerMotionHintMask;
    case X11InputEventMask::X11Button1MotionMask:
        return Button1MotionMask;
    case X11InputEventMask::X11Button2MotionMask:
        return Button2MotionMask;
    case X11InputEventMask::X11Button3MotionMask:
        return Button3MotionMask;
    case X11InputEventMask::X11Button4MotionMask:
        return Button4MotionMask;
    case X11InputEventMask::X11Button5MotionMask:
        return Button5MotionMask;
    case X11InputEventMask::X11ButtonMotionMask:
        return ButtonMotionMask;
    case X11InputEventMask::X11KeymapStateMask:
        return KeymapStateMask;
    case X11InputEventMask::X11ExposureMask:
        return ExposureMask;
    case X11InputEventMask::X11VisibilityChangeMask:
        return VisibilityChangeMask;
    case X11InputEventMask::X11StructureNotifyMask:
        return StructureNotifyMask;
    case X11InputEventMask::X11ResizeRedirectMask:
        return ResizeRedirectMask;
    case X11InputEventMask::X11SubstructureNotifyMask:
        return SubstructureNotifyMask;
    case X11InputEventMask::X11SubstructureRedirectMask:
        return SubstructureRedirectMask;
    case X11InputEventMask::X11FocusChangeMask:
        return FocusChangeMask;
    case X11InputEventMask::X11PropertyChangeMask:
        return PropertyChangeMask;
    case X11InputEventMask::X11ColormapChangeMask:
        return ColormapChangeMask;
    case X11InputEventMask::X11OwnerGrabButtonMask:
        return OwnerGrabButtonMask;
    }

    // Stop the end of control flow warning
    return NoEventMask;
}
/////////////////////////////////////
// X11 Event Type Conversion Helpers
/////////////////////////////////////

auto EventFromXlibEvent(const std::int32_t event_type) noexcept -> X11EventType
{
    switch (event_type)
    {
    case KeyPress:
        return X11EventType::X11KeyPress;
    case KeyRelease:
        return X11EventType::X11KeyRelease;
    case ButtonPress:
        return X11EventType::X11ButtonPress;
    case ButtonRelease:
        return X11EventType::X11ButtonRelease;
    case MotionNotify:
        return X11EventType::X11MotionNotify;
    case EnterNotify:
        return X11EventType::X11EnterNotify;
    case LeaveNotify:
        return X11EventType::X11LeaveNotify;
    case FocusIn:
        return X11EventType::X11FocusIn;
    case FocusOut:
        return X11EventType::X11FocusOut;
    case KeymapNotify:
        return X11EventType::X11KeymapNotify;
    case Expose:
        return X11EventType::X11Expose;
    case GraphicsExpose:
        return X11EventType::X11GraphicsExpose;
    case NoExpose:
        return X11EventType::X11NoExpose;
    case VisibilityNotify:
        return X11EventType::X11VisibilityNotify;
    case CreateNotify:
        return X11EventType::X11CreateNotify;
    case DestroyNotify:
        return X11EventType::X11DestroyNotify;
    case UnmapNotify:
        return X11EventType::X11UnmapNotify;
    case MapNotify:
        return X11EventType::X11MapNotify;
    case MapRequest:
        return X11EventType::X11MapRequest;
    case ReparentNotify:
        return X11EventType::X11ReparentNotify;
    case ConfigureNotify:
        return X11EventType::X11ConfigureNotify;
    case ConfigureRequest:
        return X11EventType::X11ConfigureRequest;
    case GravityNotify:
        return X11EventType::X11GravityNotify;
    case ResizeRequest:
        return X11EventType::X11ResizeRequest;
    case CirculateNotify:
        return X11EventType::X11CirculateNotify;
    case CirculateRequest:
        return X11EventType::X11CirculateRequest;
    case PropertyNotify:
        return X11EventType::X11PropertyNotify;
    case SelectionClear:
        return X11EventType::X11SelectionClear;
    case SelectionRequest:
        return X11EventType::X11SelectionRequest;
    case SelectionNotify:
        return X11EventType::X11SelectionNotify;
    case ColormapNotify:
        return X11EventType::X11ColormapNotify;
    case ClientMessage:
        return X11EventType::X11ClientMessage;
    case MappingNotify:
        return X11EventType::X11MappingNotify;
    case GenericEvent:
        return X11EventType::X11GenericEvent;
    default:
        return X11EventType::X11LASTEvent;
    }
}

auto EventToXlibEvent(const X11EventType event_type) noexcept -> std::int32_t
{
    switch (event_type)
    {
    case X11EventType::X11KeyPress:
        return KeyPress;
    case X11EventType::X11KeyRelease:
        return KeyRelease;
    case X11EventType::X11ButtonPress:
        return ButtonPress;
    case X11EventType::X11ButtonRelease:
        return ButtonRelease;
    case X11EventType::X11MotionNotify:
        return MotionNotify;
    case X11EventType::X11EnterNotify:
        return EnterNotify;
    case X11EventType::X11LeaveNotify:
        return LeaveNotify;
    case X11EventType::X11FocusIn:
        return FocusIn;
    case X11EventType::X11FocusOut:
        return FocusOut;
    case X11EventType::X11KeymapNotify:
        return KeymapNotify;
    case X11EventType::X11Expose:
        return Expose;
    case X11EventType::X11GraphicsExpose:
        return GraphicsExpose;
    case X11EventType::X11NoExpose:
        return NoExpose;
    case X11EventType::X11VisibilityNotify:
        return VisibilityNotify;
    case X11EventType::X11CreateNotify:
        return CreateNotify;
    case X11EventType::X11DestroyNotify:
        return DestroyNotify;
    case X11EventType::X11UnmapNotify:
        return UnmapNotify;
    case X11EventType::X11MapNotify:
        return MapNotify;
    case X11EventType::X11MapRequest:
        return MapRequest;
    case X11EventType::X11ReparentNotify:
        return ReparentNotify;
    case X11EventType::X11ConfigureNotify:
        return ConfigureNotify;
    case X11EventType::X11ConfigureRequest:
        return ConfigureRequest;
    case X11EventType::X11GravityNotify:
        return GravityNotify;
    case X11EventType::X11ResizeRequest:
        return ResizeRequest;
    case X11EventType::X11CirculateNotify:
        return CirculateNotify;
    case X11EventType::X11CirculateRequest:
        return CirculateRequest;
    case X11EventType::X11PropertyNotify:
        return PropertyNotify;
    case X11EventType::X11SelectionClear:
        return SelectionClear;
    case X11EventType::X11SelectionRequest:
        return SelectionRequest;
    case X11EventType::X11SelectionNotify:
        return SelectionNotify;
    case X11EventType::X11ColormapNotify:
        return ColormapNotify;
    case X11EventType::X11ClientMessage:
        return ClientMessage;
    case X11EventType::X11MappingNotify:
        return MappingNotify;
    case X11EventType::X11GenericEvent:
        return GenericEvent;
    case X11EventType::X11LASTEvent:
        return LASTEvent;
    }

    // stop the non-void function end of control flow warning
    return LASTEvent;
}

} // namespace Tilebox
