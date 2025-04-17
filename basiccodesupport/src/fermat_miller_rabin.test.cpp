#include "ul/fermat_miller_rabin.h" // NOLINT
#include "gtest/gtest.h"
#include <vector>
#include <cstdint>

#if __cpp_concepts && __cpp_lib_concepts

TEST(multiplicative_inverse_fermat, tests) {
    // Test multiplicative_inverse_fermat
    // For prime 7, the multiplicative inverse of 3 is 5 (3 * 5 = 15, 15 % 7 = 1)
    EXPECT_EQ(ul::multiplicative_inverse_fermat(3, 7), 5);
}

TEST(IntegerTest, ModuloOperations) {
    // Test ModuloMultiply
    const ul::ModuloMultiply<int> mmult{7};
    EXPECT_EQ(mmult(3, 4), 5); // 3 * 4 = 12, 12 % 7 = 5
    
    // Test identity_element
    EXPECT_EQ(ul::identity_element(mmult), 1);
    
    // Test fermat_test
    EXPECT_TRUE(ul::fermat_test(7, 2));  // 7 is prime
    EXPECT_TRUE(ul::fermat_test(11, 2)); // 11 is prime
    EXPECT_FALSE(ul::fermat_test(15, 2)); // 15 is not prime
    
    // Test miller_rabin_test
    // For n = 7, n-1 = 6 = 2 * 3 * 1, so q = 3, k = 1
    EXPECT_TRUE(ul::miller_rabin_test(7, 3, 1, 2));
}

TEST(PrimalityTests, Fermat) {
    EXPECT_TRUE(ul::fermat_test(7, 2));  // 7 is prime
}

TEST(PrimalityTests, MillerRabin) {
    EXPECT_TRUE(ul::miller_rabin_test(7, 3, 1, 2));
}

#else
TEST(IntegerTest, Skipped) {
    GTEST_SKIP() << "C++20 concepts not supported";
}
#endif
