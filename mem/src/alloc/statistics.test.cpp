#include "ul/mem/alloc/statistics.h" // NOLINT
#include "ul/mem/types.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

class TestStatistics : public ul::mem::alloc::Statistics {
public:
    using Statistics::stats_collect_alloc;
    using Statistics::stats_collect_current_size;
    using Statistics::stats_collect_dealloc;
    using Statistics::peak;
};

TEST(alloc_statistics, peak_tracking) {
    TestStatistics stats;
    
    // Initial peak should be nullopt
    EXPECT_FALSE(stats.peak());
    
    // Test peak tracking with increasing allocations
    stats.stats_collect_current_size(ul::mem::Bytes{100});
    ASSERT_TRUE(stats.peak());
    EXPECT_EQ(stats.peak().value(), ul::mem::Bytes{100});
    
    stats.stats_collect_current_size(ul::mem::Bytes{200});
    EXPECT_EQ(stats.peak().value(), ul::mem::Bytes{200});
    
    // Peak should not decrease
    stats.stats_collect_current_size(ul::mem::Bytes{150});
    EXPECT_EQ(stats.peak().value(), ul::mem::Bytes{200});
}

TEST(alloc_statistics, allocation_tracking) {
    TestStatistics stats;
    
    // Test allocation tracking
    stats.stats_collect_alloc(ul::mem::Bytes{100});
    ASSERT_TRUE(stats.peak());
    EXPECT_EQ(stats.peak().value(), ul::mem::Bytes{100});
    
    stats.stats_collect_alloc(ul::mem::Bytes{50});
    EXPECT_EQ(stats.peak().value(), ul::mem::Bytes{150});
    
    // Test deallocation tracking
    stats.stats_collect_dealloc(ul::mem::Bytes{30});
    // Peak should remain at 150 even after deallocation
    EXPECT_EQ(stats.peak().value(), ul::mem::Bytes{150});
}

TEST(alloc_statistics, no_statistics) {
    // Test NoStatistics class - only public interface
    EXPECT_FALSE(ul::mem::alloc::NoStatistics::peak());
}
