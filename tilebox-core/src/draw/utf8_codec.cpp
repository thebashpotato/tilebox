#include "tilebox-core/draw/utf8_codec.hpp"
#include "tilebox-core/error.hpp"
#include "tilebox-core/vendor/etl.hpp"
#include "tilebox-core/vendor/utf8/checked.h"
#include <iterator>
#include <string>
#include <utility>

using namespace etl;

namespace tilebox::core
{

auto utf8_decode(const std::string &input) noexcept -> Result<std::u32string, CoreError>
{
    if (input.empty())
    {
        return Result<std::u32string, CoreError>(
            CoreError::create("UTF-8 input is empty, cannot decode", RUNTIME_INFO));
    }

    std::u32string output{};
    try
    {
        utf8::utf8to32(input.begin(), input.end(), std::back_inserter(output));
    }
    catch (const utf8::invalid_utf8 &err)
    {
        return Result<std::u32string, CoreError>(CoreError::create(err.what(), RUNTIME_INFO));
    }
    catch (const utf8::not_enough_room &err)
    {
        return Result<std::u32string, CoreError>(CoreError::create(err.what(), RUNTIME_INFO));
    }

    return Result<std::u32string, CoreError>(std::move(output));
}

auto utf8_encode(const std::u32string &input) noexcept -> etl::Result<std::string, CoreError>
{
    if (input.empty())
    {
        return Result<std::string, CoreError>(CoreError::create("UTF-32 input is empty, cannot encode", RUNTIME_INFO));
    }

    std::string output{};
    try
    {
        utf8::utf32to8(input.begin(), input.end(), std::back_inserter(output));
    }
    catch (const utf8::invalid_utf8 &err)
    {
        return etl::Result<std::string, CoreError>(CoreError::create(err.what(), RUNTIME_INFO));
    }
    catch (const utf8::not_enough_room &err)
    {
        return Result<std::string, CoreError>(CoreError::create(err.what(), RUNTIME_INFO));
    }

    return Result<std::string, CoreError>(std::move(output));
}

} // namespace tilebox::core
