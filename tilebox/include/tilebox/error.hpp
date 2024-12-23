#pragma once

#include <etl.hpp>

#include <string_view>

namespace Tilebox
{

class Error final : public etl::BaseError
{
  public:
    explicit Error(const std::string_view &msg) noexcept : etl::BaseError(msg)
    {
    }

    Error(const std::string_view &msg, const etl::SourceCodeLocation &slc) noexcept : etl::BaseError(msg, slc)
    {
    }
};

class X11FontError final : public etl::BaseError
{
  public:
    explicit X11FontError(const std::string_view &msg) noexcept : etl::BaseError(msg)
    {
    }

    X11FontError(const std::string_view &msg, const etl::SourceCodeLocation &slc) noexcept : etl::BaseError(msg, slc)
    {
    }
};

class X11ColorError final : public etl::BaseError
{
  public:
    explicit X11ColorError(const std::string_view &msg) noexcept : etl::BaseError(msg)
    {
    }

    X11ColorError(const std::string_view &msg, const etl::SourceCodeLocation &slc) noexcept : etl::BaseError(msg, slc)
    {
    }
};

class X11CursorError final : public etl::BaseError
{
  public:
    explicit X11CursorError(const std::string_view &msg) noexcept : etl::BaseError(msg)
    {
    }

    X11CursorError(const std::string_view &msg, const etl::SourceCodeLocation &slc) noexcept : etl::BaseError(msg, slc)
    {
    }
};

} // namespace Tilebox
