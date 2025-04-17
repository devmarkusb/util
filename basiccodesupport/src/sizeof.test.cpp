#include "ul/sizeof.h"
#include "gtest/gtest.h"
#include <string>
#include <vector>

// Test structs of different sizes
struct SmallStruct {
    char c{};
};

struct MediumStruct {
    int i{};
    double d{};
};

struct LargeStruct {
    char arr[100]{};
};

// Test sizes at compile time using static_assert
TEST(SizeofTest, VerifySizes) {
    // These static_asserts verify the sizes at compile time
    static_assert(sizeof(SmallStruct) == 1, "SmallStruct should be 1 byte");
    static_assert(sizeof(MediumStruct) == 16, "MediumStruct should be 16 bytes");
    static_assert(sizeof(LargeStruct) == 100, "LargeStruct should be 100 bytes");
    
    static_assert(sizeof(char) == 1, "char should be 1 byte");
    static_assert(sizeof(int) == 4, "int should be 4 bytes");
    static_assert(sizeof(double) == 8, "double should be 8 bytes");
    static_assert(sizeof(void*) == 8, "void* should be 8 bytes");
    
    static_assert(sizeof(std::string) == 24, "std::string should be 24 bytes");
    static_assert(sizeof(std::vector<int>) == 24, "std::vector<int> should be 24 bytes");
    
    // Add a runtime assertion to make the test do something
    SUCCEED() << "All size checks passed at compile time";
}

// Note: UL_CT_SIZEOF_OPT2 can only be tested at compile time
// as it generates compile errors. It cannot be tested at runtime.
// The following would cause compile errors if uncommented:
TEST(SizeofTest, CT_Sizeof) {
    // UL_CT_SIZEOF_OPT2(SmallStruct);  // This would cause a compile error
    // UL_CT_SIZEOF_OPT2(MediumStruct); // This would cause a compile error
    // UL_CT_SIZEOF_OPT2(LargeStruct);  // This would cause a compile error
}
