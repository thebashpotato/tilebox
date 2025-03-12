#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <cstdint>

namespace Tbwm
{

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)

/// @brief Global pointer for the original error handler
inline int (*g_error_handler_callback)(Display *, XErrorEvent *) = nullptr;

/// @brief Allows to check from Xlib error callback, rather than calling std::exit
///
/// @details Only used in the beginning of the program, not that big of a deal.
inline bool g_another_window_manager_is_running = false;

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

/// @brief This function calls std::exit, any heap allocated memory will be left to the OS
/// to clean up, the good news is it won't be much.
[[nodiscard]] auto WmStartupErrorHandler(Display *dpy, XErrorEvent *ee) -> std::int32_t;

/// @brief Error handler for all runtime errors
///
/// @details There's no way to check accesses to destroyed windows, thus those cases are
/// ignored (especially UnmapNotify's). Other types of errors call Xlibs default error handler,
/// which may call exit
[[nodiscard]] auto WmRuntimeErrorHandler(Display *dpy, XErrorEvent *ee) -> std::int32_t;

} // namespace Tbwm
