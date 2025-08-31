#include <gtest/gtest.h>

#include <type_traits>
#include <utility>

#include "display_fixture.hpp"
#include "ui/font.hpp"

namespace Tilebox
{

static_assert(!std::is_copy_constructible_v<Ui::X11Font>, "X11Font must be non-copyable");
static_assert(!std::is_copy_assignable_v<Ui::X11Font>, "X11Font must be non-copyable");
static_assert(std::is_move_constructible_v<Ui::X11Font>, "X11Font must be move-constructible");
static_assert(std::is_move_assignable_v<Ui::X11Font>, "X11Font must be move-assignable");

class Tilebox_X11FontTestSuite : public X11DisplayFixture
{
  protected:
    void SetUp() override
    {
        SetupDisplay();
        auto font_res = Ui::X11Font::TryCreate(this->dpy(), "monospace:size=12");
        ASSERT_TRUE(font_res.is_ok()) << "Font creation failed: " << font_res.err()->info();
        font = std::move(*font_res.ok());
        ASSERT_TRUE(font.IsValid());
    }

    void TearDown() override
    {
        TeardownDisplay();
    }

    Ui::X11Font font;
};

// ---------------- Move Semantics ----------------

TEST_F(Tilebox_X11FontTestSuite, VerifyMoveConstructor)
{
    auto *raw_before = font.font();
    const auto height_before = font.height().value;

    const Ui::X11Font moved = std::move(font);

    EXPECT_EQ(moved.font(), raw_before);
    EXPECT_EQ(moved.height().value, height_before);
    EXPECT_TRUE(moved.IsValid());
    EXPECT_FALSE(font.IsValid());                   // moved-from should be inert
    EXPECT_TRUE(moved.ContainsChar(display, U'A')); // basic smoke check still works
}

TEST_F(Tilebox_X11FontTestSuite, VerifyMoveAssignment)
{
    // Prepare a different target to assign into
    auto other_res = Ui::X11Font::TryCreate(display, "monospace:size=10");
    ASSERT_TRUE(other_res.is_ok());
    Ui::X11Font target = std::move(*other_res.ok());
    auto *old_target_raw = target.font();

    // Source is the fixture's 12pt monospace
    auto *src_raw = font.font();
    const auto src_height = font.height().value;

    target = std::move(font);

    EXPECT_EQ(target.font(), src_raw);
    EXPECT_EQ(target.height().value, src_height);
    EXPECT_NE(target.font(), old_target_raw); // really replaced the old font
    EXPECT_FALSE(font.IsValid());             // source was moved-from
    EXPECT_TRUE(target.ContainsChar(display, U'A'));
}

// ---------------- ContainsChar ----------------

TEST_F(Tilebox_X11FontTestSuite, ContainsChar_AsciiTrue)
{
    EXPECT_TRUE(font.ContainsChar(display, U'A'));
    EXPECT_TRUE(font.ContainsChar(display, U'z'));
}

TEST_F(Tilebox_X11FontTestSuite, ContainsChar_NonAscii_Smoke)
{
    // the default font that we load is monospace, it supports the copyright Unicode symbol
    EXPECT_TRUE(font.ContainsChar(display, U'©'));

    // But it doesn't support the check mark Unicode symbol
    EXPECT_FALSE(font.ContainsChar(display, U'✓'));
}

// ---------------- GetTextExtents ----------------

TEST_F(Tilebox_X11FontTestSuite, GetTextExtents_EmptyStringFails)
{
    const auto res = font.GetTextExtents(display, "");
    EXPECT_TRUE(res.is_err());
}

TEST_F(Tilebox_X11FontTestSuite, GetTextExtents_BasicAndMonotonic)
{
    const auto a = font.GetTextExtents(display, "Hello");
    ASSERT_TRUE(a.is_ok()) << a.err()->info();
    const auto b = font.GetTextExtents(display, "HelloHello");
    ASSERT_TRUE(b.is_ok()) << b.err()->info();

    auto [aw, ah] = *a.ok();
    auto [bw, bh] = *b.ok();

    // The heights should match since its all the monospace font
    EXPECT_EQ(ah, font.height().value);
    EXPECT_EQ(bh, font.height().value);

    // The widths should be different, since longer text
    EXPECT_LT(aw, bw);
}

// ---------------- FontMatch ----------------

TEST_F(Tilebox_X11FontTestSuite, FontMatch_ReturnsPatternAndConstructsFont)
{
    // Definitely found in monospace primary font
    auto pat_res = font.FontMatch(display, U'A');
    ASSERT_TRUE(pat_res.is_ok()) << pat_res.err()->info();

    FcPattern *pat = *pat_res.ok();
    auto f2_res = Ui::X11Font::TryCreate(display, pat);
    ASSERT_TRUE(f2_res.is_ok()) << f2_res.err()->info();

    const Ui::X11Font f2 = std::move(*f2_res.ok());
    EXPECT_TRUE(f2.IsValid());
    EXPECT_TRUE(f2.ContainsChar(display, U'A'));
}

// ---------------- FallbackForChar ----------------

TEST_F(Tilebox_X11FontTestSuite, FallbackForChar_ProvidesFontThatHandlesGlyph)
{
    // Pick a non-ASCII glyph that often requires fallback in monospace fonts.
    constexpr char32_t ch = U'✓'; // U+2713 CHECK MARK (usually in Symbol/Noto/etc.)
    const bool primary_has = font.ContainsChar(display, ch);

    auto fb_res = font.FallbackForChar(display, ch);
    if (fb_res.is_err())
    {
        // If the system truly lacks any font for this glyph, skip to avoid flakiness.
        if (!primary_has)
        {
            GTEST_SKIP() << "No system fallback available for U+2713 on this machine.";
        }
        // If primary has it, FontMatch/TryCreate should still succeed (may return primary again).
        FAIL() << "FallbackForChar failed even though primary appears to contain the glyph.";
    }

    const Ui::X11Font fb = std::move(*fb_res.ok());
    EXPECT_TRUE(fb.IsValid());
    EXPECT_TRUE(fb.ContainsChar(display, ch));
}

} // namespace Tilebox
