#include "mb/ul/basiccodesupport/profiler.hpp"
#include "mb/ul/basiccodesupport/ignore_unused.hpp"
#include "mb/ul/buildenv/gtest_utils.hpp"
#include "gtest/gtest.h"
#include <chrono>
#include <cstddef>
#include <string_view>
#include <thread>

namespace {
using DefaultProfiler = ul::PerformanceProfiler<>;
using ManualProfiler = ul::PerformanceProfiler<ul::ManualProfilerClock>;

constexpr auto exact_seconds_tolerance{1e-12};

void expect_dump_entry(
    const ManualProfiler::DumpDataset& entry,
    std::string_view expected_name,
    size_t expected_count,
    double expected_total,
    double expected_average,
    double expected_mean,
    double expected_std_dev) {
    EXPECT_EQ(expected_name, entry.item_name);
    EXPECT_EQ(expected_count, entry.count);
    EXPECT_NEAR(expected_total, entry.total, exact_seconds_tolerance);
    EXPECT_NEAR(expected_average, entry.average, exact_seconds_tolerance);
    EXPECT_NEAR(expected_mean, entry.mean, exact_seconds_tolerance);
    EXPECT_NEAR(expected_std_dev, entry.std_dev, exact_seconds_tolerance);
}

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

    static void advance(double seconds) {
        ul::ManualProfilerClock::advance(seconds);
    }
};
} // namespace

TEST(profilerTest, basics) {
    const auto before{ul::profiler_now()};
    constexpr auto example_duration{std::chrono::milliseconds{420}};
    std::this_thread::sleep_for(example_duration);
    const auto after{ul::profiler_now()};
    EXPECT_GT(after, before);
    const auto diff{std::chrono::duration_cast<std::chrono::milliseconds>(ul::profiler_diff(before, after))};
    EXPECT_GT(720, diff.count());
    EXPECT_LT(120, diff.count());
}

TEST(ToFormattedStringTest, Rounding) {
    EXPECT_EQ("99.99 ps", DefaultProfiler::to_formatted_string(0.000000000099985));
    EXPECT_EQ("99.98 ps", DefaultProfiler::to_formatted_string(0.000000000099984));
    EXPECT_EQ("99.99 ns", DefaultProfiler::to_formatted_string(0.000000099986));
    EXPECT_EQ("99.98 ns", DefaultProfiler::to_formatted_string(0.000000099983));
    EXPECT_EQ("99.99 \xC2\xB5s", DefaultProfiler::to_formatted_string(0.000099987));
    EXPECT_EQ("99.98 \xC2\xB5s", DefaultProfiler::to_formatted_string(0.000099982));
    EXPECT_EQ("99.99 ms", DefaultProfiler::to_formatted_string(0.099988));
    EXPECT_EQ("99.98 ms", DefaultProfiler::to_formatted_string(0.099981));
    EXPECT_EQ("59.99 s", DefaultProfiler::to_formatted_string(59.989));
    EXPECT_EQ("59.98 s", DefaultProfiler::to_formatted_string(59.980));
    EXPECT_EQ("59:59.99", DefaultProfiler::to_formatted_string(3599.985));
    EXPECT_EQ("59:59.98", DefaultProfiler::to_formatted_string(3599.984));
}

TEST(ToFormattedStringTest, UnitStepping) {
    EXPECT_EQ("99.99 ps", DefaultProfiler::to_formatted_string(0.000000000099990));
    EXPECT_EQ("99.99 ns", DefaultProfiler::to_formatted_string(0.000000099990));
    EXPECT_EQ("99.99 \xC2\xB5s", DefaultProfiler::to_formatted_string(0.000099990));
    EXPECT_EQ("99.99 ms", DefaultProfiler::to_formatted_string(0.099990));
    EXPECT_EQ("0.10 s", DefaultProfiler::to_formatted_string(0.1));
    EXPECT_EQ("1.00 s", DefaultProfiler::to_formatted_string(1.0));
    EXPECT_EQ("01:00.00", DefaultProfiler::to_formatted_string(60.0));
    EXPECT_EQ("01:00:00", DefaultProfiler::to_formatted_string(3600.0));
}

TEST(ToFormattedStringTest, RoundingCausingUnitStepping) {
    EXPECT_EQ("0.10 s", DefaultProfiler::to_formatted_string(0.099999));
}

TEST(ToFormattedStringTest, NoRounding) {
    EXPECT_EQ("99:59:58", DefaultProfiler::to_formatted_string(359998.5));
    EXPECT_EQ("99:59:58", DefaultProfiler::to_formatted_string(359998.4));
}

TEST(ToFormattedStringTest, Zero) {
    EXPECT_EQ("0.00 ps", DefaultProfiler::to_formatted_string(0.0));
}

TEST(ToFormattedStringTest, Neg) {
    EXPECT_EQ("-1.00 s", DefaultProfiler::to_formatted_string(-1.0));
}

TEST(ToFormattedStringTest, Inf) {
    EXPECT_EQ(">= 100 h", DefaultProfiler::to_formatted_string(360'000));
}

// NOLINTBEGIN
TEST_F(ManualProfilerTest, PracticalScenario) {
    ul::dump_test_name();
    ManualProfiler perfscope0{"1. 500ms"};
    EXPECT_NEAR(0.0, perfscope0.elapsed_current_item(), exact_seconds_tolerance);
    advance(0.5);
    EXPECT_NEAR(0.5, perfscope0.elapsed_current_item(), exact_seconds_tolerance);
    perfscope0.start_new_item("2. nest0");
    {
        ManualProfiler perfscope1{"3. 100ms", 1U};
        advance(0.1);
        perfscope1.start_new_item("4. nest1");
        {
            ManualProfiler perfscope2{"5. for", 2U};
            for (size_t _{}; _ < 5U; ++_) {
                const ManualProfiler perfscope3{"6. 500ms", 3U};
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
    const auto& data{ManualProfiler::dumped_data()};
    ASSERT_EQ(9U, data.size());
    expect_dump_entry(data[0U], "1. 500ms", 1U, 0.5, 0.5, 0.5, 0.0);
    expect_dump_entry(data[1U], "2. nest0", 1U, 2.8, 2.8, 2.8, 0.0);
    expect_dump_entry(data[2U], " 3. 100ms", 1U, 0.1, 0.1, 0.1, 0.0);
    expect_dump_entry(data[3U], " 4. nest1", 1U, 2.7, 2.7, 2.7, 0.0);
    expect_dump_entry(data[4U], "  5. for", 1U, 2.5, 2.5, 2.5, 0.0);
    expect_dump_entry(data[5U], "   6. 500ms", 5U, 2.5, 0.5, 0.5, 0.0);
    expect_dump_entry(data[6U], "  7. 200ms", 1U, 0.2, 0.2, 0.2, 0.0);
    expect_dump_entry(data[7U], "8. 300ms", 1U, 0.3, 0.3, 0.3, 0.0);
    expect_dump_entry(data[8U], "9. 100ms", 1U, 0.1, 0.1, 0.1, 0.0);

    ManualProfiler::reset();
    EXPECT_STREQ(
        ManualProfiler::dump_all_items<ManualProfiler::DumpFormat::string_and_structure>().c_str(),
        "No performance measurement data.\n");
    EXPECT_EQ(0U, ManualProfiler::dumped_data().size());
}

// NOLINTEND
