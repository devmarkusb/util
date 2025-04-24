#include "ul/random.h"
#include "gtest/gtest.h"
#include <cstddef>
#include <vector>
#include <algorithm>

namespace ul = mb::ul;

TEST(RandomTest, random_frac) {
    const int num_samples = 10000;
    std::vector<double> samples;
    samples.reserve(num_samples);
    
    // Generate samples
    for (int i = 0; i < num_samples; ++i) {
        samples.push_back(ul::math::random_frac());
    }
    
    // Check range [0.0, 1.0)
    for (const auto sample : samples) {
        EXPECT_GE(sample, 0.0);
        EXPECT_LT(sample, 1.0);
    }
    
    // Check distribution
    // Count samples in each decile
    std::vector decile_counts(10, 0);
    for (const auto sample : samples) {
        const auto decile = static_cast<size_t>(sample * 10);
        decile_counts[decile]++;
    }
    
    // Expected count per decile (with some tolerance)
    const int expected_per_decile = num_samples / 10;
    const int tolerance = expected_per_decile / 2;  // 50% tolerance
    
    for (const auto count : decile_counts) {
        EXPECT_GE(count, expected_per_decile - tolerance);
        EXPECT_LE(count, expected_per_decile + tolerance);
    }
    
    // Check for basic randomness properties
    // 1. No consecutive identical values
    bool found_different = false;
    for (size_t i = 1; i < samples.size(); ++i) {
        if (samples[i] != samples[i-1]) {
            found_different = true;
            break;
        }
    }
    EXPECT_TRUE(found_different);
    
    // 2. Check that values are not all the same
    const auto [min_it, max_it] = std::minmax_element(samples.begin(), samples.end());
    EXPECT_GT(*max_it - *min_it, 0.0);
}
