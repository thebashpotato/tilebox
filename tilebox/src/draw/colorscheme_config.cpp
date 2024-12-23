#include "tilebox/draw/colorscheme_config.hpp"

#include <string>
#include <utility>

namespace Tilebox
{

auto ColorSchemeConfig::Build(const ColorSchemeKind kind) -> ColorSchemeConfigBuilder
{
    return ColorSchemeConfigBuilder(kind);
}

auto ColorSchemeConfig::kind() const -> ColorSchemeKind
{
    return m_kind;
}

auto ColorSchemeConfig::foreground() const -> const std::string &
{
    return m_fg;
}

auto ColorSchemeConfig::background() const -> const std::string &
{
    return m_bg;
}

auto ColorSchemeConfig::border() const -> const std::string &
{
    return m_border;
}

ColorSchemeConfigBuilder::ColorSchemeConfigBuilder(const ColorSchemeKind kind) noexcept
{
    m_def.m_kind = kind;
}

ColorSchemeConfigBuilder::operator ColorSchemeConfig()
{
    return std::move(m_def);
}

auto ColorSchemeConfigBuilder::foreground(std::string &&hex_code) -> ColorSchemeConfigBuilder &
{
    m_def.m_fg = std::move(hex_code);
    return *this;
}

auto ColorSchemeConfigBuilder::background(std::string &&hex_code) -> ColorSchemeConfigBuilder &
{
    m_def.m_bg = std::move(hex_code);
    return *this;
}

auto ColorSchemeConfigBuilder::border(std::string &&hex_code) -> ColorSchemeConfigBuilder &
{
    m_def.m_border = std::move(hex_code);
    return *this;
}

} // namespace Tilebox
