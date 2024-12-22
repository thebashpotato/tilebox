#include <tilebox/core.hpp>

namespace Tilebox
{

auto Factorial(int n) noexcept -> int
{
    int result = 1;

    while (n > 0)
    {
        result *= n;
        --n;
    }

    return result;
}

} // namespace Tilebox
