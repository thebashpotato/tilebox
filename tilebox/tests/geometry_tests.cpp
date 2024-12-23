#include <tilebox/geometry.hpp>

#include <gtest/gtest.h>

#include <cmath>
#include <cstdint>
#include <limits>

using namespace Tilebox;

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
    const auto [tl, tr, br, bl] = r.Corners();

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

    ASSERT_EQ(r.Midpoint(), p);
}

TEST(TileboxCoreGeometryTestSuite, VerifyMidpointWorksBothOdd)
{
    const Rect r(Width(11), Height(21));
    const Point p(X(5), Y(10));

    ASSERT_EQ(r.Midpoint(), p);
}

TEST(TileboxCoreGeometryTestSuite, VerifyMidpointWorksWidthEven)
{
    const Rect r(Width(10), Height(21));
    const Point p(X(5), Y(10));

    ASSERT_EQ(r.Midpoint(), p);
}

TEST(TileboxCoreGeometryTestSuite, VerifyMidpointWorksHeightEven)
{
    const Rect r(Width(11), Height(20));
    const Point p(X(5), Y(10));

    ASSERT_EQ(r.Midpoint(), p);
}

TEST(TileboxCoreGeometryTestSuite, VerifyShinkInWorksSmallBorder)
{
    constexpr std::uint32_t border = 1;
    const Width w(8);
    const Height h(18);

    const Rect r(Width(10), Height(20));
    const auto shrunk = r.ShrinkIn(border);

    ASSERT_EQ(shrunk, Rect(Point(X(r.point.x), Y(r.point.y)), w, h));
}

TEST(TileboxCoreGeometryTestSuite, VerifyShinkInWorksMassiveBorder)
{
    constexpr std::uint32_t border = 1000;
    const Width w(1);
    const Height h(1);

    const Rect r(Width(10), Height(20));
    const auto shrunk = r.ShrinkIn(border);

    ASSERT_EQ(shrunk, Rect(Point(X(r.point.x), Y(r.point.y)), w, h));
}

TEST(TileboxCoreGeometryTestSuite, VerifyShinkInWorksBorderHalfOfWidth)
{
    constexpr std::uint32_t border = 5;
    const Width w(1);
    const Height h(10);

    const Rect r(Width(10), Height(20));
    const auto shrunk = r.ShrinkIn(border);

    ASSERT_EQ(shrunk, Rect(Point(X(r.point.x), Y(r.point.y)), w, h));
}

TEST(TileboxCoreGeometryTestSuite, VerifyShinkInWorksBorderHalfOfHeight)
{
    constexpr std::uint32_t border = 5;
    const Width w(10);
    const Height h(1);

    const Rect r(Width(20), Height(10));
    const auto shrunk = r.ShrinkIn(border);

    ASSERT_EQ(shrunk, Rect(Point(X(r.point.x), Y(r.point.y)), w, h));
}

TEST(TileboxCoreGeometryTestSuite, VerifyScaleWidthUp)
{
    constexpr std::double_t factor = 1.5;
    const Point coordinates(X(10), Y(20));
    const Height height(40);

    const Rect expected(coordinates, Width(45), height);
    const Rect r(coordinates, Width(30), height);

    ASSERT_EQ(r.ScaleWidth(factor), expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyScaleWidthDown)
{
    constexpr std::double_t factor = 0.5;
    const Point coordinates(X(10), Y(20));
    const Height height(40);

    const Rect expected(coordinates, Width(15), height);
    const Rect r(coordinates, Width(30), height);

    ASSERT_EQ(r.ScaleWidth(factor), expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyScaleWidthUnchanged)
{
    constexpr std::double_t factor = 1.0;
    const Point coordinates(X(10), Y(20));
    const Height height(40);

    const Rect expected(coordinates, Width(30), height);
    const Rect r(coordinates, Width(30), height);

    ASSERT_EQ(r.ScaleWidth(factor), expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyScaleHeightUp)
{
    constexpr std::double_t factor = 1.5;
    const Point coordinates(X(10), Y(20));
    const Width width(30);

    const Rect expected(coordinates, width, Height(60));
    const Rect r(coordinates, width, Height(40));

    ASSERT_EQ(r.ScaleHeight(factor), expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyScaleHeightDown)
{
    constexpr std::double_t factor = 0.5;
    const Point coordinates(X(10), Y(20));
    const Width width(30);

    const Rect expected(coordinates, width, Height(20));
    const Rect r(coordinates, width, Height(40));

    ASSERT_EQ(r.ScaleHeight(factor), expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyScaleHeightUnchanged)
{
    constexpr std::double_t factor = 1.0;
    const Point coordinates(X(10), Y(20));
    const Width width(30);

    const Rect expected(coordinates, width, Height(40));
    const Rect r(coordinates, width, Height(40));

    ASSERT_EQ(r.ScaleHeight(factor), expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyResizeIncrease)
{
    const X dw(1);
    const Y dh(2);

    const Rect expected(Width(11), Height(22));
    Rect r(Width(10), Height(20));

    r.Resize(dw, dh);

    ASSERT_EQ(r, expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyResizeDecrease)
{

    const X dw(-1);
    const Y dh(-2);

    const Rect expected(Width(9), Height(18));
    Rect r(Width(10), Height(20));

    r.Resize(dw, dh);

    ASSERT_EQ(r, expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyResizeClamp)
{
    const X dw(-100);
    const Y dh(-100);

    const Rect expected(Width(1), Height(1));
    Rect r(Width(10), Height(20));

    r.Resize(dw, dh);

    ASSERT_EQ(r, expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyResizeDecreaseMax)
{
    const X dw(std::numeric_limits<std::int32_t>::max());
    const Y dh(std::numeric_limits<std::int32_t>::min());

    const Rect expected(Width(1), Height(1));
    Rect r(Width(10), Height(20));

    r.Resize(dw, dh);

    ASSERT_EQ(r, expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyRepositionIncrease)
{
    const X dx(1);
    const Y dy(2);

    const Rect expected(Point(X(11), Y(22)), Width(10), Height(20));
    Rect r(Point(X(10), Y(20)), Width(10), Height(20));

    r.Reposition(dx, dy);

    ASSERT_EQ(r, expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyRepositionDecrease)
{
    const X dx(-1);
    const Y dy(-2);

    const Rect expected(Point(X(9), Y(18)), Width(10), Height(20));
    Rect r(Point(X(10), Y(20)), Width(10), Height(20));

    r.Reposition(dx, dy);

    ASSERT_EQ(r, expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyRepositionClamp)
{
    const X dx(-100);
    const Y dy(-100);

    const Rect expected(Point(X(0), Y(0)), Width(10), Height(20));
    Rect r(Point(X(10), Y(20)), Width(10), Height(20));

    r.Reposition(dx, dy);

    ASSERT_EQ(r, expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyDecreaseMax)
{
    const X dx(std::numeric_limits<std::int32_t>::min());
    const Y dy(std::numeric_limits<std::int32_t>::min());

    const Rect expected(Point(X(0), Y(0)), Width(10), Height(20));
    Rect r(Point(X(10), Y(20)), Width(10), Height(20));

    r.Reposition(dx, dy);

    ASSERT_EQ(r, expected);
}

TEST(TileboxCoreGeometryTestSuite, VerifyContainsRect)
{
    const Rect r1(Point(X(10), Y(10)), Width(50), Height(50));
    const Rect r2(Width(100), Height(100));

    ASSERT_TRUE(r2.Contains(r1));
    ASSERT_FALSE(r1.Contains(r2));
}

TEST(TileboxCoreGeometryTestSuite, VerifyContainsPointOutside)
{
    const Rect r(Point(X(10), Y(20)), Width(30), Height(40));
    const Point p;

    ASSERT_FALSE(r.ContainsPoint(p));
}

TEST(TileboxCoreGeometryTestSuite, VerifyContainsPointInside)
{
    const Rect r(Point(X(10), Y(20)), Width(30), Height(40));
    const Point p(X(30), Y(20));

    ASSERT_TRUE(r.ContainsPoint(p));
}

TEST(TileboxCoreGeometryTestSuite, VerifyContainsPointTopLeft)
{
    const Rect r(Point(X(10), Y(20)), Width(30), Height(40));
    const Point p(X(10), Y(20));

    ASSERT_TRUE(r.ContainsPoint(p));
}

TEST(TileboxCoreGeometryTestSuite, VerifyContainsPointTopRight)
{
    const Rect r(Point(X(10), Y(20)), Width(30), Height(40));
    const Point p(X(40), Y(20));

    ASSERT_TRUE(r.ContainsPoint(p));
}

TEST(TileboxCoreGeometryTestSuite, VerifyContainsPointBottomRight)
{
    const Rect r(Point(X(10), Y(20)), Width(30), Height(40));
    const Point p(X(40), Y(60));

    ASSERT_TRUE(r.ContainsPoint(p));
}

TEST(TileboxCoreGeometryTestSuite, VerifyContainsPointBottomLeft)
{
    const Rect r(Point(X(10), Y(20)), Width(30), Height(40));
    const Point p(X(10), Y(60));

    ASSERT_TRUE(r.ContainsPoint(p));
}
