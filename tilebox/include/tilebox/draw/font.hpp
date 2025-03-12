#pragma once

#include "tilebox/error.hpp"
#include "tilebox/geometry.hpp"
#include "tilebox/utils/attributes.hpp"
#include "tilebox/x11/display.hpp"

#include <X11/Xft/Xft.h>
#include <etl.hpp>
#include <fontconfig/fontconfig.h>
#include <ft2build.h>

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>

namespace Tilebox
{

struct TILEBOX_INTERNAL XftFontDeleter
{
    X11DisplaySharedResource dpy;

    explicit XftFontDeleter(X11DisplaySharedResource display) noexcept;

    auto operator()(XftFont *font) const noexcept -> void;
};

using XftFontSharedResource = std::shared_ptr<XftFont>;

/// @brief Provides a RAII wrapper around an XftFont and fontconfig.
class TILEBOX_INTERNAL X11Font
{
  public:
    /// @brief An X11Font can be represented by one of these 3 types.
    enum class Type : std::uint8_t
    {
        Primary,
        Secondary,
        Tertiary, // Should remain the last font type in the enum at all times.
    };

    using TypeIterator = etl::EnumerationIterator<Type, Type::Primary, Type::Tertiary>;

    /// @brief Converts the underlying Type enum class to its integer equivalent
    [[nodiscard]] constexpr static auto ToUnderlying(const Type type) noexcept -> std::uint8_t
    {
        return std::underlying_type_t<Type>(type);
    }

  public:
    X11Font() = default;
    ~X11Font();
    X11Font(X11Font &&rhs) noexcept;
    X11Font(const X11Font &rhs) noexcept;

  public:
    auto operator=(X11Font &&rhs) noexcept -> X11Font &;
    auto operator=(const X11Font &rhs) noexcept -> X11Font &;

  public:
    /// @brief Creates a font based on the name of the font.
    [[nodiscard]] static auto Create(const X11DisplaySharedResource &dpy, const std::string &font_name,
                                     Type type) noexcept -> etl::Result<X11Font, X11FontError>;

    /// @brief Creates a font based on the pattern of the font.
    [[nodiscard]] static auto Create(const X11DisplaySharedResource &dpy, FcPattern *font_pattern, Type type) noexcept
        -> etl::Result<X11Font, X11FontError>;

    [[nodiscard]] auto type() const noexcept -> std::optional<Type>;
    [[nodiscard]] auto xftfont() const noexcept -> const XftFontSharedResource &;
    [[nodiscard]] auto pattern() const noexcept -> FcPattern *;
    [[nodiscard]] auto height() const noexcept -> Height;

  private:
    X11Font(XftFontSharedResource &&xft_font, FcPattern *pattern, Type type, Height height) noexcept;

  private:
    XftFontSharedResource m_xftfont;
    // we can't have a smart pointer, Keith Packard forward declared
    // the FcPattern type, thus the call to sizeof() in the unique_ptr constructor will
    // fail at compile time due to the incomplete type.
    FcPattern *m_pattern{};
    std::optional<Type> m_type;
    Height m_height;
};

} // namespace Tilebox
