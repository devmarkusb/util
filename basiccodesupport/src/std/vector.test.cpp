// VLD detects lots of memory leaks in this file (unclear)
#include "ul/std/vector.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

class VectorMoveRangeTest : public ::testing::Test {
protected:
    std::vector<char> v7_;

    void SetUp() override {
        // abcdefg
        v7_.push_back('a');
        v7_.push_back('b');
        v7_.push_back('c');
        v7_.push_back('d');
        v7_.push_back('e');
        v7_.push_back('f');
        v7_.push_back('g');
    }

    void TearDown() override {
    }

    static std::string vec2_str(const std::vector<char>& v) {
        std::string s(v.begin(), v.end());
        return s;
    }
};

// NOLINTBEGIN

TEST_F(VectorMoveRangeTest, Move1FromStartMiddleEndToLeftOrRight) {
    ul::vector::move_range(0, 1, 2, v7_);
    EXPECT_EQ("bacdefg", vec2_str(v7_));
    ul::vector::move_range(3, 1, 1, v7_);
    EXPECT_EQ("bdacefg", vec2_str(v7_));
    ul::vector::move_range(6, 1, 5, v7_);
    EXPECT_EQ("bdacegf", vec2_str(v7_));
    ul::vector::move_range(3, 1, 5, v7_);
    EXPECT_EQ("bdaecgf", vec2_str(v7_));
}

TEST_F(VectorMoveRangeTest, Move2FromStartMiddleEndToLeftOrRight) {
    ul::vector::move_range(0, 2, 3, v7_);
    EXPECT_EQ("cabdefg", vec2_str(v7_));
    ul::vector::move_range(3, 2, 1, v7_);
    EXPECT_EQ("cdeabfg", vec2_str(v7_));
    ul::vector::move_range(5, 2, 4, v7_);
    EXPECT_EQ("cdeafgb", vec2_str(v7_));
    ul::vector::move_range(3, 2, 6, v7_);
    EXPECT_EQ("cdegafb", vec2_str(v7_));
}

TEST_F(VectorMoveRangeTest, Move1FromMiddleToStartEndAndBeyondEnd) {
    ul::vector::move_range(3, 1, 0, v7_);
    EXPECT_EQ("dabcefg", vec2_str(v7_));
    ul::vector::move_range(3, 1, 7, v7_);
    EXPECT_EQ("dabefgc", vec2_str(v7_));
    ul::vector::move_range(3, 1, 8, v7_);
    EXPECT_EQ("dabfgce", vec2_str(v7_));
}

TEST_F(VectorMoveRangeTest, Move1TooLittle) {
    ul::vector::move_range(1, 1, 1, v7_);
    EXPECT_EQ("abcdefg", vec2_str(v7_));
    ul::vector::move_range(1, 1, 2, v7_);
    EXPECT_EQ("abcdefg", vec2_str(v7_));
}

TEST_F(VectorMoveRangeTest, Move2TooLittle) {
    ul::vector::move_range(1, 2, 1, v7_);
    EXPECT_EQ("abcdefg", vec2_str(v7_));
    ul::vector::move_range(1, 2, 2, v7_);
    EXPECT_EQ("abcdefg", vec2_str(v7_));
    ul::vector::move_range(1, 2, 3, v7_);
    EXPECT_EQ("abcdefg", vec2_str(v7_));
}

TEST_F(VectorMoveRangeTest, MoveLargeRange) {
    ul::vector::move_range(1, 6, 0, v7_);
    EXPECT_EQ("bcdefga", vec2_str(v7_));
    ul::vector::move_range(2, 5, 1, v7_);
    EXPECT_EQ("bdefgac", vec2_str(v7_));
}

TEST_F(VectorMoveRangeTest, MoveFullRange) {
    ul::vector::move_range(0, 7, 0, v7_);
    EXPECT_EQ("abcdefg", vec2_str(v7_));
    ul::vector::move_range(0, 7, 8, v7_);
    EXPECT_EQ("abcdefg", vec2_str(v7_));
}

TEST_F(VectorMoveRangeTest, MoveTooLargeRange) {
    ul::vector::move_range(0, 8, 1, v7_);
    EXPECT_EQ("abcdefg", vec2_str(v7_));
    ul::vector::move_range(0, 8, 9, v7_);
    EXPECT_EQ("abcdefg", vec2_str(v7_));
}

TEST_F(VectorMoveRangeTest, MoveNothing) {
    ul::vector::move_range(2, 0, 4, v7_);
    EXPECT_EQ("abcdefg", vec2_str(v7_));
}

// NOLINTEND
