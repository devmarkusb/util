#include "ul/c_array.h"
#include "gtest/gtest.h"
#include <array>
#include <string>

namespace ul = mb::ul;

TEST(CArrayTest, BasicFunctionality) {
    // Test with integer arrays
    int int_array[5] = {};
    EXPECT_EQ(ul::c_array_length(int_array), 5);
    
    int int_array2[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    EXPECT_EQ(ul::c_array_length(int_array2), 10);
    
    // Test with char arrays
    char char_array[3] = {'a', 'b', 'c'};
    EXPECT_EQ(ul::c_array_length(char_array), 3);
    
    // Test with string literals
    EXPECT_EQ(ul::c_array_length("Hello"), 6); // Including null terminator
    
    // Test with single-element arrays (smallest valid array size)
    int single_element_array[1] = {42};
    EXPECT_EQ(ul::c_array_length(single_element_array), 1);
    
    // Test with multi-dimensional arrays
    int multi_array[3][4] = {};
    EXPECT_EQ(ul::c_array_length(multi_array), 3);
    
    // Test with struct arrays
    struct TestStruct {
        int a;
        double b;
    };
    TestStruct struct_array[2] = {};
    EXPECT_EQ(ul::c_array_length(struct_array), 2);
    
    // Test with const arrays
    const int const_array[5] = {1, 2, 3, 4, 5};
    EXPECT_EQ(ul::c_array_length(const_array), 5);
    
    // Test with volatile arrays
    volatile int volatile_array[3] = {1, 2, 3};
    EXPECT_EQ(ul::c_array_length(volatile_array), 3);
    
    // Test with static arrays
    static int static_array[4] = {1, 2, 3, 4};
    EXPECT_EQ(ul::c_array_length(static_array), 4);
}

// Test that c_array_length works in a loop
TEST(CArrayTest, LoopUsage) {
    int array[5] = {1, 2, 3, 4, 5};
    int sum = 0;
    
    for (size_t i = 0; i < ul::c_array_length(array); ++i) {
        sum += array[i];
    }
    
    EXPECT_EQ(sum, 15);
}

// Test that c_array_length works with template functions
template<typename T, size_t N>
void test_template_function(T (&arr)[N]) {
    EXPECT_EQ(ul::c_array_length(arr), N);
}

TEST(CArrayTest, TemplateFunction) {
    int array[5] = {};
    test_template_function(array);
    
    char char_array[3] = {'a', 'b', 'c'};
    test_template_function(char_array);
}
