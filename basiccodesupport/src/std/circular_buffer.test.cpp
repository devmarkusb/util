#include "ul/std/circular_buffer.h"
#include "gtest/gtest.h"
#include <string>

namespace ul = mb::ul;

namespace {
constexpr auto some_number_choice{42};
constexpr auto some_other_number_choice{46};
} // namespace

TEST(CircularBuffer, construction) {
    ul::CircularBuffer<int> cb{3};
    EXPECT_EQ(cb.capacity(), 3);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());

    int item{};
    ASSERT_FALSE(cb.try_pop(item));
    EXPECT_EQ(cb.capacity(), 3);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());

    cb.reset();
    EXPECT_EQ(cb.capacity(), 3);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer, push) {
    ul::CircularBuffer<int> cb{3};
    cb.push(some_number_choice);
    EXPECT_EQ(cb.size(), 1);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer, emplace) {
    ul::CircularBuffer<int> cb{3};
    cb.emplace(some_number_choice);
    EXPECT_EQ(cb.size(), 1);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer, reset) {
    ul::CircularBuffer<int> cb{3};
    cb.push(some_number_choice);
    cb.reset();
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer, pop) {
    ul::CircularBuffer<int> cb{3};
    cb.push(some_number_choice);
    int item{};
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_number_choice);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer, front) {
    ul::CircularBuffer<int> cb{3};
    cb.push(some_number_choice);
    int item{};
    ASSERT_TRUE(cb.try_front(item));
    EXPECT_EQ(item, some_number_choice);
    EXPECT_EQ(cb.size(), 1);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(!cb.full());

    ASSERT_TRUE(cb.try_pop(item));
    ASSERT_FALSE(cb.try_front(item));
}

TEST(CircularBuffer, pop_toomuch) {
    ul::CircularBuffer<int> cb{3};
    cb.push(some_number_choice);
    int item{};
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_number_choice);
    ASSERT_FALSE(cb.try_pop(item));
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer, push_until_full_and_pop) {
    ul::CircularBuffer<int> cb{3};
    cb.push(some_number_choice);
    cb.push(some_number_choice + 1);
    cb.push(some_number_choice + 2);
    EXPECT_EQ(cb.size(), 3);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(cb.full());

    int item{};
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_number_choice);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer, push_until_full_and_reset) {
    ul::CircularBuffer<int> cb{3};
    cb.push(some_number_choice);
    cb.push(some_number_choice + 1);
    cb.push(some_number_choice + 2);
    EXPECT_EQ(cb.size(), 3);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(cb.full());

    cb.reset();
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer, push_until_overflow_and_pop) {
    ul::CircularBuffer<int> cb{3};
    cb.push(some_number_choice);
    cb.push(some_number_choice + 1);
    cb.push(some_number_choice + 2);
    cb.push(some_number_choice + 3);
    EXPECT_EQ(cb.size(), 3);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(cb.full());

    int item{};
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_number_choice + 1);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(!cb.full());

    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_number_choice + 2);
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_number_choice + 3);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer, push_overflow_and_pop) {
    ul::CircularBuffer<std::string> cb{3};
    cb.push("42");
    cb.push("43");
    cb.push("44");
    cb.push("45");
    cb.push("46");
    EXPECT_EQ(cb.size(), 3);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(cb.full());

    std::string item;
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_STREQ(item.c_str(), "44");
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(!cb.full());

    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_STREQ(item.c_str(), "45");
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_STREQ(item.c_str(), "46");
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}

//TEST(CircularBuffer, emplace_overflow_and_pop)
//{
//    ul::CircularBuffer<std::string> cb{3};
//    cb.emplace("42");
//    cb.emplace("43");
//    cb.emplace("44");
//    cb.emplace("45");
//    cb.emplace("46");
//    EXPECT_EQ(cb.size(), 3);
//    EXPECT_TRUE(!cb.empty());
//    EXPECT_TRUE(cb.full());
//
//    EXPECT_STREQ(cb.pop().c_str(), "44");
//    EXPECT_TRUE(!cb.empty());
//    EXPECT_TRUE(!cb.full());
//
//    EXPECT_STREQ(cb.pop().c_str(), "45");
//    EXPECT_STREQ(cb.pop().c_str(), "46");
//    EXPECT_EQ(cb.size(), 0);
//    EXPECT_TRUE(cb.empty());
//    EXPECT_TRUE(!cb.full());
//}

TEST(CircularBuffer, doublebuffer_maxsize_2) {
    ul::CircularBuffer<int> cb{2};
    EXPECT_EQ(cb.capacity(), 2);
    EXPECT_EQ(cb.size(), 0);
    cb.push(some_number_choice);
    cb.push(some_number_choice + 1);
    cb.push(some_number_choice + 2);
    cb.push(some_number_choice + 3);
    EXPECT_EQ(cb.size(), 2);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(cb.full());

    int item{};
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_number_choice + 2);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(!cb.full());

    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_number_choice + 3);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());

    cb.push(some_other_number_choice);
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_other_number_choice);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer, pathological_maxsize_1) {
    ul::CircularBuffer<int> cb{1};
    EXPECT_EQ(cb.capacity(), 1);
    EXPECT_EQ(cb.size(), 0);
    cb.push(some_number_choice);
    cb.push(some_number_choice + 1);
    cb.push(some_number_choice + 2);
    cb.push(some_number_choice + 3);
    EXPECT_EQ(cb.size(), 1);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(cb.full());

    int item{};
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_number_choice + 3);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());

    ASSERT_FALSE(cb.try_pop(item));
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());

    cb.push(some_other_number_choice);
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_other_number_choice);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer, other_type) {
    ul::CircularBuffer<std::string> cb{2};
    EXPECT_EQ(cb.capacity(), 2);
    EXPECT_EQ(cb.size(), 0);
    cb.push("s1");
    cb.push("s2");
    cb.push("s3");
    cb.push("s4");
    EXPECT_EQ(cb.size(), 2);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(cb.full());

    std::string item;
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_STREQ(item.c_str(), "s3");
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(!cb.full());

    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_STREQ(item.c_str(), "s4");
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());

    cb.push("s5");
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_STREQ(item.c_str(), "s5");
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}

//####################################################################################################################

TEST(CircularBuffer_compiletime, construction) {
    ul::CircularBuffer<int, 3> cb;
    EXPECT_EQ(cb.capacity(), 3);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());

    int item{};
    ASSERT_FALSE(cb.try_pop(item));
    EXPECT_EQ(cb.capacity(), 3);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());

    cb.reset();
    EXPECT_EQ(cb.capacity(), 3);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer_compiletime, push) {
    ul::CircularBuffer<int, 3> cb;
    cb.push(some_number_choice);
    EXPECT_EQ(cb.size(), 1);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer_compiletime, reset) {
    ul::CircularBuffer<int, 3> cb;
    cb.push(some_number_choice);
    cb.reset();
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer_compiletime, pop) {
    ul::CircularBuffer<int, 3> cb;
    cb.push(some_number_choice);
    int item{};
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_number_choice);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer_compiletime, pop_toomuch) {
    ul::CircularBuffer<int, 3> cb;
    cb.push(some_number_choice);
    int item{};
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_number_choice);
    ASSERT_FALSE(cb.try_pop(item));
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer_compiletime, push_until_full_and_pop) {
    ul::CircularBuffer<int, 3> cb;
    cb.push(some_number_choice);
    cb.push(some_number_choice + 1);
    cb.push(some_number_choice + 2);
    EXPECT_EQ(cb.size(), 3);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(cb.full());

    int item{};
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_number_choice);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer_compiletime, push_until_full_and_reset) {
    ul::CircularBuffer<int, 3> cb;
    cb.push(some_number_choice);
    cb.push(some_number_choice + 1);
    cb.push(some_number_choice + 2);
    EXPECT_EQ(cb.size(), 3);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(cb.full());

    cb.reset();
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer_compiletime, push_until_overflow_and_pop) {
    ul::CircularBuffer<int, 3> cb;
    cb.push(some_number_choice);
    cb.push(some_number_choice + 1);
    cb.push(some_number_choice + 2);
    cb.push(some_number_choice + 3);
    EXPECT_EQ(cb.size(), 3);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(cb.full());

    int item{};
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_number_choice + 1);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(!cb.full());

    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_number_choice + 2);
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_number_choice + 3);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer_compiletime, push_overflow_and_pop) {
    ul::CircularBuffer<std::string, 3> cb;
    cb.push("42");
    cb.push("43");
    cb.push("44");
    cb.push("45");
    cb.push("46");
    EXPECT_EQ(cb.size(), 3);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(cb.full());

    std::string item;
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_STREQ(item.c_str(), "44");
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(!cb.full());

    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_STREQ(item.c_str(), "45");
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_STREQ(item.c_str(), "46");
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer_compiletime, doubleBuffer_compiletime_maxsize_2) {
    ul::CircularBuffer<int, 2> cb;
    EXPECT_EQ(cb.capacity(), 2);
    EXPECT_EQ(cb.size(), 0);
    cb.push(some_number_choice);
    cb.push(some_number_choice + 1);
    cb.push(some_number_choice + 2);
    cb.push(some_number_choice + 3);
    EXPECT_EQ(cb.size(), 2);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(cb.full());

    int item{};
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_number_choice + 2);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(!cb.full());

    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_number_choice + 3);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());

    cb.push(some_other_number_choice);
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_other_number_choice);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer_compiletime, pathological_maxsize_1) {
    ul::CircularBuffer<int, 1> cb;
    EXPECT_EQ(cb.capacity(), 1);
    EXPECT_EQ(cb.size(), 0);
    cb.push(some_number_choice);
    cb.push(some_number_choice + 1);
    cb.push(some_number_choice + 2);
    cb.push(some_number_choice + 3);
    EXPECT_EQ(cb.size(), 1);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(cb.full());

    int item{};
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_number_choice + 3);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());

    ASSERT_FALSE(cb.try_pop(item));
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());

    cb.push(some_other_number_choice);
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_EQ(item, some_other_number_choice);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}

TEST(CircularBuffer_compiletime, other_type) {
    ul::CircularBuffer<std::string, 2> cb;
    EXPECT_EQ(cb.capacity(), 2);
    EXPECT_EQ(cb.size(), 0);
    cb.push("s1");
    cb.push("s2");
    cb.push("s3");
    cb.push("s4");
    EXPECT_EQ(cb.size(), 2);
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(cb.full());

    std::string item;
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_STREQ(item.c_str(), "s3");
    EXPECT_TRUE(!cb.empty());
    EXPECT_TRUE(!cb.full());

    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_STREQ(item.c_str(), "s4");
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());

    cb.push("s5");
    ASSERT_TRUE(cb.try_pop(item));
    EXPECT_STREQ(item.c_str(), "s5");
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(!cb.full());
}
