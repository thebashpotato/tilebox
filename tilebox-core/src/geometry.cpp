#include "tilebox-core/geometry.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <tuple>
#include <utility>

using namespace tilebox::core;

Vec2D::Vec2D() noexcept : width(Width(0)), height(Height(1))
{
}

Vec2D::Vec2D(Width w, Height h) noexcept : width(std::move(w)), height(std::move(h))
{
}

Point::Point() noexcept : x(X(0)), y(Y(0))
{
}

Point::Point(X px, Y py) noexcept : x(std::move(px)), y(std::move(py))
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

Rect::Rect(Width w, Height h) : width(std::move(w)), height(std::move(h))
{
}

Rect::Rect(Point p, Width w, Height h) : point(std::move(p)), width(std::move(w)), height(std::move(h))
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

auto Rect::x() const noexcept -> int32_t
{
    return point.x.value;
}

auto Rect::y() const noexcept -> int32_t
{
    return point.y.value;
}

auto Rect::w() const noexcept -> uint32_t
{
    return width.value;
}

auto Rect::h() const noexcept -> uint32_t
{
    return height.value;
}

auto Rect::corners() const noexcept -> std::tuple<Point, Point, Point, Point>
{
    return {
        Point(point.x, point.y),                                              // top left
        Point(X(point.x.value + static_cast<int32_t>(width.value)), point.y), // top right
        Point(X(point.x.value + static_cast<int32_t>(width.value)),
              Y(point.y.value + static_cast<int32_t>(height.value))),          // bottom right
        Point(point.x, Y(point.y.value + static_cast<int32_t>(height.value))), // bottom left
    };
}

auto Rect::midpoint() const noexcept -> Point
{
    return {X(point.x.value + static_cast<int32_t>(width.value / 2)),
            Y(point.y.value + static_cast<int32_t>(height.value / 2))};
}

auto Rect::shrink_in(const uint32_t border) const noexcept -> Rect
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

auto Rect::scale_width(const double_t factor) const noexcept -> Rect
{
    if (factor == 1.0)
    {
        return *this;
    }

    return {
        point,
        Width(static_cast<uint32_t>(std::floor(width.value * factor))),
        height,
    };
}

auto Rect::scale_height(const double_t factor) const noexcept -> Rect
{
    if (factor == 1.0)
    {
        return *this;
    }

    return {
        point,
        width,
        Height(static_cast<uint32_t>(std::floor(height.value * factor))),
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

auto Rect::contains(const Rect &other) const noexcept -> bool
{
    if (other.point.x < point.x)
    {
        return false;
    }

    if (other.point.y < point.y)
    {
        return false;
    }

    if ((other.point.x + static_cast<int32_t>(other.width.value)) > (point.x + static_cast<int32_t>(width.value)))
    {
        return false;
    }

    if ((other.point.y + static_cast<int32_t>(other.height.value)) > (point.y + static_cast<int32_t>(height.value)))
    {
        return false;
    }

    return true;
}

auto Rect::contains_point(const Point &other) const noexcept -> bool
{
    return (point.x <= other.x && other.x <= (point.x + static_cast<int32_t>(width.value))) &&
           (point.y <= other.y && other.y <= (point.y + static_cast<int32_t>(height.value)));
}
