#include <array>
#include <gtest/gtest.h>
#include <string>
#include <tilebox-core/draw/utf8_codec.hpp>
#include <tilebox-core/vendor/etl.hpp>

using namespace etl;
using namespace tilebox::core;

TEST(TilboxUtf8CodecTestSuite, DecodeValidUTF8)
{
    const std::string input = "Hello, 世界!";
    const auto result = utf8_decode(input);

    ASSERT_TRUE(result.is_ok());

    const std::u32string output = result.ok().value();

    ASSERT_EQ(output.size(), 10);
    ASSERT_EQ(output[0], U'H');
    ASSERT_EQ(output[7], U'世');
    ASSERT_EQ(output[8], U'界');
}

TEST(TilboxUtf8CodecTestSuite, DecodeInvalidUTF8)
{
    const std::string input = "Hello, \xF0\x28\x8C\x28";
    const auto result = utf8_decode(input);

    ASSERT_TRUE(result.is_err());
}

TEST(TilboxUtf8CodecTestSuite, EncodeValidUTF32)
{
    const std::u32string input = U"Hello, 世界!";
    const auto result = utf8_encode(input);

    ASSERT_TRUE(result.is_ok());

    const std::string output = result.ok().value();

    ASSERT_EQ(output.size(), 14);
    ASSERT_EQ(output, "Hello, 世界!");
}

TEST(TilboxUtf8CodecTestSuite, EncodeEmptyUTF32)
{
    const std::u32string input;

    const auto result = utf8_encode(input);

    ASSERT_TRUE(result.is_err());
}

TEST(TilboxUtf8CodecTestSuite, DecodeEmptyUTF8)
{
    const std::string input;

    const auto result = utf8_decode(input);

    ASSERT_TRUE(result.is_err());
}

TEST(TilboxUtf8CodecTestSuite, EncodeAndDecodeUTF8)
{
    const std::string original_utf8_str = "Hello, 世界!";
    const auto decoded_result = utf8_decode(original_utf8_str);

    ASSERT_TRUE(decoded_result.is_ok());

    const std::u32string utf32_str = decoded_result.ok().value();
    const auto encoded_result = utf8_encode(utf32_str);

    ASSERT_TRUE(encoded_result.is_ok());

    const std::string utf8_str = encoded_result.ok().value();
    ASSERT_EQ(original_utf8_str, utf8_str);
}

TEST(TilboxUtf8CodecTestSuite, DecodeStandardASCIIArray)
{
    const std::array<std::string, 9> input = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

    for (const std::string &i : input)
    {
        const auto decoded_result = utf8_decode(i);
        ASSERT_TRUE(decoded_result.is_ok());

        const std::u32string output = decoded_result.ok().value();
        ASSERT_EQ(output.size(), 1);
    }
}

TEST(TilboxUtf8CodecTestSuite, DecodeUTF8Array)
{
    const std::array<std::string, 9> input = {"", "", "", "", "", "", "", "", ""};

    for (const std::string &i : input)
    {
        const auto decoded_result = utf8_decode(i);
        ASSERT_TRUE(decoded_result.is_ok());

        const std::u32string output = decoded_result.ok().value();
        ASSERT_EQ(output.size(), 1);
    }
}
