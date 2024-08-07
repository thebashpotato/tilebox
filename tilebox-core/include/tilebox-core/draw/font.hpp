#pragma once

#include "tilebox-core/error.hpp"
#include "tilebox-core/geometry.hpp"
#include "tilebox-core/utils/attributes.hpp"
#include "tilebox-core/vendor/etl.hpp"
#include "tilebox-core/x11/display.hpp"
#include <X11/X.h>
#include <X11/Xft/Xft.h>
#include <cstdint>
#include <fontconfig/fontconfig.h>
#include <ft2build.h>
#include <memory>
#include <optional>
#include <string>

namespace tilebox::core
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
    enum class Type : std::uint32_t
    {
        Primary,
        Secondary,
        Tertiary, // Should remain the last font type in the enum at all times.
    };

    /// @brief Returns the size of the X11Font::Type enum for the purposes of statically
    /// defining stack allocated containers.
    [[nodiscard]] constexpr static inline auto get_underlying_size() noexcept -> std::size_t
    {
        return static_cast<std::size_t>(static_cast<std::size_t>(X11Font::Type::Tertiary) + 1);
    }

    /// @brief Converts the underlying Type enum class to its integer equivalent
    [[nodiscard]] constexpr static inline auto to_underlying(const X11Font::Type type) noexcept -> std::uint32_t
    {
        return static_cast<std::underlying_type_t<X11Font::Type>>(type);
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
    [[nodiscard]] static auto create(const X11DisplaySharedResource &dpy, const std::string &font_name,
                                     const X11Font::Type type) noexcept -> etl::Result<X11Font, X11FontError>;

    /// @brief Creates a font based on the pattern of the font.
    [[nodiscard]] static auto create(const X11DisplaySharedResource &dpy, FcPattern *font_pattern,
                                     const X11Font::Type type) noexcept -> etl::Result<X11Font, X11FontError>;

    [[nodiscard]] auto type() const noexcept -> std::optional<X11Font::Type>;
    [[nodiscard]] auto xftfont() const noexcept -> const XftFontSharedResource &;
    [[nodiscard]] auto pattern() const noexcept -> FcPattern *;
    [[nodiscard]] auto height() const noexcept -> Height;

  private:
    X11Font(XftFontSharedResource &&xft_font, FcPattern *pattern, const X11Font::Type type, Height height) noexcept;

  private:
    XftFontSharedResource _xftfont;
    // we can't have a smart pointer, Keith Packard forward declared
    // the FcPattern type, thus the call to sizeof() in the unique_ptr constructor will
    // fail at compile time due to the incomplete type.
    FcPattern *_pattern{nullptr};
    std::optional<X11Font::Type> _type;
    Height _height;
};

} // namespace tilebox::core
