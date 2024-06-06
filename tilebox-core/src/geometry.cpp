#include "tilebox-core/geometry.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <tuple>
#include <utility>

using namespace tilebox::core;

Vec2D::Vec2D(const A &a, const B &b) noexcept : a(a.value), b(b.value)
{
}

Point::Point() noexcept : x(X(0)), y(Y(0))
{
}

Point::Point(X x, Y y) noexcept : x(std::move(x)), y(std::move(y))
{
}

auto Point::operator+(const Point &rhs) const noexcept -> Point
{
    return {x - rhs.x, y - rhs.y};
}

auto Point::operator-(const Point &rhs) const noexcept -> Point
{
    return {x - rhs.x, y - rhs.y};
}

auto Point::operator==(const Point &rhs) const noexcept -> bool
{
    return x == rhs.x && y == rhs.y;
}

auto Point::operator!=(const Point &rhs) const noexcept -> bool
{
    return !(*this == rhs);
}

Rect::Rect() : width(Width(1)), height(Height(1))
{
}

Rect::Rect(Width width, Height height) : width(std::move(width)), height(std::move(height))
{
}

Rect::Rect(Point point, Width width, Height height)
    : point(std::move(point)), width(std::move(width)), height(std::move(height))
{
}

auto Rect::operator+(const Rect &rhs) const noexcept -> Rect
{
    return {point + rhs.point, width + rhs.width, height + rhs.height};
}

auto Rect::operator-(const Rect &rhs) const noexcept -> Rect
{
    return {point - rhs.point, width - rhs.width, height - rhs.height};
}

auto Rect::operator<(const Rect &rhs) const noexcept -> bool
{
    return width < rhs.width && height < rhs.height;
}

auto Rect::operator<=(const Rect &rhs) const noexcept -> bool
{
    return width <= rhs.width && height <= rhs.height;
}

auto Rect::operator>(const Rect &rhs) const noexcept -> bool
{
    return width > rhs.width && height > rhs.height;
}

auto Rect::operator>=(const Rect &rhs) const noexcept -> bool
{

    return width >= rhs.width && height >= rhs.height;
}

auto Rect::operator==(const Rect &rhs) const noexcept -> bool
{
    return point == rhs.point && width == rhs.width && height == rhs.height;
}

auto Rect::operator!=(const Rect &rhs) const noexcept -> bool
{
    return !(*this == rhs);
}

auto Rect::x() const noexcept -> std::int32_t
{
    return static_cast<std::int32_t>(point.x.value);
}

auto Rect::y() const noexcept -> std::int32_t
{
    return static_cast<std::int32_t>(point.y.value);
}

auto Rect::w() const noexcept -> std::uint32_t
{
    return width.value;
}

auto Rect::h() const noexcept -> std::uint32_t
{
    return height.value;
}

auto Rect::corners() const noexcept -> std::tuple<Point, Point, Point, Point>
{
    return {
        Point(point.x, point.y),                                              // top left
        Point(X(point.x.value + static_cast<int32_t>(width.value)), point.y), // top right
        Point(X(point.x.value + static_cast<int32_t>(width.value)),
              Y(point.y.value + static_cast<int32_t>(height.value))),               // bottom right
        Point(point.x, Y(point.y.value + static_cast<std::int32_t>(height.value))), // bottom left
    };
}

auto Rect::midpoint() const noexcept -> Point
{
    return {X(point.x.value + static_cast<int32_t>(width.value / 2)),
            Y(point.y.value + static_cast<int32_t>(height.value / 2))};
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
        w.value = width.value - (2 * border);
    }

    if (height.value <= 2 * border)
    {
        h.value = 1;
    }
    else
    {
        h.value = height.value - (2 * border);
    }

    return {point, w, h};
}

auto Rect::scale_width(const std::double_t factor) const noexcept -> Rect
{
    if (factor == 1.0)
    {
        return *this;
    }

    return {
        point,
        Width(static_cast<std::uint32_t>(std::floor(width.value * factor))),
        height,
    };
}

auto Rect::scale_height(const std::double_t factor) const noexcept -> Rect
{
    if (factor == 1.0)
    {
        return *this;
    }

    return {
        point,
        width,
        Height(static_cast<std::uint32_t>(std::floor(height.value * factor))),
    };
}

auto Rect::resize(const X &dw, const Y &dh) noexcept -> void
{
    const X min_width(1);
    const Y min_height(1);

    if (dw != 0)
    {
        width = Width(static_cast<uint32_t>(std::max(min_width.value, dw.value + static_cast<int32_t>(width.value))));
    }

    if (dh != 0)
    {
        height =
            Height(static_cast<uint32_t>(std::max(min_height.value, dh.value + static_cast<int32_t>(height.value))));
    }
}

auto Rect::reposition(const X &dx, const Y &dy) noexcept -> void
{
    const X min_x(0);
    const Y min_y(0);
    if (dx != 0)
    {
        point.x = X(std::max(min_x, dx + point.x));
    }

    if (dy != 0)
    {
        point.y = Y(std::max(min_y, dy + point.y));
    }
}

auto Rect::contains(const Rect &rhs) const noexcept -> bool
{
    if (rhs.point.x < point.x)
    {
        return false;
    }

    if (rhs.point.y < point.y)
    {
        return false;
    }

    if ((rhs.point.x + static_cast<int32_t>(rhs.width.value)) > (point.x + static_cast<int32_t>(width.value)))
    {
        return false;
    }

    if ((rhs.point.y + static_cast<int32_t>(rhs.height.value)) > (point.y + static_cast<int32_t>(height.value)))
    {
        return false;
    }

    return true;
}

auto Rect::contains_point(const Point &rhs) const noexcept -> bool
{
    return (point.x <= rhs.x && rhs.x <= (point.x + static_cast<int32_t>(width.value))) &&
           (point.y <= rhs.y && rhs.y <= (point.y + static_cast<int32_t>(height.value)));
}
