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

class Tilebox_X11FontFixture : public X11DisplayFixture
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

TEST_F(Tilebox_X11FontFixture, VerifyMoveConstructor)
{
    auto *raw_before = font.xftfont();
    auto height_before = font.height().value;

    const Ui::X11Font moved = std::move(font);

    EXPECT_EQ(moved.xftfont(), raw_before);
    EXPECT_EQ(moved.height().value, height_before);
    EXPECT_TRUE(moved.IsValid());
    EXPECT_FALSE(font.IsValid()); // moved-from should be inert
    // EXPECT_TRUE(moved.ContainsChar(display, U'A')); // basic smoke check still works
}

// TEST_F(Tilebox_X11FontFixture, VerifyMoveAssignment)
// {
//     // Prepare a different target to assign into
//     auto other_res = X11Font::TryCreate(display, "monospace:size=10");
//     ASSERT_TRUE(other_res.is_ok());
//     X11Font target = std::move(*other_res.ok());
//     auto *old_target_raw = target.xftfont();
//
//     // Source is the fixture's 12pt monospace
//     auto *src_raw = font.xftfont();
//     auto src_height = font.height().value;
//
//     target = std::move(font);
//
//     EXPECT_EQ(target.xftfont(), src_raw);
//     EXPECT_EQ(target.height().value, src_height);
//     EXPECT_NE(target.xftfont(), old_target_raw); // really replaced the old font
//     EXPECT_FALSE(font.IsValid());                // source was moved-from
//     EXPECT_TRUE(target.ContainsChar(display, U'A'));
// }

} // namespace Tilebox
