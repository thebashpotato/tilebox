#pragma once

#include "tilebox-core/utils/attributes.hpp"
#include <cstdint>

namespace tilebox::core
{

class TILEBOX_EXPORT Coordinate
{
  public:
    std::uint32_t x;
    std::uint32_t y;

  public:
    Coordinate(std::uint32_t x_coord, std::uint32_t y_coord);

    auto operator+(const Coordinate &other) const -> Coordinate;
    auto operator-(const Coordinate &other) const -> Coordinate;
};

class TILEBOX_EXPORT Rect
{
  public:
    Coordinate c;
    std::uint32_t w;
    std::uint32_t h;

  public:
    Rect(Coordinate coord, std::uint32_t width, std::uint32_t height);

    auto operator+(const Rect &other) const -> Rect;
    auto operator-(const Rect &other) const -> Rect;
};

} // namespace tilebox::core
