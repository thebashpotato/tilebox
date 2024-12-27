#pragma once

#include "tilebox/utils/attributes.hpp"
#include <etl.hpp>

#include <cstdint>
#include <string>

namespace Tilebox
{

enum class ColorSchemeKind : std::uint8_t
{
    Primary,
    Secondary,
    Tertiary,
};

using ColorSchemeKindIterator =
    etl::EnumerationIterator<ColorSchemeKind, ColorSchemeKind::Primary, ColorSchemeKind::Tertiary>;

class ColorSchemeConfigBuilder;

class TILEBOX_EXPORT ColorSchemeConfig
{
  public:
    [[nodiscard]] static auto Build(ColorSchemeKind kind) -> ColorSchemeConfigBuilder;
    [[nodiscard]] auto kind() const -> ColorSchemeKind;
    [[nodiscard]] auto foreground() const -> const std::string &;
    [[nodiscard]] auto border() const -> const std::string &;
    [[nodiscard]] auto background() const -> const std::string &;

  private:
    friend class ColorSchemeConfigBuilder;
    explicit ColorSchemeConfig() = default;

    ColorSchemeKind m_kind{};
    std::string m_fg;
    std::string m_bg;
    std::string m_border;
};

class TILEBOX_EXPORT ColorSchemeConfigBuilder
{
  public:
    explicit ColorSchemeConfigBuilder(ColorSchemeKind kind) noexcept;
    operator ColorSchemeConfig(); // NOLINT

    [[nodiscard]] auto foreground(std::string &&hex_code) -> ColorSchemeConfigBuilder &;
    [[nodiscard]] auto background(std::string &&hex_code) -> ColorSchemeConfigBuilder &;
    [[nodiscard]] auto border(std::string &&hex_code) -> ColorSchemeConfigBuilder &;

  private:
    ColorSchemeConfig m_def;
};

} // namespace Tilebox
