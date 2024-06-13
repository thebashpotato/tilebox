#pragma once

#include "tilebox-core/vendor/etl.hpp"
#include <string>
#include <string_view>

namespace tilebox::core
{

/// @brief Basic tilebox core library error, this error is considered to be unrecoverable.
class CoreError : public etl::IError
{
  public:
    /// @brief Constructs the error with only a message
    ///
    /// @details This constructor is private to prevent the user from circumventing the create() method
    explicit CoreError(std::string_view const &msg) noexcept : _msg(msg)
    {
    }

    /// @brief Constructs the error with the message and source location
    /// using move semantics
    ///
    /// @details This constructor is private to prevent the user from circumventing the create() method
    ///
    /// @param `msg` the error message
    /// @param `slc` the source code location object
    CoreError(std::string_view const &msg, etl::SourceCodeLocation const &slc) noexcept : _msg(msg)
    {
        _info.append("Error: ")
            .append(msg)
            .append("\nFunction: ")
            .append(slc.function())
            .append("\nFile: ")
            .append(slc.file())
            .append(":")
            .append(std::to_string(slc.line()));
    }

    /// @brief Default Destructor, Move/Copy constructor and assignment
    ~CoreError() override = default;
    CoreError(CoreError &&other) noexcept = default;
    auto operator=(CoreError &&other) noexcept -> CoreError & = default;
    CoreError(CoreError const &other) = default;
    auto operator=(CoreError const &other) -> CoreError & = default;

  public:
    /// @brief Get just the error message
    [[nodiscard]] inline auto msg() const noexcept -> std::string override
    {
        return _msg;
    }

    /// @brief Get the pre-formatted (pretty printed) error string.
    ///
    /// @details  If Error was not created with the RUNTIME_INFO macro info_ will be empty,
    /// in which case the msg_ will be returned instead.
    [[nodiscard]] inline auto info() const noexcept -> std::string override
    {
        if (_info.empty())
        {
            return _msg;
        }
        return _info;
    }

  private:
    std::string _msg;
    std::string _info;
};

/// @brief Error wrapper for fontconfig and XftFont.
class X11FontError : public etl::IError
{
  public:
    /// @brief Constructs the error with only a message
    ///
    /// @details This constructor is private to prevent the user from circumventing the create() method
    explicit X11FontError(std::string_view const &msg) noexcept : _msg(msg)
    {
    }

    /// @brief Constructs the error with the message and source location
    /// using move semantics
    ///
    /// @details This constructor is private to prevent the user from circumventing the create() method
    ///
    /// @param `msg` the error message
    /// @param `slc` the source code location object
    X11FontError(std::string_view const &msg, etl::SourceCodeLocation const &slc) noexcept : _msg(msg)
    {
        _info.append("Error: ")
            .append(msg)
            .append("\nFunction: ")
            .append(slc.function())
            .append("\nFile: ")
            .append(slc.file())
            .append(":")
            .append(std::to_string(slc.line()));
    }

    /// @brief Default Destructor, Move/Copy constructor and assignment
    ~X11FontError() override = default;
    X11FontError(X11FontError &&other) noexcept = default;
    auto operator=(X11FontError &&other) noexcept -> X11FontError & = default;
    X11FontError(X11FontError const &other) = default;
    auto operator=(X11FontError const &other) -> X11FontError & = default;

  public:
    /// @brief Get just the error message
    [[nodiscard]] inline auto msg() const noexcept -> std::string override
    {
        return _msg;
    }

    /// @brief Override the current error message, useful when using the Result.mapErr method.
    [[nodiscard]] inline auto set(std::string_view const &msg) noexcept
    {
        _msg = msg;
    }

    /// @brief Get the pre-formatted (pretty printed) error string.
    ///
    /// @details  If Error was not created with the RUNTIME_INFO macro info_ will be empty,
    /// in which case the msg_ will be returned instead.
    [[nodiscard]] inline auto info() const noexcept -> std::string override
    {
        if (_info.empty())
        {
            return _msg;
        }
        return _info;
    }

  private:
    std::string _msg;
    std::string _info;
};

} // namespace tilebox::core
