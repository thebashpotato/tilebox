#pragma once

#include "tilebox/error.hpp"
#include "tilebox/geometry.hpp"
#include "tilebox/x11/display.hpp"

#include <X11/Xft/Xft.h>
#include <etl.hpp>
#include <fontconfig/fontconfig.h>
#include <ft2build.h>

#include <cstdint>
#include <string>

namespace Tilebox::Ui
{

/// @brief Provides a RAII wrapper around an XftFont and fontconfig.
class X11Font
{
  public:
    X11Font() noexcept = default;
    ~X11Font();
    X11Font(X11Font &&rhs) noexcept;
    X11Font(const X11Font &rhs) = delete;
    auto operator=(X11Font &&rhs) noexcept -> X11Font &;
    auto operator=(const X11Font &rhs) -> X11Font & = delete;

  public:
    /// @brief Trys to creates a font based on the name of the font.
    [[nodiscard]] static auto TryCreate(const X11DisplaySharedResource &dpy, const std::string &font_name) noexcept
        -> etl::Result<X11Font, X11FontError>;

    /// @brief Trys to create a font based on the pattern of the font.
    [[nodiscard]] static auto TryCreate(const X11DisplaySharedResource &dpy, FcPattern *const fcp) noexcept
        -> etl::Result<X11Font, X11FontError>;

    [[nodiscard]] auto ContainsChar(const X11DisplaySharedResource &dpy, const char32_t c) const noexcept -> bool;

    [[nodiscard]] auto GetTextExtents(const X11DisplaySharedResource &dpy, const std::string_view &text) const noexcept
        -> etl::Result<std::pair<std::uint32_t, std::uint32_t>, X11FontError>;

    [[nodiscard]] auto FallbackForChar(const X11DisplaySharedResource &dpy, const char32_t c) const noexcept
        -> etl::Result<X11Font, X11FontError>;

    [[nodiscard]] auto FontMatch(const X11DisplaySharedResource &dpy, const char32_t c) const noexcept
        -> etl::Result<FcPattern *const, X11FontError>;

    [[nodiscard]] auto IsValid() const noexcept -> bool;
    [[nodiscard]] auto xftfont() const noexcept -> XftFont *;
    [[nodiscard]] auto height() const noexcept -> Height;

  private:
    X11Font(X11DisplaySharedResource dpy, XftFont *xft_font, FcPattern *pattern, Height height) noexcept;
    void Release() noexcept;

  private:
    X11DisplaySharedResource m_dpy;
    XftFont *m_xftfont{};
    FcPattern *m_pattern{};
    Height m_height;
};

} // namespace Tilebox::Ui

/// @brief Hi-jack the etl namespace to add a custom template specialization for Tilebox::Ui::X11Font
template <typename ErrType> class etl::Result<Tilebox::Ui::X11Font, ErrType>
{
  public:
    Result() noexcept = default;

    explicit Result(Tilebox::Ui::X11Font &&value) noexcept : m_result(std::move(value)), m_is_ok(true)
    {
    }

    explicit Result(const ErrType &error) noexcept : m_result(error)
    {
    }

    explicit Result(ErrType &&error) noexcept : m_result(std::move(error))
    {
    }

  public:
    /// @brief Check if the union value is of the ok type
    [[nodiscard]] auto is_ok() const noexcept -> bool
    {
        return m_is_ok;
    }

    /// @brief Check if the union value is of the error type
    [[nodiscard]] auto is_err() const noexcept -> bool
    {
        return !m_is_ok;
    }

    /// @brief Check if the union value is of the error type
    ///
    /// @details The use should always use is_ok() before using ok()
    ///
    /// @return std::optional<Tilebox::Ui::X11Font> for safety, in case the user did not call
    /// is_ok() before using this method.
    [[nodiscard]] auto ok() noexcept -> std::optional<Tilebox::Ui::X11Font>
    {
        std::optional<Tilebox::Ui::X11Font> ret;
        if (m_is_ok)
        {
            if (auto *value = std::get_if<Tilebox::Ui::X11Font>(&m_result))
            {
                ret.emplace(std::move(*value));
            }
        }
        return ret;
    }

    /// @brief Check if the union value is of the error type
    ///
    /// @details The use should always use is_err() before using err()
    ///
    /// @return std::optional<ErrType> for safety, in case the user did not call
    /// is_err() before using this method.
    [[nodiscard]] auto err() const noexcept -> std::optional<ErrType>
    {
        std::optional<ErrType> ret;
        if (!m_is_ok)
        {
            if (auto *err = std::get_if<ErrType>(&m_result))
            {
                ret.emplace(*err);
            }
        }
        return ret;
    }

  private:
    std::variant<Tilebox::Ui::X11Font, ErrType> m_result;
    bool m_is_ok{};
}; // namespace etl
