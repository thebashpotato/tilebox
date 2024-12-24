#include "xerror_handler.hpp"
#include "log.hpp"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>

#include <cstdint>
#include <cstdlib>

namespace Tilebox::Twm
{

auto WmStartupErrorHandler([[maybe_unused]] Display *dpy, [[maybe_unused]] XErrorEvent *ee) -> std::int32_t
{
    // TODO: Figure out a better way to handle errors other than abandoning all hope
    Log::Error("Another window manager is already running");
    std::exit(EXIT_FAILURE);

    // technically unreachable
    return -1;
}

auto WmRuntimeErrorHandler(Display *dpy, XErrorEvent *ee) -> std::int32_t
{
    if (ee->error_code == BadWindow || (ee->request_code == X_SetInputFocus && ee->error_code == BadMatch) ||
        (ee->request_code == X_PolyText8 && ee->error_code == BadDrawable) ||
        (ee->request_code == X_PolyFillRectangle && ee->error_code == BadDrawable) ||
        (ee->request_code == X_PolySegment && ee->error_code == BadDrawable) ||
        (ee->request_code == X_ConfigureWindow && ee->error_code == BadMatch) ||
        (ee->request_code == X_GrabButton && ee->error_code == BadAccess) ||
        (ee->request_code == X_GrabKey && ee->error_code == BadAccess) ||
        (ee->request_code == X_CopyArea && ee->error_code == BadDrawable))
    {
        return 0;
    }
    Log::Error("twm: fatal error: request code={}, error code={}", ee->request_code, ee->error_code);
    return g_error_handler_callback(dpy, ee); // may call exit
}

} // namespace Tilebox::Twm
