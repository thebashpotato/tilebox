#pragma once

#include "tilebox-core/utils/attributes.hpp"
#include "tilebox-core/vendor/etl.hpp"
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

class TILEBOX_INTERNAL DeltaOneTag
{
};

class TILEBOX_INTERNAL DeltaTwoTag
{
};

} // namespace TILEBOX_INTERNAL detail

/// @brief Tagged type for uint32_t x coordinate
using X = etl::TaggedFundamental<detail::Xtag, std::uint32_t>;

/// @brief Tagged type for uint32_t y coordinate
using Y = etl::TaggedFundamental<detail::Ytag, std::uint32_t>;

/// @brief Tagged type for uint32_t width size
using Width = etl::TaggedFundamental<detail::WidthTag, std::uint32_t>;

/// @brief Tagged type for uint32_t height size
using Height = etl::TaggedFundamental<detail::HeightTag, std::uint32_t>;

/// @brief Tagged type for int32_t rate of change variable
using DeltaOne = etl::TaggedFundamental<detail::DeltaOneTag, std::int32_t>;

/// @brief Tagged type for int32_t rate of change variable
using DeltaTwo = etl::TaggedFundamental<detail::DeltaTwoTag, std::int32_t>;

/// @brief Represents a 2D coordinate set on a cartesian plane.
///
/// @detail A point cannot be negative, and does not hold signed integers.
class TILEBOX_EXPORT Point
{
  public:
    X x;
    Y y;

  public:
    /// @brief Builds a default Point with (0, 0).
    Point();

    /// @brief Builds a user defined Point.
    Point(X x, Y y);

  public:
    auto operator+(const Point &rhs) const noexcept -> Point;
    auto operator-(const Point &rhs) const noexcept -> Point;
    auto operator==(const Point &rhs) const noexcept -> bool;
    auto operator!=(const Point &rhs) const noexcept -> bool;
};

/// @brief Rect Lays at the heart of an X11 program. Contains all
/// necessary geometric functions for manipulating the layout of a client.
///
/// @details Has member functions for resizing, scaling, and manipulating
/// a rectangle on a X11 cartesian plane via (x, y, width and height).
class TILEBOX_EXPORT Rect
{
  public:
    Point point;
    Width width;
    Height height;

  public:
    /// @brief Builds a default Rect with (0, 0, 1, 1);
    Rect();

    /// @brief Builds a Rect with (0,0) and user defined width and height.
    Rect(Width width, Height height);

    /// @brief Builds a user defined Rect.
    Rect(Point point, Width width, Height height);

  public:
    auto operator+(const Rect &rhs) const noexcept -> Rect;
    auto operator-(const Rect &rhs) const noexcept -> Rect;
    auto operator<(const Rect &rhs) const noexcept -> bool;
    auto operator<=(const Rect &rhs) const noexcept -> bool;
    auto operator>(const Rect &rhs) const noexcept -> bool;
    auto operator>=(const Rect &rhs) const noexcept -> bool;
    auto operator==(const Rect &rhs) const noexcept -> bool;
    auto operator!=(const Rect &rhs) const noexcept -> bool;

  public:
    [[nodiscard]] auto x() const noexcept -> std::int32_t;
    [[nodiscard]] auto y() const noexcept -> std::int32_t;
    [[nodiscard]] auto w() const noexcept -> std::uint32_t;
    [[nodiscard]] auto h() const noexcept -> std::uint32_t;

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

    /// @brief Creates a new Rect with width equal to the factor * the current width
    [[nodiscard]] auto scale_width(const std::double_t factor) const noexcept -> Rect;

    /// @brief Creates a new Rect with height equal to the factor * the current height
    [[nodiscard]] auto scale_height(const std::double_t factor) const noexcept -> Rect;

    /// @brief Mutate the width and height of this Rect by specified deltas.
    ///
    /// @detail Minimum size is clamped at width = 1, height = 1.
    ///
    /// BUG: std::numeric_limits<std::int32_t>::max() is not accounted for and will likely
    ///      cause a crash. However it is unlikely anyones screen would be `2147483647`
    ///      pixels in width or height.
    auto resize(const DeltaOne &dw, const DeltaTwo &dh) noexcept -> void;

    /// @brief Mutate the position of this Rect by specified deltas
    ///
    /// @detail Minimum (x, y) coordinates are clamped at x = 0, y = 0.
    auto reposition(const DeltaOne &dx, const DeltaTwo &dy) noexcept -> void;

    /// @brief Check whether this Rect contains `rhs` as a sub-Rect
    [[nodiscard]] auto contains(const Rect &rhs) const noexcept -> bool;

    /// @brief Check whether this Rect contains 'rhs'
    [[nodiscard]] auto contains_point(const Point &rhs) const noexcept -> bool;
};

} // namespace tilebox::core
