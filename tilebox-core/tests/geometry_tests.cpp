#include <cmath>
#include <cstdint>
#include <gtest/gtest.h>
#include <limits>
#include <tilebox-core/geometry.hpp>

using namespace tilebox::core;

TEST(TileboxCoreGeometryTestSuite, VerifyBasicRectConstruction)
{
    const Rect r(Width(1), Height(2));

    ASSERT_EQ(r.point.x, X(0));
    ASSERT_EQ(r.point.y, Y(0));
    ASSERT_EQ(r.width, Width(1));
    ASSERT_EQ(r.height, Height(2));
}

TEST(TileboxCoreGeometryTestSuite, VerifyCornersWork)
{
    const Rect r(Width(1), Height(2));
    const auto [tl, tr, br, bl] = r.corners();

    const Point expected_tl(X(0), Y(0));
    const Point expected_tr(X(1), Y(0));
    const Point expected_br(X(1), Y(2));
    const Point expected_bl(X(0), Y(2));

    ASSERT_EQ(tl.x, expected_tl.x);
    ASSERT_EQ(tl.y, expected_tl.y);

    ASSERT_EQ(tr.x, expected_tr.x);
    ASSERT_EQ(tr.y, expected_tr.y);

    ASSERT_EQ(br.x, expected_br.x);
    ASSERT_EQ(br.y, expected_br.y);

    ASSERT_EQ(bl.x, expected_bl.x);
    ASSERT_EQ(bl.y, expected_bl.y);
}

TEST(TileboxCoreGeometryTestSuite, VerifyMidpointWorksBothEven)
{
    const Rect r(Width(10), Height(20));
    const Point p(X(5), Y(10));

    ASSERT_EQ(r.midpoint(), p);
}

TEST(TileboxCoreGeometryTestSuite, VerifyMidpointWorksBothOdd)
{
    const Rect r(Width(11), Height(21));
    const Point p(X(5), Y(10));

    ASSERT_EQ(r.midpoint(), p);
}

TEST(TileboxCoreGeometryTestSuite, VerifyMidpointWorksWidthEven)
{
    const Rect r(Width(10), Height(21));
    const Point p(X(5), Y(10));

    ASSERT_EQ(r.midpoint(), p);
}

TEST(TileboxCoreGeometryTestSuite, VerifyMidpointWorksHeightEven)
{
    const Rect r(Width(11), Height(20));
    const Point p(X(5), Y(10));

    ASSERT_EQ(r.midpoint(), p);
}

TEST(TileboxCoreGeometryTestSuite, VerifyShinkInWorksSmallBorder)
{
    const std::uint32_t border = 1;
    const Width w(8);
    const Height h(18);

    const Rect r(Width(10), Height(20));
    const auto shrunk = r.shrink_in(border);

    ASSERT_EQ(shrunk, Rect(Point(X(r.point.x), Y(r.point.y)), w, h));
}

TEST(TileboxCoreGeometryTestSuite, VerifyShinkInWorksMassiveBorder)
{
    const std::uint32_t border = 1000;
    const Width w(1);
    const Height h(1);

    const Rect r(Width(10), Height(20));
    const auto shrunk = r.shrink_in(border);

    ASSERT_EQ(shrunk, Rect(Point(X(r.point.x), Y(r.point.y)), w, h));
}

TEST(TileboxCoreGeometryTestSuite, VerifyShinkInWorksBorderHalfOfWidth)
{
    const std::uint32_t border = 5;
    const Width w(1);
    const Height h(10);

    const Rect r(Width(10), Height(20));
    const auto shrunk = r.shrink_in(border);

    ASSERT_EQ(shrunk, Rect(Point(X(r.point.x), Y(r.point.y)), w, h));
}

TEST(TileboxCoreGeometryTestSuite, VerifyShinkInWorksBorderHalfOfHeight)
{
    const std::uint32_t border = 5;
    const Width w(10);
    const Height h(1);

    const Rect r(Width(20), Height(10));
    const auto shrunk = r.shrink_in(border);

    ASSERT_EQ(shrunk, Rect(Point(X(r.point.x), Y(r.point.y)), w, h));
}

TEST(TileboxCoreGeometryTestSuite, VerifyScaleWidthUp)
{
    const std::double_t factor = 1.5;
    const Point coordinates(X(10), Y(20));
    const Height height(40);

    const Rect expected(coordinates, Width(45), height);
    const Rect r(coordinates, Width(30), height);

    ASSERT_EQ(r.scale_width(factor), expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyScaleWidthDown)
{
    const std::double_t factor = 0.5;
    const Point coordinates(X(10), Y(20));
    const Height height(40);

    const Rect expected(coordinates, Width(15), height);
    const Rect r(coordinates, Width(30), height);

    ASSERT_EQ(r.scale_width(factor), expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyScaleWidthUnchanged)
{
    const std::double_t factor = 1.0;
    const Point coordinates(X(10), Y(20));
    const Height height(40);

    const Rect expected(coordinates, Width(30), height);
    const Rect r(coordinates, Width(30), height);

    ASSERT_EQ(r.scale_width(factor), expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyScaleHeightUp)
{
    const std::double_t factor = 1.5;
    const Point coordinates(X(10), Y(20));
    const Width width(30);

    const Rect expected(coordinates, width, Height(60));
    const Rect r(coordinates, width, Height(40));

    ASSERT_EQ(r.scale_height(factor), expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyScaleHeightDown)
{
    const std::double_t factor = 0.5;
    const Point coordinates(X(10), Y(20));
    const Width width(30);

    const Rect expected(coordinates, width, Height(20));
    const Rect r(coordinates, width, Height(40));

    ASSERT_EQ(r.scale_height(factor), expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyScaleHeightUnchanged)
{
    const std::double_t factor = 1.0;
    const Point coordinates(X(10), Y(20));
    const Width width(30);

    const Rect expected(coordinates, width, Height(40));
    const Rect r(coordinates, width, Height(40));

    ASSERT_EQ(r.scale_height(factor), expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyResizeIncrease)
{
    const DeltaOne dw(1);
    const DeltaTwo dh(2);

    const Rect expected(Width(11), Height(22));
    Rect r(Width(10), Height(20));

    r.resize(dw, dh);

    ASSERT_EQ(r, expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyResizeDecrease)
{

    const DeltaOne dw(-1);
    const DeltaTwo dh(-2);

    const Rect expected(Width(9), Height(18));
    Rect r(Width(10), Height(20));

    r.resize(dw, dh);

    ASSERT_EQ(r, expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyResizeClamp)
{
    const DeltaOne dw(-100);
    const DeltaTwo dh(-100);

    const Rect expected(Width(1), Height(1));
    Rect r(Width(10), Height(20));

    r.resize(dw, dh);

    ASSERT_EQ(r, expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyResizeDecreaseMax)
{
    const DeltaOne dw(std::numeric_limits<std::int32_t>::max());
    const DeltaTwo dh(std::numeric_limits<std::int32_t>::min());

    const Rect expected(Width(1), Height(1));
    Rect r(Width(10), Height(20));

    r.resize(dw, dh);

    ASSERT_EQ(r, expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyRepositionIncrease)
{
    const DeltaOne dx(1);
    const DeltaTwo dy(2);

    const Rect expected(Point(X(11), Y(22)), Width(10), Height(20));
    Rect r(Point(X(10), Y(20)), Width(10), Height(20));

    r.reposition(dx, dy);

    ASSERT_EQ(r, expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyRepositionDecrease)
{
    const DeltaOne dx(-1);
    const DeltaTwo dy(-2);

    const Rect expected(Point(X(9), Y(18)), Width(10), Height(20));
    Rect r(Point(X(10), Y(20)), Width(10), Height(20));

    r.reposition(dx, dy);

    ASSERT_EQ(r, expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyRepositionClamp)
{
    const DeltaOne dx(-100);
    const DeltaTwo dy(-100);

    const Rect expected(Point(X(0), Y(0)), Width(10), Height(20));
    Rect r(Point(X(10), Y(20)), Width(10), Height(20));

    r.reposition(dx, dy);

    ASSERT_EQ(r, expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyDecreaseMax)
{
    const DeltaOne dx(std::numeric_limits<std::int32_t>::min());
    const DeltaTwo dy(std::numeric_limits<std::int32_t>::min());

    const Rect expected(Point(X(0), Y(0)), Width(10), Height(20));
    Rect r(Point(X(10), Y(20)), Width(10), Height(20));

    r.reposition(dx, dy);

    ASSERT_EQ(r, expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyContainsRect)
{
    const Rect r1(Point(X(10), Y(10)), Width(50), Height(50));
    const Rect r2(Width(100), Height(100));

    ASSERT_TRUE(r2.contains(r1));
    ASSERT_FALSE(r1.contains(r2));
}

TEST(TileboxCoreGeometryTestSuite, VerifyContainsPointOutside)
{
    const Rect r(Point(X(10), Y(20)), Width(30), Height(40));
    const Point p;

    ASSERT_FALSE(r.contains_point(p));
}

TEST(TileboxCoreGeometryTestSuite, VerifyContainsPointInside)
{
    const Rect r(Point(X(10), Y(20)), Width(30), Height(40));
    const Point p(X(30), Y(20));

    ASSERT_TRUE(r.contains_point(p));
}

TEST(TileboxCoreGeometryTestSuite, VerifyContainsPointTopLeft)
{
    const Rect r(Point(X(10), Y(20)), Width(30), Height(40));
    const Point p(X(10), Y(20));

    ASSERT_TRUE(r.contains_point(p));
}

TEST(TileboxCoreGeometryTestSuite, VerifyContainsPointTopRight)
{
    const Rect r(Point(X(10), Y(20)), Width(30), Height(40));
    const Point p(X(40), Y(20));

    ASSERT_TRUE(r.contains_point(p));
}

TEST(TileboxCoreGeometryTestSuite, VerifyContainsPointBottomRight)
{
    const Rect r(Point(X(10), Y(20)), Width(30), Height(40));
    const Point p(X(40), Y(60));

    ASSERT_TRUE(r.contains_point(p));
}

TEST(TileboxCoreGeometryTestSuite, VerifyContainsPointBottomLeft)
{
    const Rect r(Point(X(10), Y(20)), Width(30), Height(40));
    const Point p(X(10), Y(60));

    ASSERT_TRUE(r.contains_point(p));
}
