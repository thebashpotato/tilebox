#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <cstdint>

namespace Tilebox::Twm
{

// Global pointer for the original error handler
static int (*g_error_handler_callback)(Display *, XErrorEvent *) = nullptr;

/// @brief This function calls std::exit, any heap allocated memory will be left to the OS
/// to clean up, the good news is it won't be much.
[[nodiscard]] auto WmStartupErrorHandler(Display *dpy, XErrorEvent *ee) -> std::int32_t;

/// @brief Error handler for all runtime errors
///
/// @details There's no way to check accesses to destroyed windows, thus those cases are
/// ignored (especially UnmapNotify's). Other types of errors call Xlibs default error handler,
/// which may call exit
[[nodiscard]] auto WmRuntimeErrorHandler(Display *dpy, XErrorEvent *ee) -> std::int32_t;

} // namespace Tilebox::Twm
