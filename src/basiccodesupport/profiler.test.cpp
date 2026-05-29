#include "mb/ul/basiccodesupport/profiler.hpp"
#include "mb/ul/basiccodesupport/ignore_unused.hpp"
#include "mb/ul/buildenv/gtest_utils.hpp"
#include "gtest/gtest.h"
#include <chrono>
#include <thread>

namespace {
using ManualProfiler = ul::PerformanceProfiler<ul::ManualProfilerClock>;

constexpr auto exact_seconds_tolerance{1e-12};

class ManualProfilerTest : public testing::Test {
protected:
    void SetUp() override {
        ul::ManualProfilerClock::reset();
        ManualProfiler::reset();
    }

    void TearDown() override {
        ManualProfiler::reset();
        ul::ManualProfilerClock::reset();
    }

    static void advance(const double seconds) {
        ul::ManualProfilerClock::advance(seconds);
    }
};
} // namespace

TEST(profilerTest, basics) {
    const auto before{ul::profiler_now()};
    constexpr auto example_duration{std::chrono::milliseconds(420)};
    std::this_thread::sleep_for(example_duration);
    const auto after{ul::profiler_now()};
    EXPECT_GT(after, before);
    const auto diff{std::chrono::duration_cast<std::chrono::milliseconds>(ul::profiler_diff(before, after))};
    EXPECT_GT(720, diff.count());
    EXPECT_LT(120, diff.count());
}

TEST(ToFormattedStringTest, Rounding) {
    EXPECT_EQ("99.99 ps", ul::PerformanceProfiler<>::to_formatted_string(0.000000000099985));
    EXPECT_EQ("99.98 ps", ul::PerformanceProfiler<>::to_formatted_string(0.000000000099984));
    EXPECT_EQ("99.99 ns", ul::PerformanceProfiler<>::to_formatted_string(0.000000099986));
    EXPECT_EQ("99.98 ns", ul::PerformanceProfiler<>::to_formatted_string(0.000000099983));
    EXPECT_EQ("99.99 \xC2\xB5s", ul::PerformanceProfiler<>::to_formatted_string(0.000099987));
    EXPECT_EQ("99.98 \xC2\xB5s", ul::PerformanceProfiler<>::to_formatted_string(0.000099982));
    EXPECT_EQ("99.99 ms", ul::PerformanceProfiler<>::to_formatted_string(0.099988));
    EXPECT_EQ("99.98 ms", ul::PerformanceProfiler<>::to_formatted_string(0.099981));
    EXPECT_EQ("59.99 s", ul::PerformanceProfiler<>::to_formatted_string(59.989));
    EXPECT_EQ("59.98 s", ul::PerformanceProfiler<>::to_formatted_string(59.980));
    EXPECT_EQ("59:59.99", ul::PerformanceProfiler<>::to_formatted_string(3599.985));
    EXPECT_EQ("59:59.98", ul::PerformanceProfiler<>::to_formatted_string(3599.984));
}

TEST(ToFormattedStringTest, UnitStepping) {
    EXPECT_EQ("99.99 ps", ul::PerformanceProfiler<>::to_formatted_string(0.000000000099990));
    EXPECT_EQ("99.99 ns", ul::PerformanceProfiler<>::to_formatted_string(0.000000099990));
    EXPECT_EQ("99.99 \xC2\xB5s", ul::PerformanceProfiler<>::to_formatted_string(0.000099990));
    EXPECT_EQ("99.99 ms", ul::PerformanceProfiler<>::to_formatted_string(0.099990));
    EXPECT_EQ("0.10 s", ul::PerformanceProfiler<>::to_formatted_string(0.1));
    EXPECT_EQ("1.00 s", ul::PerformanceProfiler<>::to_formatted_string(1.0));
    EXPECT_EQ("01:00.00", ul::PerformanceProfiler<>::to_formatted_string(60.0));
    EXPECT_EQ("01:00:00", ul::PerformanceProfiler<>::to_formatted_string(3600.0));
}

TEST(ToFormattedStringTest, RoundingCausingUnitStepping) {
    EXPECT_EQ("0.10 s", ul::PerformanceProfiler<>::to_formatted_string(0.099999));
}

TEST(ToFormattedStringTest, NoRounding) {
    EXPECT_EQ("99:59:58", ul::PerformanceProfiler<>::to_formatted_string(359998.5));
    EXPECT_EQ("99:59:58", ul::PerformanceProfiler<>::to_formatted_string(359998.4));
}

TEST(ToFormattedStringTest, Zero) {
    EXPECT_EQ("0.00 ps", ul::PerformanceProfiler<>::to_formatted_string(0.0));
}

TEST(ToFormattedStringTest, Neg) {
    EXPECT_EQ("-1.00 s", ul::PerformanceProfiler<>::to_formatted_string(-1.0));
}

TEST(ToFormattedStringTest, Inf) {
    EXPECT_EQ(">= 100 h", ul::PerformanceProfiler<>::to_formatted_string(360'000));
}

// NOLINTBEGIN
TEST_F(ManualProfilerTest, PracticalScenario) {
    ul::dump_test_name();
    ManualProfiler perfscope0("1. 500ms");
    EXPECT_NEAR(0.0, perfscope0.elapsed_current_item(), exact_seconds_tolerance);
    advance(0.5);
    EXPECT_NEAR(0.5, perfscope0.elapsed_current_item(), exact_seconds_tolerance);
    perfscope0.start_new_item("2. nest0");
    {
        ManualProfiler perfscope1("3. 100ms", 1);
        advance(0.1);
        perfscope1.start_new_item("4. nest1");
        {
            ManualProfiler perfscope2("5. for", 2);
            for (size_t i = 1; i <= 5; ++i) {
                const ManualProfiler perfscope3("6. 500ms", 3);
                ul::ignore_unused(perfscope3);
                advance(0.5);
            }
            perfscope2.start_new_item("7. 200ms");
            advance(0.2);
        }
    }
    perfscope0.start_new_item("8. 300ms");
    advance(0.3);
    perfscope0.start_new_item("9. 100ms");
    advance(0.1);
    perfscope0.stop_item();
    std::cout << ManualProfiler::dump_all_items<ManualProfiler::DumpFormat::string_and_structure>();
    const std::vector<ManualProfiler::DumpDataset>& data = ManualProfiler::dumped_data();
    ASSERT_EQ(9U, data.size());
    EXPECT_EQ("1. 500ms", data[0].item_name);
    EXPECT_EQ(1U, data[0].count);
    EXPECT_NEAR(0.5, data[0].total, exact_seconds_tolerance);
    EXPECT_NEAR(0.5, data[0].average, exact_seconds_tolerance);
    EXPECT_NEAR(0.5, data[0].mean, exact_seconds_tolerance);
    EXPECT_NEAR(0.0, data[0].std_dev, exact_seconds_tolerance);

    EXPECT_EQ("2. nest0", data[1].item_name);
    EXPECT_EQ(1U, data[1].count);
    EXPECT_NEAR(2.8, data[1].total, exact_seconds_tolerance);
    EXPECT_NEAR(2.8, data[1].average, exact_seconds_tolerance);
    EXPECT_NEAR(2.8, data[1].mean, exact_seconds_tolerance);
    EXPECT_NEAR(0.0, data[1].std_dev, exact_seconds_tolerance);

    EXPECT_EQ(" 3. 100ms", data[2].item_name);
    EXPECT_EQ(1U, data[2].count);
    EXPECT_NEAR(0.1, data[2].total, exact_seconds_tolerance);
    EXPECT_NEAR(0.1, data[2].average, exact_seconds_tolerance);
    EXPECT_NEAR(0.1, data[2].mean, exact_seconds_tolerance);
    EXPECT_NEAR(0.0, data[2].std_dev, exact_seconds_tolerance);

    EXPECT_EQ(" 4. nest1", data[3].item_name);
    EXPECT_EQ(1U, data[3].count);
    EXPECT_NEAR(2.7, data[3].total, exact_seconds_tolerance);
    EXPECT_NEAR(2.7, data[3].average, exact_seconds_tolerance);
    EXPECT_NEAR(2.7, data[3].mean, exact_seconds_tolerance);
    EXPECT_NEAR(0.0, data[3].std_dev, exact_seconds_tolerance);

    EXPECT_EQ("  5. for", data[4].item_name);
    EXPECT_EQ(1U, data[4].count);
    EXPECT_NEAR(2.5, data[4].total, exact_seconds_tolerance);
    EXPECT_NEAR(2.5, data[4].average, exact_seconds_tolerance);
    EXPECT_NEAR(2.5, data[4].mean, exact_seconds_tolerance);
    EXPECT_NEAR(0.0, data[4].std_dev, exact_seconds_tolerance);

    EXPECT_EQ("   6. 500ms", data[5].item_name);
    EXPECT_EQ(5U, data[5].count);
    EXPECT_NEAR(2.5, data[5].total, exact_seconds_tolerance);
    EXPECT_NEAR(0.5, data[5].average, exact_seconds_tolerance);
    EXPECT_NEAR(0.5, data[5].mean, exact_seconds_tolerance);
    EXPECT_NEAR(0.0, data[5].std_dev, exact_seconds_tolerance);

    EXPECT_EQ("  7. 200ms", data[6].item_name);
    EXPECT_EQ(1U, data[6].count);
    EXPECT_NEAR(0.2, data[6].total, exact_seconds_tolerance);
    EXPECT_NEAR(0.2, data[6].average, exact_seconds_tolerance);
    EXPECT_NEAR(0.2, data[6].mean, exact_seconds_tolerance);
    EXPECT_NEAR(0.0, data[6].std_dev, exact_seconds_tolerance);

    EXPECT_EQ("8. 300ms", data[7].item_name);
    EXPECT_EQ(1U, data[7].count);
    EXPECT_NEAR(0.3, data[7].total, exact_seconds_tolerance);
    EXPECT_NEAR(0.3, data[7].average, exact_seconds_tolerance);
    EXPECT_NEAR(0.3, data[7].mean, exact_seconds_tolerance);
    EXPECT_NEAR(0.0, data[7].std_dev, exact_seconds_tolerance);

    EXPECT_EQ("9. 100ms", data[8].item_name);
    EXPECT_EQ(1U, data[8].count);
    EXPECT_NEAR(0.1, data[8].total, exact_seconds_tolerance);
    EXPECT_NEAR(0.1, data[8].average, exact_seconds_tolerance);
    EXPECT_NEAR(0.1, data[8].mean, exact_seconds_tolerance);
    EXPECT_NEAR(0.0, data[8].std_dev, exact_seconds_tolerance);

    ManualProfiler::reset();
    EXPECT_STREQ(
        ManualProfiler::dump_all_items<ManualProfiler::DumpFormat::string_and_structure>().c_str(),
        "No performance measurement data.\n");
    EXPECT_EQ(0U, ManualProfiler::dumped_data().size());
}

// NOLINTEND
