#pragma once

#include "tilebox-core/error.hpp"
#include "tilebox-core/geometry.hpp"
#include "tilebox-core/utils/attributes.hpp"
#include "tilebox-core/vendor/etl.hpp"
#include "tilebox-core/x11/display.hpp"
#include <X11/X.h>
#include <X11/Xft/Xft.h>
#include <fontconfig/fontconfig.h>
#include <ft2build.h>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <variant>

namespace tilebox::core
{

struct TILEBOX_INTERNAL XftFontDeleter
{
    X11DisplaySharedResource dpy;

    explicit XftFontDeleter(X11DisplaySharedResource display) noexcept;

    auto operator()(XftFont *font) const noexcept -> void;
};

using XftFontUniqueResource = std::unique_ptr<XftFont, XftFontDeleter>;

class TILEBOX_EXPORT X11Font
{
  public:
    ~X11Font();
    X11Font(X11Font &&rhs) noexcept;
    X11Font(const X11Font &rhs) noexcept = delete;

  public:
    auto operator=(X11Font &&rhs) noexcept -> X11Font &;
    auto operator=(const X11Font &rhs) noexcept -> X11Font & = delete;

  public:
    /// @brief Creates a font based on the name of the font.
    [[nodiscard]] static auto create(const X11DisplaySharedResource &dpy, const std::string &font_name) noexcept
        -> etl::Result<X11Font, X11FontError>;

    /// @brief Creates a font based on the pattern of the font.
    [[nodiscard]] static auto create(const X11DisplaySharedResource &dpy, FcPattern *font_pattern) noexcept
        -> etl::Result<X11Font, X11FontError>;

    [[nodiscard]] auto height() const noexcept -> Height;
    [[nodiscard]] auto pattern() const noexcept -> FcPattern *;

  private:
    X11Font(XftFontUniqueResource &&xft_font, FcPattern *pattern, Height height) noexcept;

  private:
    XftFontUniqueResource _xftfont;
    // we can't have a smart pointer, Keith Packard forward declared
    // the FcPattern type, thus the call to sizeof() in the unique_ptr constructor will
    // fail at compile time due to the incomplete type.
    FcPattern *_pattern;
    Height _height;
};

} // namespace tilebox::core

namespace etl
{

/// @brief Template specializaion for `Result<tilebox::core::X11Font, Error>`
template <typename ErrType> class Result<tilebox::core::X11Font, ErrType>
{
  private:
    std::variant<tilebox::core::X11Font, ErrType> _result{};
    bool _is_ok{};

  public:
    Result() noexcept = default;
    ~Result() = default;

    explicit Result(tilebox::core::X11Font value) noexcept : _result(std::move(value)), _is_ok(true)
    {
    }

    explicit Result(const ErrType &error) noexcept : _result(error)
    {
    }

    explicit Result(ErrType &&error) noexcept : _result(std::move(error))
    {
    }

    Result(Result &&rhs) noexcept : _result(std::move(rhs._result)), _is_ok(rhs._is_ok)
    {
        rhs._is_ok = false;
    }

    Result(const Result &rhs) noexcept = delete;

    auto operator=(Result &&rhs) noexcept -> Result &
    {
        if (this != &rhs)
        {
            _result = std::move(rhs._result);
            _is_ok = rhs.is_ok;
            rhs.is_ok = false;
        }

        return *this;
    }

    auto operator=(const Result &rhs) noexcept = delete;

  public:
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
    /// @return std::optinal<tilebox::core::X11Font> for safety, incase the user did not call
    /// isOk() before using this method.
    [[nodiscard]] inline auto ok() noexcept -> std::optional<tilebox::core::X11Font>
    {
        std::optional<tilebox::core::X11Font> opt;
        if (_is_ok)
        {
            if (auto *value = std::get_if<tilebox::core::X11Font>(&_result))
            {
                opt.emplace(std::move(*value));
                return opt;
            }
        }
        return opt;
    }

    /// @brief Check if the union value is of the error type
    ///
    /// @details The use should always use isErr() before using err()
    ///
    /// @return std::optinal<ErrType> for safety, incase the user did not call
    /// isErr() before using this method.
    [[nodiscard]] inline auto err() const noexcept -> std::optional<ErrType>
    {
        std::optional<ErrType> opt;
        if (!_is_ok)
        {
            if (auto *err = std::get_if<ErrType>(&_result))
            {
                opt.emplace(std::move(*err));
                return opt;
            }
        }
        return opt;
    }
};
} // namespace etl
