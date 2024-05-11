#pragma once

#include "tilebox-core/utils/attributes.hpp"
#include "tilebox-core/utils/tagged_fundamental.hpp"
#include <cmath>
#include <cstdint>
#include <tuple>

namespace tilebox::core
{

namespace TILEBOX_INTERNAL detail
{

class TILEBOX_INTERNAL Xtag
{
};

class TILEBOX_INTERNAL Ytag
{
};

class TILEBOX_INTERNAL WidthTag
{
};

class TILEBOX_INTERNAL HeightTag
{
};

} // namespace TILEBOX_INTERNAL detail

using X = TaggedFundamental<detail::Xtag, std::uint32_t>;
using Y = TaggedFundamental<detail::Ytag, std::uint32_t>;
using Width = TaggedFundamental<detail::WidthTag, std::uint32_t>;
using Height = TaggedFundamental<detail::HeightTag, std::uint32_t>;

class TILEBOX_EXPORT Point
{
  public:
    X x;
    Y y;

  public:
    Point(X x, Y y);

    auto operator+(const Point &other) const noexcept -> Point;
    auto operator-(const Point &other) const noexcept -> Point;
};

class TILEBOX_EXPORT Rect
{
  public:
    Point point;
    Width width;
    Height height;

  public:
    Rect(Point point, Width width, Height height);

    auto operator+(const Rect &other) const noexcept -> Rect;
    auto operator-(const Rect &other) const noexcept -> Rect;
    auto operator<(const Rect &other) const noexcept -> bool;
    auto operator<=(const Rect &other) const noexcept -> bool;
    auto operator>(const Rect &other) const noexcept -> bool;
    auto operator>=(const Rect &other) const noexcept -> bool;

    /// @brief The four corners of this Rect in Point form return in clockwise order
    /// from the top left corner.
    ///
    /// @detail It starts from the top left because X11 identifies the top left
    /// corner of a screen as the origin (0,0). Basically, the screen is just the 2nd quadrant of a
    /// cartesian plane inverted.
    [[nodiscard]] auto corners() const noexcept -> std::tuple<Point, Point, Point, Point>;

    /// @brief The midpoint of this rectangle.
    ///
    /// @detail Odd side lengths will lead to a truncated point towards to top left corner
    /// in order to maintain integer coordinates.
    [[nodiscard]] auto midpoint() const noexcept -> Point;

    /// @brief Creats a new Rect with a shrunken width and height by the given pixel border,
    /// maintaining the current (x,y) coordinates.
    ///
    /// @detail The resulting Rect will always have a minimum
    /// width and height of 1.
    [[nodiscard]] auto shrink_in(const std::uint32_t border) const noexcept -> Rect;

    /// @brief Crates a new Rect with width equal to the factor * the current width
    [[nodiscard]] auto scale_width(const std::double_t factor) const noexcept -> Rect;

    /// @brief Crates a new Rect with height equal to the factor * the current width
    [[nodiscard]] auto scale_height(const std::double_t factor) const noexcept -> Rect;

    /// @brief Update the width and height of this Rect by specified deltas.
    ///
    /// @detail Minimum size is clamped at 1x1
    auto resize(const std::int32_t dw, const std::int32_t dh) noexcept -> void;

    /// @brief Update the position of this Rect by specified deltas
    ///
    /// @detail Minimum (x, y) coordinates are clamped at (0,0)
    auto reposition(const std::int32_t dx, const std::int32_t dy) noexcept -> void;

    /// @brief Check whether this Rect contains `other` as a sub-Rect
    [[nodiscard]] auto contains(const Rect &other) const noexcept -> bool;

    /// @brief Check whether this Rect contains 'other_point'
    [[nodiscard]] auto contains_point(const Point &other_point) const noexcept -> bool;
};

} // namespace tilebox::core
