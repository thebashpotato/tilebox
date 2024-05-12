#include <type_traits>
#include <utility>

namespace tilebox::core
{

/// @brief Tag a primitive fundamental type to descriptive class names.
///
/// @details Solves the issue when a function takes in many arguments of the same type,
/// which can lead to confusion and mistakes from the programmer.
///
/// For example consider a Rectangle with a constructor that takes a
/// uint32_t x, uint32_t y, uint32_T width, and uint32_t height.
///
/// They can easily be swapped, resulting in runtime bugs.
/// This can be avoided by "Tagging the type".
template <typename Tag, typename FundamentalType> class TaggedFundamental
{
  public:
    FundamentalType value{};

  public:
    /// @brief All the constructors needed to build a fundamental wrapped type
    TaggedFundamental()
    {
        static_assert(std::is_fundamental<FundamentalType>::value);
    };

    constexpr explicit TaggedFundamental(FundamentalType value) noexcept : value(std::move(value))
    {
        static_assert(std::is_fundamental<FundamentalType>::value);
    }

    virtual ~TaggedFundamental() = default;
    TaggedFundamental(TaggedFundamental &&rhs) noexcept = default;
    auto operator=(TaggedFundamental &&rhs) noexcept -> TaggedFundamental & = default;
    TaggedFundamental(const TaggedFundamental &rhs) = default;
    auto operator=(const TaggedFundamental &rhs) -> TaggedFundamental & = default;

  public:
    auto operator+(const TaggedFundamental &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value + rhs.value);
    }

    auto operator+(const FundamentalType &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value + rhs);
    }

    auto operator-(const TaggedFundamental &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value - rhs.value);
    }

    auto operator-(const FundamentalType &rhs) const noexcept -> TaggedFundamental
    {
        return TaggedFundamental(value - rhs);
    }

    auto operator<(const TaggedFundamental &rhs) const noexcept -> bool
    {
        return value < rhs.value;
    }

    auto operator<(const FundamentalType &rhs) const noexcept -> bool
    {
        return value < rhs;
    }

    auto operator<=(const TaggedFundamental &rhs) const noexcept -> bool
    {
        return value <= rhs.value;
    }

    auto operator<=(const FundamentalType &rhs) const noexcept -> bool
    {
        return value <= rhs;
    }

    auto operator>(const TaggedFundamental &rhs) const noexcept -> bool
    {
        return value > rhs.value;
    }

    auto operator>(const FundamentalType &rhs) const noexcept -> bool
    {
        return value > rhs;
    }

    auto operator>=(const TaggedFundamental &rhs) const noexcept -> bool
    {
        return value >= rhs.value;
    }

    auto operator>=(const FundamentalType &rhs) const noexcept -> bool
    {
        return value >= rhs;
    }

    auto operator==(const TaggedFundamental &rhs) const noexcept -> bool
    {
        return value == rhs.value;
    }

    auto operator==(const FundamentalType &rhs) const noexcept -> bool
    {
        return value == rhs;
    }

    auto operator!=(const TaggedFundamental &rhs) const noexcept -> bool
    {
        return value != rhs.value;
    }

    auto operator!=(const FundamentalType &rhs) const noexcept -> bool
    {
        return value != rhs;
    }
};

} // namespace tilebox::core
