#include "tilebox-core/draw/color.hpp"
#include "tilebox-core/error.hpp"
#include "tilebox-core/vendor/etl.hpp"
#include "tilebox-core/x11/display.hpp"
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <memory>
#include <string>
#include <utility>

using namespace tilebox::core;
using namespace etl;

XftColorDeleter::XftColorDeleter(X11DisplaySharedResource display) noexcept : dpy(std::move(display))
{
}

auto XftColorDeleter::operator()(XftColor *color) const noexcept -> void
{
    if (color != nullptr && dpy->is_connected())
    {
        XftColorFree(dpy->raw(), DefaultVisual(dpy->raw(), dpy->screen_id()),
                     DefaultColormap(dpy->raw(), dpy->screen_id()), color);
        delete color; // NOLINT
        color = nullptr;
    }
}

X11Color::X11Color(std::shared_ptr<XftColor> &&color) noexcept : _color(std::move(color))
{
}

auto X11Color::create(const X11DisplaySharedResource &dpy,
                      const std::string &hex_code) -> Result<X11Color, X11ColorError>
{
    // Basic fail case handling.
    //
    // TODO: Write a hex code validator to ensure valid hex representation of RGBA color
    // codes are being passed through.
    if (hex_code.empty())
    {
        return Result<X11Color, X11ColorError>(
            X11ColorError(std::string("Error, empty or invalid hex code ").append(hex_code), RUNTIME_INFO));
    }

    auto color = std::shared_ptr<XftColor>(new XftColor, XftColorDeleter(dpy));

    if (XftColorAllocName(dpy->raw(), DefaultVisual(dpy->raw(), dpy->screen_id()),
                          DefaultColormap(dpy->raw(), dpy->screen_id()), hex_code.c_str(), color.get()) == False)
    {
        return Result<X11Color, X11ColorError>(X11ColorError(
            std::string("XftColorAllocName: Could not allocate hex code ").append(hex_code), RUNTIME_INFO));
    }

    return Result<X11Color, X11ColorError>(X11Color(std::move(color)));
}

auto X11Color::raw() const noexcept -> XftColor *
{
    return this->_color.get();
}
