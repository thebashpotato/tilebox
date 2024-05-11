#include <cmath>
#include <cstdint>
#include <gtest/gtest.h>
#include <tilebox-core/geometry.hpp>

using namespace tilebox::core;

TEST(TileboxCoreGeometryTestSuite, VerifyBasicRectConstruction)
{
    auto r = Rect(Width(1), Height(2));

    ASSERT_EQ(r.point.x, X(0));
    ASSERT_EQ(r.point.y, Y(0));
    ASSERT_EQ(r.width, Width(1));
    ASSERT_EQ(r.height, Height(2));
}

TEST(TileboxCoreGeometryTestSuite, VerifyCornersWork)
{
    auto r = Rect(Width(1), Height(2));
    auto [p0, p1, p2, p3] = r.corners();

    auto point0 = Point(X(0), Y(0));
    auto point1 = Point(X(1), Y(0));
    auto point2 = Point(X(1), Y(2));
    auto point3 = Point(X(0), Y(2));

    ASSERT_EQ(p0.x, point0.x);
    ASSERT_EQ(p0.y, point0.y);

    ASSERT_EQ(p1.x, point1.x);
    ASSERT_EQ(p1.y, point1.y);

    ASSERT_EQ(p2.x, point2.x);
    ASSERT_EQ(p2.y, point2.y);

    ASSERT_EQ(p3.x, point3.x);
    ASSERT_EQ(p3.y, point3.y);
}

TEST(TileboxCoreGeometryTestSuite, VerifyMidpointWorksBothEven)
{
    auto r = Rect(Width(10), Height(20));
    auto p = Point(X(5), Y(10));

    ASSERT_EQ(r.midpoint(), p);
}

TEST(TileboxCoreGeometryTestSuite, VerifyMidpointWorksBothOdd)
{
    auto r = Rect(Width(11), Height(21));
    auto p = Point(X(5), Y(10));

    ASSERT_EQ(r.midpoint(), p);
}

TEST(TileboxCoreGeometryTestSuite, VerifyMidpointWorksWidthEven)
{
    auto r = Rect(Width(10), Height(21));
    auto p = Point(X(5), Y(10));

    ASSERT_EQ(r.midpoint(), p);
}

TEST(TileboxCoreGeometryTestSuite, VerifyMidpointWorksHeightEven)
{
    auto r = Rect(Width(11), Height(20));
    auto p = Point(X(5), Y(10));

    ASSERT_EQ(r.midpoint(), p);
}

TEST(TileboxCoreGeometryTestSuite, VerifyShinkInWorksSmallBorder)
{
    const std::uint32_t border = 1;
    const Width w(8);
    const Height h(18);

    auto r = Rect(Width(10), Height(20));
    const auto shrunk = r.shrink_in(border);

    ASSERT_EQ(shrunk, Rect(Point(X(r.point.x), Y(r.point.y)), w, h));
}

TEST(TileboxCoreGeometryTestSuite, VerifyShinkInWorksMassiveBorder)
{
    const std::uint32_t border = 1000;
    const Width w(1);
    const Height h(1);

    auto r = Rect(Width(10), Height(20));
    const auto shrunk = r.shrink_in(border);

    ASSERT_EQ(shrunk, Rect(Point(X(r.point.x), Y(r.point.y)), w, h));
}

TEST(TileboxCoreGeometryTestSuite, VerifyShinkInWorksBorderHalfOfWidth)
{
    const std::uint32_t border = 5;
    const Width w(1);
    const Height h(10);

    auto r = Rect(Width(10), Height(20));
    const auto shrunk = r.shrink_in(border);

    ASSERT_EQ(shrunk, Rect(Point(X(r.point.x), Y(r.point.y)), w, h));
}

TEST(TileboxCoreGeometryTestSuite, VerifyShinkInWorksBorderHalfOfHeight)
{
    const std::uint32_t border = 5;
    const Width w(10);
    const Height h(1);

    auto r = Rect(Width(20), Height(10));
    const auto shrunk = r.shrink_in(border);

    ASSERT_EQ(shrunk, Rect(Point(X(r.point.x), Y(r.point.y)), w, h));
}

TEST(TileboxCoreGeometryTestSuite, VerifyScaleWidthUp)
{
    const std::double_t factor = 1.5;
    const Point coordinates(X(10), Y(20));
    const Height height(40);

    const Rect expected = Rect(coordinates, Width(45), height);
    const Rect r = Rect(coordinates, Width(30), height);

    ASSERT_EQ(r.scale_width(factor), expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyScaleWidthDown)
{
    const std::double_t factor = 0.5;
    const Point coordinates(X(10), Y(20));
    const Height height(40);

    const Rect expected = Rect(coordinates, Width(15), height);
    const Rect r = Rect(coordinates, Width(30), height);

    ASSERT_EQ(r.scale_width(factor), expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyScaleWidthUnchanged)
{
    const std::double_t factor = 1.0;
    const Point coordinates(X(10), Y(20));
    const Height height(40);

    const Rect expected = Rect(coordinates, Width(30), height);
    const Rect r = Rect(coordinates, Width(30), height);

    ASSERT_EQ(r.scale_width(factor), expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyScaleHeightUp)
{
    const std::double_t factor = 1.5;
    const Point coordinates(X(10), Y(20));
    const Width width(30);

    const Rect expected = Rect(coordinates, width, Height(60));
    const Rect r = Rect(coordinates, width, Height(40));

    ASSERT_EQ(r.scale_height(factor), expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyScaleHeightDown)
{
    const std::double_t factor = 0.5;
    const Point coordinates(X(10), Y(20));
    const Width width(30);

    const Rect expected = Rect(coordinates, width, Height(20));
    const Rect r = Rect(coordinates, width, Height(40));

    ASSERT_EQ(r.scale_height(factor), expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyScaleHeightUnchanged)
{
    const std::double_t factor = 1.0;
    const Point coordinates(X(10), Y(20));
    const Width width(30);

    const Rect expected = Rect(coordinates, width, Height(40));
    const Rect r = Rect(coordinates, width, Height(40));

    ASSERT_EQ(r.scale_height(factor), expected);
}
