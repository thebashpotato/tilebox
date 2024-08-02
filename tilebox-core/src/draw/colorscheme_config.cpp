#include "tilebox-core/draw/colorscheme_config.hpp"
#include <string>
#include <utility>

using namespace tilebox::core;

auto ColorSchemeConfig::build(const ColorSchemeKind kind) -> ColorSchemeConfigBuilder
{
    return ColorSchemeConfigBuilder(kind);
}

auto ColorSchemeConfig::kind() const -> ColorSchemeKind
{
    return _kind;
}

auto ColorSchemeConfig::foreground() const -> const std::string &
{
    return _fg;
}

auto ColorSchemeConfig::background() const -> const std::string &
{
    return _bg;
}

auto ColorSchemeConfig::border() const -> const std::string &
{
    return _border;
}

ColorSchemeConfigBuilder::ColorSchemeConfigBuilder(const ColorSchemeKind kind) noexcept
{
    _def._kind = kind;
}

ColorSchemeConfigBuilder::operator ColorSchemeConfig()
{
    return std::move(_def);
}

auto ColorSchemeConfigBuilder::foreground(std::string &&hex_code) -> ColorSchemeConfigBuilder &
{
    _def._fg = std::move(hex_code);
    return *this;
}

auto ColorSchemeConfigBuilder::background(std::string &&hex_code) -> ColorSchemeConfigBuilder &
{
    _def._bg = std::move(hex_code);
    return *this;
}

auto ColorSchemeConfigBuilder::border(std::string &&hex_code) -> ColorSchemeConfigBuilder &
{
    _def._border = std::move(hex_code);
    return *this;
}
