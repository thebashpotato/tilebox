#include "ui/font.hpp"
#include <utility>

#include <tilebox/x11/display.hpp>

auto main() -> int
{
    if (auto dpy_opt = Tilebox::X11Display::Create(); dpy_opt.has_value())
    {
        const auto dpy = std::move(dpy_opt.value());
        if (auto font_res = Tilebox::Ui::X11Font::TryCreate(dpy, "monospace:size=12"); font_res.is_ok())
        {
            auto font = std::move(*font_res.ok());
        }
    }
}
