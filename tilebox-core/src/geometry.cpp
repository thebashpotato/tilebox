#include "tilebox-core/geometry.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <tuple>
#include <utility>

namespace tilebox::core
{

Point::Point(X x, Y y) : x(std::move(x)), y(std::move(y))
{
}

auto Point::operator+(const Point &other) const noexcept -> Point
{
    return {x - other.x, y - other.y};
}

auto Point::operator-(const Point &other) const noexcept -> Point
{
    return {x - other.x, y - other.y};
}

Rect::Rect(Point point, Width width, Height height)
    : point(std::move(point)), width(std::move(width)), height(std::move(height))
{
}

auto Rect::operator+(const Rect &other) const noexcept -> Rect
{
    return {point + other.point, width + other.width, height + other.height};
}

auto Rect::operator-(const Rect &other) const noexcept -> Rect
{
    return {point - other.point, width - other.width, height - other.height};
}

auto Rect::operator<(const Rect &other) const noexcept -> bool
{
    return width.value < other.width.value && height.value < other.height.value;
}

auto Rect::operator<=(const Rect &other) const noexcept -> bool
{
    return width.value <= other.width.value && height.value <= other.height.value;
}

auto Rect::operator>(const Rect &other) const noexcept -> bool
{
    return width.value > other.width.value && height.value > other.height.value;
}

auto Rect::operator>=(const Rect &other) const noexcept -> bool
{

    return width.value >= other.width.value && height.value >= other.height.value;
}

auto Rect::corners() const noexcept -> std::tuple<Point, Point, Point, Point>
{
    return {
        Point(point.x, point.y),                                                // top left
        Point(X(point.x.value + width.value), point.y),                         // top right
        Point(X(point.x.value + width.value), Y(point.y.value + height.value)), // bottom right
        Point(point.x, Y(point.y.value + height.value)),                        // bottom left
    };
}

auto Rect::midpoint() const noexcept -> Point
{
    return {X(point.x.value + width.value / 2), Y(point.y.value + height.value / 2)};
}

auto Rect::shrink_in(const std::uint32_t border) const noexcept -> Rect
{
    Width w;
    Height h;

    if (width.value <= 2 * border)
    {
        w.value = 1;
    }
    else
    {
        w.value = width.value - 2 * border;
    }

    if (height.value <= 2 * border)
    {
        h.value = 1;
    }
    else
    {
        h.value = height.value - 2 * border;
    }

    return {point, w, h};
}

auto Rect::scale_width(const std::double_t factor) const noexcept -> Rect
{
    return {
        point,
        Width(static_cast<std::uint32_t>(std::floor(width.value * factor))),
        height,
    };
}

auto Rect::scale_height(const std::double_t factor) const noexcept -> Rect
{
    return {
        point,
        width,
        Height(static_cast<std::uint32_t>(std::floor(height.value * factor))),
    };
}

auto Rect::resize(const std::int32_t dw, const std::int32_t dh) noexcept -> void
{
    width.value = static_cast<std::uint32_t>(std::max(1, (static_cast<std::int32_t>(width.value) + dw)));
    height.value = static_cast<std::uint32_t>(std::max(1, (static_cast<std::int32_t>(height.value) + dh)));
}

auto Rect::reposition(const std::int32_t dx, const std::int32_t dy) noexcept -> void
{
    point.x.value = static_cast<std::uint32_t>(std::max(0, static_cast<std::int32_t>(point.x.value) + dx));
    point.y.value = static_cast<std::uint32_t>(std::max(0, static_cast<std::int32_t>(point.y.value) + dy));
}

auto Rect::contains(const Rect &other) const noexcept -> bool
{
    if (other.point.x.value < point.x.value)
    {
        return false;
    }

    if (other.point.y.value < point.y.value)
    {
        return false;
    }

    if ((other.point.x.value + other.width.value) > (point.x.value + width.value))
    {
        return false;
    }

    if ((other.point.y.value + other.height.value) > (point.y.value + height.value))
    {
        return false;
    }

    return true;
}

auto Rect::contains_point(const Point &other_point) const noexcept -> bool
{
    return (point.x.value <= other_point.x.value && other_point.x.value <= (point.x.value + width.value)) &&
           (point.y.value <= other_point.y.value && other_point.y.value <= (point.x.value + height.value));
}

} // namespace tilebox::core
