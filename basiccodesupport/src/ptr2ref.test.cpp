#include "ul/ptr2ref.h"
#include "gtest/gtest.h"
#include <memory>

namespace ul = mb::ul;

TEST(Ptr2RefTest, BasicFunctionality) {
    // Test with primitive types
    int value = 42;
    int* ptr = &value;
    
    // Test ptr2ref
    int& ref = ul::ptr2ref(ptr);
    EXPECT_EQ(ref, 42);
    ref = 100;
    EXPECT_EQ(value, 100);
    
    // Test ptr2refc
    const int* const_ptr = &value;
    const int& const_ref = ul::ptr2refc(const_ptr);
    EXPECT_EQ(const_ref, 100);
    
    // Test ref2ptr
    int* ptr2 = ul::ref2ptr(value);
    EXPECT_EQ(ptr2, &value);
    EXPECT_EQ(*ptr2, 100);
    
    // Test ref2ptrc
    const int* const_ptr2 = ul::ref2ptrc(value);
    EXPECT_EQ(const_ptr2, &value);
    EXPECT_EQ(*const_ptr2, 100);
}

TEST(Ptr2RefTest, WithObjects) {
    // Test with objects
    struct TestClass {
        int value;
        explicit TestClass(int v) : value(v) {}
    };
    
    TestClass obj{42};
    TestClass* obj_ptr = &obj;
    
    // Test ptr2ref
    TestClass& obj_ref = ul::ptr2ref(obj_ptr);
    EXPECT_EQ(obj_ref.value, 42);
    obj_ref.value = 100;
    EXPECT_EQ(obj.value, 100);
    
    // Test ptr2refc
    const TestClass* const_obj_ptr = &obj;
    const TestClass& const_obj_ref = ul::ptr2refc(const_obj_ptr);
    EXPECT_EQ(const_obj_ref.value, 100);
    
    // Test ref2ptr
    TestClass* obj_ptr2 = ul::ref2ptr(obj);
    EXPECT_EQ(obj_ptr2, &obj);
    EXPECT_EQ(obj_ptr2->value, 100);
    
    // Test ref2ptrc
    const TestClass* const_obj_ptr2 = ul::ref2ptrc(obj);
    EXPECT_EQ(const_obj_ptr2, &obj);
    EXPECT_EQ(const_obj_ptr2->value, 100);
}

TEST(Ptr2RefTest, WithSmartPointers) {
    // Test with smart pointers
    auto smart_ptr = std::make_unique<int>(42);
    
    // Test ref2ptr with smart pointer
    int* raw_ptr = ul::ref2ptr(smart_ptr.get());
    EXPECT_EQ(raw_ptr, smart_ptr.get());
    EXPECT_EQ(*raw_ptr, 42);
    
    // Test ref2ptrc with smart pointer
    const int* const_raw_ptr = ul::ref2ptrc(*smart_ptr.get());
    EXPECT_EQ(const_raw_ptr, smart_ptr.get());
    EXPECT_EQ(*const_raw_ptr, 42);
}

TEST(Ptr2RefTest, WithArrays) {
    // Test with arrays
    int arr[5] = {1, 2, 3, 4, 5};
    int* arr_ptr = arr;
    
    // Test ptr2ref with array element
    int& arr_ref = ul::ptr2ref(arr_ptr);
    EXPECT_EQ(arr_ref, 1);
    arr_ref = 100;
    EXPECT_EQ(arr[0], 100);
    
    // Test ref2ptr with array element
    int* arr_ptr2 = ul::ref2ptr(arr[0]);
    EXPECT_EQ(arr_ptr2, &arr[0]);
    EXPECT_EQ(*arr_ptr2, 100);
}
