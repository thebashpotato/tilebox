#pragma once

#include "tilebox-core/draw/colorscheme.hpp"
#include "tilebox-core/draw/colorscheme_config.hpp"
#include "tilebox-core/draw/font.hpp"
#include "tilebox-core/error.hpp"
#include "tilebox-core/geometry.hpp"
#include "tilebox-core/utils/attributes.hpp"
#include "tilebox-core/vendor/etl.hpp"
#include "tilebox-core/x11/display.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <string>
#include <string_view>
#include <vector>

namespace tilebox::core
{

/// @brief The main class of the drawing library, it handles drawing text, and colors to rectangles.
class TILEBOX_EXPORT X11Draw
{
  public:
    ~X11Draw() noexcept;
    X11Draw(X11Draw &&rhs) noexcept;
    X11Draw(const X11Draw &rhs) noexcept = delete;

  public:
    auto operator=(X11Draw &&rhs) noexcept -> X11Draw &;
    auto operator=(const X11Draw &rhs) noexcept -> X11Draw & = delete;

  public:
    [[nodiscard]] static auto create(const X11DisplaySharedResource &dpy, const Width &width,
                                     const Height &height) noexcept -> etl::Result<X11Draw, CoreError>;

    [[nodiscard]] auto add_font_set(const std::string &font_name) noexcept -> etl::Result<etl::Void, X11FontError>;

    [[nodiscard]] auto add_colorscheme(const ColorSchemeConfig &config) noexcept
        -> etl::Result<etl::Void, X11ColorError>;

    [[nodiscard]] auto remove_colorscheme(const ColorSchemeKind kind) noexcept -> bool;

    [[nodiscard]] auto get_colorscheme(const ColorSchemeKind kind) const noexcept -> std::optional<X11ColorScheme>;

    auto resize(const Width &width, const Height &height) noexcept -> void;

    [[nodiscard]] auto width() const noexcept -> const Width &;

    [[nodiscard]] auto height() const noexcept -> const Height &;

  private:
    [[nodiscard]] auto _get_text_extents(const X11Font &font, const std::string_view &text,
                                         const uint32_t len) noexcept -> etl::Result<Vec2D, X11FontError>;

  private:
    X11Draw(X11DisplaySharedResource dpy, GC graphics_ctx, Drawable drawable, Width width, Height height) noexcept;

  private:
    X11DisplaySharedResource _dpy;
    std::vector<X11Font> _fonts;
    std::vector<X11ColorScheme> _colorschemes;
    GC _graphics_ctx;
    Drawable _drawable;
    Width _width;
    Height _height;
};

} // namespace tilebox::core

/// @brief Hi-jack the etl namespace to add a custom template specialization for tilebox::core::X11Draw
namespace etl
{

template <typename ErrType> class Result<tilebox::core::X11Draw, ErrType>
{
  public:
    Result() noexcept = default;

    explicit Result(tilebox::core::X11Draw &&value) noexcept : _result(std::move(value)), _is_ok(true)
    {
    }

    explicit Result(const ErrType &error) noexcept : _result(error)
    {
    }

    explicit Result(ErrType &&error) noexcept : _result(std::move(error))
    {
    }

  public:
    /// @brief Check if the union value is of the ok type
    [[nodiscard]] inline auto is_ok() const noexcept -> bool
    {
        return _is_ok;
    }

    /// @brief Check if the union value is of the error type
    [[nodiscard]] inline auto is_err() const noexcept -> bool
    {
        return !_is_ok;
    }

    /// @brief Check if the union value is of the error type
    ///
    /// @details The use should always use isOk() before using ok()
    ///
    /// @return std::optional<tilebox::core::X11Draw> for safety, incase the user did not call
    /// isOk() before using this method.
    [[nodiscard]] inline auto ok() noexcept -> std::optional<tilebox::core::X11Draw>
    {
        std::optional<tilebox::core::X11Draw> ret;
        if (_is_ok)
        {
            if (auto *value = std::get_if<tilebox::core::X11Draw>(&_result))
            {
                ret.emplace(std::move(*value));
            }
        }
        return ret;
    }

    /// @brief Check if the union value is of the error type
    ///
    /// @details The use should always use isErr() before using err()
    ///
    /// @return std::optional<ErrType> for safety, incase the user did not call
    /// isErr() before using this method.
    [[nodiscard]] inline auto err() const noexcept -> std::optional<ErrType>
    {
        std::optional<ErrType> ret;
        if (!_is_ok)
        {
            if (auto *err = std::get_if<ErrType>(&_result))
            {
                ret.emplace(*err);
            }
        }
        return ret;
    }

  private:
    std::variant<tilebox::core::X11Draw, ErrType> _result;
    bool _is_ok{};
};

} // namespace etl
