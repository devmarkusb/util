// VLD detects lots of memory leaks in this file (unclear)
#include "Toolib/std/vector.h"
#include "gtest/gtest.h"


class VectorMoveRangeTest : public ::testing::Test
{
protected:
    std::vector<char> m_v7;

    virtual void SetUp()
    {
        // abcdefg
        m_v7.push_back('a');
        m_v7.push_back('b');
        m_v7.push_back('c');
        m_v7.push_back('d');
        m_v7.push_back('e');
        m_v7.push_back('f');
        m_v7.push_back('g');
    }

    virtual void TearDown() {}

    std::string Vec2Str(const std::vector<char>& v)
    {
        std::string s(v.begin(), v.end());
        return s;
    }
};


TEST_F(VectorMoveRangeTest, Move1FromStartMiddleEndToLeftOrRight)
{
    too::vector::move_range(0, 1, 2, m_v7);
    EXPECT_EQ("bacdefg", Vec2Str(m_v7));
    too::vector::move_range(3, 1, 1, m_v7);
    EXPECT_EQ("bdacefg", Vec2Str(m_v7));
    too::vector::move_range(6, 1, 5, m_v7);
    EXPECT_EQ("bdacegf", Vec2Str(m_v7));
    too::vector::move_range(3, 1, 5, m_v7);
    EXPECT_EQ("bdaecgf", Vec2Str(m_v7));
}

TEST_F(VectorMoveRangeTest, Move2FromStartMiddleEndToLeftOrRight)
{
    too::vector::move_range(0, 2, 3, m_v7);
    EXPECT_EQ("cabdefg", Vec2Str(m_v7));
    too::vector::move_range(3, 2, 1, m_v7);
    EXPECT_EQ("cdeabfg", Vec2Str(m_v7));
    too::vector::move_range(5, 2, 4, m_v7);
    EXPECT_EQ("cdeafgb", Vec2Str(m_v7));
    too::vector::move_range(3, 2, 6, m_v7);
    EXPECT_EQ("cdegafb", Vec2Str(m_v7));
}

TEST_F(VectorMoveRangeTest, Move1FromMiddleToStartEndAndBeyondEnd)
{
    too::vector::move_range(3, 1, 0, m_v7);
    EXPECT_EQ("dabcefg", Vec2Str(m_v7));
    too::vector::move_range(3, 1, 7, m_v7);
    EXPECT_EQ("dabefgc", Vec2Str(m_v7));
    too::vector::move_range(3, 1, 8, m_v7);
    EXPECT_EQ("dabfgce", Vec2Str(m_v7));
}

TEST_F(VectorMoveRangeTest, Move1TooLittle)
{
    too::vector::move_range(1, 1, 1, m_v7);
    EXPECT_EQ("abcdefg", Vec2Str(m_v7));
    too::vector::move_range(1, 1, 2, m_v7);
    EXPECT_EQ("abcdefg", Vec2Str(m_v7));
}

TEST_F(VectorMoveRangeTest, Move2TooLittle)
{
    too::vector::move_range(1, 2, 1, m_v7);
    EXPECT_EQ("abcdefg", Vec2Str(m_v7));
    too::vector::move_range(1, 2, 2, m_v7);
    EXPECT_EQ("abcdefg", Vec2Str(m_v7));
    too::vector::move_range(1, 2, 3, m_v7);
    EXPECT_EQ("abcdefg", Vec2Str(m_v7));
}

TEST_F(VectorMoveRangeTest, MoveLargeRange)
{
    too::vector::move_range(1, 6, 0, m_v7);
    EXPECT_EQ("bcdefga", Vec2Str(m_v7));
    too::vector::move_range(2, 5, 1, m_v7);
    EXPECT_EQ("bdefgac", Vec2Str(m_v7));
}

TEST_F(VectorMoveRangeTest, MoveFullRange)
{
    too::vector::move_range(0, 7, 0, m_v7);
    EXPECT_EQ("abcdefg", Vec2Str(m_v7));
    too::vector::move_range(0, 7, 8, m_v7);
    EXPECT_EQ("abcdefg", Vec2Str(m_v7));
}

TEST_F(VectorMoveRangeTest, MoveTooLargeRange)
{
    too::vector::move_range(0, 8, 1, m_v7);
    EXPECT_EQ("abcdefg", Vec2Str(m_v7));
    too::vector::move_range(0, 8, 9, m_v7);
    EXPECT_EQ("abcdefg", Vec2Str(m_v7));
}

TEST_F(VectorMoveRangeTest, MoveNothing)
{
    too::vector::move_range(2, 0, 4, m_v7);
    EXPECT_EQ("abcdefg", Vec2Str(m_v7));
}
