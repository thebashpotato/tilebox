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
    explicit CoreError(const std::string_view &msg) noexcept : _msg(msg)
    {
    }

    /// @brief Constructs the error with the message and source location
    /// using move semantics
    ///
    /// @details This constructor is private to prevent the user from circumventing the create() method
    ///
    /// @param `msg` the error message
    /// @param `slc` the source code location object
    CoreError(const std::string_view &msg, const etl::SourceCodeLocation &slc) noexcept : _msg(msg)
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
    CoreError(const CoreError &other) = default;
    auto operator=(const CoreError &other) -> CoreError & = default;

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
    explicit X11FontError(const std::string_view &msg) noexcept : _msg(msg)
    {
    }

    /// @brief Constructs the error with the message and source location
    /// using move semantics
    ///
    /// @details This constructor is private to prevent the user from circumventing the create() method
    ///
    /// @param `msg` the error message
    /// @param `slc` the source code location object
    X11FontError(const std::string_view &msg, const etl::SourceCodeLocation &slc) noexcept : _msg(msg)
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
    X11FontError(const X11FontError &other) = default;
    auto operator=(const X11FontError &other) -> X11FontError & = default;

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

/// @brief Error wrapper for XftColor.
class X11ColorError : public etl::IError
{
  public:
    /// @brief Constructs the error with only a message
    ///
    /// @details This constructor is private to prevent the user from circumventing the create() method
    explicit X11ColorError(const std::string_view &msg) noexcept : _msg(msg)
    {
    }

    /// @brief Constructs the error with the message and source location
    /// using move semantics
    ///
    /// @details This constructor is private to prevent the user from circumventing the create() method
    ///
    /// @param `msg` the error message
    /// @param `slc` the source code location object
    X11ColorError(const std::string_view &msg, const etl::SourceCodeLocation &slc) noexcept : _msg(msg)
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
    ~X11ColorError() override = default;
    X11ColorError(X11ColorError &&other) noexcept = default;
    auto operator=(X11ColorError &&other) noexcept -> X11ColorError & = default;
    X11ColorError(const X11ColorError &other) = default;
    auto operator=(const X11ColorError &other) -> X11ColorError & = default;

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

class X11CursorError : public etl::IError
{
  public:
    /// @brief Constructs the error with only a message
    ///
    /// @details This constructor is private to prevent the user from circumventing the create() method
    explicit X11CursorError(const std::string_view &msg) noexcept : _msg(msg)
    {
    }

    /// @brief Constructs the error with the message and source location
    /// using move semantics
    ///
    /// @details This constructor is private to prevent the user from circumventing the create() method
    ///
    /// @param `msg` the error message
    /// @param `slc` the source code location object
    X11CursorError(const std::string_view &msg, const etl::SourceCodeLocation &slc) noexcept : _msg(msg)
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
    ~X11CursorError() override = default;
    X11CursorError(X11CursorError &&other) noexcept = default;
    auto operator=(X11CursorError &&other) noexcept -> X11CursorError & = default;
    X11CursorError(const X11CursorError &other) = default;
    auto operator=(const X11CursorError &other) -> X11CursorError & = default;

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

} // namespace tilebox::core
