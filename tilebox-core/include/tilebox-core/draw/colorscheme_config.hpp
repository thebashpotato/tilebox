#pragma once

#include "tilebox-core/utils/attributes.hpp"
#include <cstddef>
#include <string>

namespace tilebox::core
{

enum class ColorSchemeKind
{
    Primary,
    Secondary,
    Tertiary,
};

constexpr inline auto colorscheme_kind_size() -> std::size_t
{
    return static_cast<std::size_t>(static_cast<size_t>(ColorSchemeKind::Tertiary) + 1);
}

class ColorSchemeConfigBuilder;

class TILEBOX_EXPORT ColorSchemeConfig
{
  public:
    [[nodiscard]] static auto build(const ColorSchemeKind kind) -> ColorSchemeConfigBuilder;
    [[nodiscard]] auto kind() const -> ColorSchemeKind;
    [[nodiscard]] auto foreground() const -> const std::string &;
    [[nodiscard]] auto border() const -> const std::string &;
    [[nodiscard]] auto background() const -> const std::string &;

  private:
    friend class ColorSchemeConfigBuilder;
    explicit ColorSchemeConfig() = default;

    ColorSchemeKind _kind{};
    std::string _fg;
    std::string _bg;
    std::string _border;
};

class TILEBOX_EXPORT ColorSchemeConfigBuilder
{
  public:
    explicit ColorSchemeConfigBuilder(const ColorSchemeKind kind) noexcept;
    operator ColorSchemeConfig(); // NOLINT

    [[nodiscard]] auto foreground(std::string &&hex_code) -> ColorSchemeConfigBuilder &;
    [[nodiscard]] auto background(std::string &&hex_code) -> ColorSchemeConfigBuilder &;
    [[nodiscard]] auto border(std::string &&hex_code) -> ColorSchemeConfigBuilder &;

  private:
    ColorSchemeConfig _def;
};

} // namespace tilebox::core
