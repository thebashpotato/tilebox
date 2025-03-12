#include "tilebox/draw/color.hpp"
#include "tilebox/error.hpp"
#include "tilebox/x11/display.hpp"

#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <etl.hpp>

#include <memory>
#include <string>
#include <utility>

using namespace etl;

namespace Tilebox
{

XftColorDeleter::XftColorDeleter(X11DisplaySharedResource display) noexcept : dpy(std::move(display))
{
}

auto XftColorDeleter::operator()(XftColor *color) const noexcept -> void
{
    if (color != nullptr && dpy->IsConnected())
    {
        XftColorFree(dpy->Raw(), DefaultVisual(dpy->Raw(), dpy->ScreenId()),
                     DefaultColormap(dpy->Raw(), dpy->ScreenId()), color);
        delete color; // NOLINT
        color = nullptr;
    }
}

X11Color::X11Color(std::shared_ptr<XftColor> &&color) noexcept : m_color(std::move(color))
{
}

auto X11Color::Create(const X11DisplaySharedResource &dpy, const std::string &hex_code)
    -> Result<X11Color, X11ColorError>
{
    // Basic fail case handling.
    //
    // TODO: Write a hex code validator to ensure valid hex representation of RGBA color codes are being passed through.
    if (hex_code.empty())
    {
        return Result<X11Color, X11ColorError>(
            X11ColorError(std::string("Error, empty or invalid hex code ").append(hex_code), RUNTIME_INFO));
    }

    auto color = std::shared_ptr<XftColor>(new XftColor, XftColorDeleter(dpy));

    if (XftColorAllocName(dpy->Raw(), DefaultVisual(dpy->Raw(), dpy->ScreenId()),
                          DefaultColormap(dpy->Raw(), dpy->ScreenId()), hex_code.c_str(), color.get()) == False)
    {
        return Result<X11Color, X11ColorError>(X11ColorError(
            std::string("XftColorAllocName: Could not allocate hex code ").append(hex_code), RUNTIME_INFO));
    }

    return Result<X11Color, X11ColorError>(X11Color(std::move(color)));
}

auto X11Color::Raw() const noexcept -> XftColor *
{
    return this->m_color.get();
}

} // namespace Tilebox
