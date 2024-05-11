#include <type_traits>
#include <utility>

namespace tilebox::core
{

/// @brief Tag a primitive fundamental type to descriptive class names.
///
/// @details Solves the issue when a function takes in many arguments of the same type,
/// which can lead to confusion and mistakes from the programmer.
/// For example consider a Rectangle with a constructor that takes a uint32_t width, and uint32_t height.
/// The width and the height can easily be swapped, resulting in runtime bugs.
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

    explicit TaggedFundamental(FundamentalType value) : value(std::move(value))
    {
        static_assert(std::is_fundamental<FundamentalType>::value);
    }

    /// @brief Default Destructor Move/Copy constructor and assignment
    virtual ~TaggedFundamental() = default;
    TaggedFundamental(TaggedFundamental &&other) noexcept = default;
    auto operator=(TaggedFundamental &&other) noexcept -> TaggedFundamental & = default;
    TaggedFundamental(const TaggedFundamental &other) = default;
    auto operator=(const TaggedFundamental &other) -> TaggedFundamental & = default;

  public:
    auto operator+(const TaggedFundamental &other) const -> TaggedFundamental
    {
        return TaggedFundamental(value + other.value);
    }

    auto operator-(const TaggedFundamental &other) const -> TaggedFundamental
    {
        return TaggedFundamental(value - other.value);
    }
};

} // namespace tilebox::core
