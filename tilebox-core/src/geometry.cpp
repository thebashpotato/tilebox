#include "tilebox-core/geometry.hpp"
#include <cstdint>

namespace tilebox::core
{

Coordinate::Coordinate(std::uint32_t x_coord, std::uint32_t y_coord) : x(x_coord), y(y_coord)
{
}

auto Coordinate::operator+(const Coordinate &other) const -> Coordinate
{
    return {x + other.x, y + other.y};
}

auto Coordinate::operator-(const Coordinate &other) const -> Coordinate
{
    return {x - other.x, y - other.y};
}

Rect::Rect(Coordinate coord, std::uint32_t width, std::uint32_t height) : c(coord), w(width), h(height)
{
}

auto Rect::operator+(const Rect &other) const -> Rect
{
    return {c + other.c, w + other.w, h + other.h};
}

auto Rect::operator-(const Rect &other) const -> Rect
{
    return {c - other.c, w - other.w, h - other.h};
}

} // namespace tilebox::core
