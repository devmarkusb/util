#include "ul/profiler.h"
#include "ul/ignore_unused.h"
#include "gtest/gtest.h"
#include <thread>

TEST(profilerTest, basics) {
    const auto before{ul::profiler_now()};
    constexpr auto exampleDuration{std::chrono::milliseconds(42)};
    std::this_thread::sleep_for(exampleDuration);
    const auto after{ul::profiler_now()};
    EXPECT_GT(after, before);
    const auto diff{std::chrono::duration_cast<std::chrono::milliseconds>(ul::profiler_diff(before, after))};
    EXPECT_GT(62, diff.count());
    EXPECT_LT(22, diff.count());
}

TEST(ToFormattedStringTest, Rounding) {
    EXPECT_EQ("99.99 ps", ul::PerformanceProfiler::toFormattedString(0.000000000099985));
    EXPECT_EQ("99.98 ps", ul::PerformanceProfiler::toFormattedString(0.000000000099984));
    EXPECT_EQ("99.99 ns", ul::PerformanceProfiler::toFormattedString(0.000000099986));
    EXPECT_EQ("99.98 ns", ul::PerformanceProfiler::toFormattedString(0.000000099983));
    EXPECT_EQ("99.99 \xC2\xB5s", ul::PerformanceProfiler::toFormattedString(0.000099987));
    EXPECT_EQ("99.98 \xC2\xB5s", ul::PerformanceProfiler::toFormattedString(0.000099982));
    EXPECT_EQ("99.99 ms", ul::PerformanceProfiler::toFormattedString(0.099988));
    EXPECT_EQ("99.98 ms", ul::PerformanceProfiler::toFormattedString(0.099981));
    EXPECT_EQ("59.99 s", ul::PerformanceProfiler::toFormattedString(59.989));
    EXPECT_EQ("59.98 s", ul::PerformanceProfiler::toFormattedString(59.980));
    EXPECT_EQ("59:59.99", ul::PerformanceProfiler::toFormattedString(3599.985));
    EXPECT_EQ("59:59.98", ul::PerformanceProfiler::toFormattedString(3599.984));
}

TEST(ToFormattedStringTest, UnitStepping) {
    EXPECT_EQ("99.99 ps", ul::PerformanceProfiler::toFormattedString(0.000000000099990));
    EXPECT_EQ("99.99 ns", ul::PerformanceProfiler::toFormattedString(0.000000099990));
    EXPECT_EQ("99.99 \xC2\xB5s", ul::PerformanceProfiler::toFormattedString(0.000099990));
    EXPECT_EQ("99.99 ms", ul::PerformanceProfiler::toFormattedString(0.099990));
    EXPECT_EQ("0.10 s", ul::PerformanceProfiler::toFormattedString(0.1));
    EXPECT_EQ("1.00 s", ul::PerformanceProfiler::toFormattedString(1.0));
    EXPECT_EQ("01:00.00", ul::PerformanceProfiler::toFormattedString(60.0));
    EXPECT_EQ("01:00:00", ul::PerformanceProfiler::toFormattedString(3600.0));
}

TEST(ToFormattedStringTest, RoundingCausingUnitStepping) {
    EXPECT_EQ("0.10 s", ul::PerformanceProfiler::toFormattedString(0.099999));
}

TEST(ToFormattedStringTest, NoRounding) {
    EXPECT_EQ("99:59:58", ul::PerformanceProfiler::toFormattedString(359998.5));
    EXPECT_EQ("99:59:58", ul::PerformanceProfiler::toFormattedString(359998.4));
}

TEST(ToFormattedStringTest, Zero) {
    EXPECT_EQ("0.00 ps", ul::PerformanceProfiler::toFormattedString(0.0));
}

TEST(ToFormattedStringTest, Neg) {
    EXPECT_EQ("-1.00 s", ul::PerformanceProfiler::toFormattedString(-1.0));
}

TEST(ToFormattedStringTest, Inf) {
    EXPECT_EQ(">= 100 h", ul::PerformanceProfiler::toFormattedString(360000));
}

// NOLINTBEGIN
TEST(DISABLED_DumpAllItemsTest, PracticalScenario) {
    ul::PerformanceProfiler perfscope0("1. 500ms");
    EXPECT_NEAR(0.0, perfscope0.elapsed_currentItem(), 0.02);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    EXPECT_NEAR(0.5, perfscope0.elapsed_currentItem(), 0.02);
    perfscope0.startNewItem("2. nest0");
    {
        ul::PerformanceProfiler perfscope1("3. 100ms", 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        perfscope1.startNewItem("4. nest1");
        {
            ul::PerformanceProfiler perfscope2("5. for", 2);
            for (size_t i = 1; i <= 5; ++i) {
                ul::PerformanceProfiler const perfscope3("6. 500ms", 3);
                ul::ignore_unused(perfscope3);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            perfscope2.startNewItem("7. 200ms");
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
    perfscope0.startNewItem("8. 300ms");
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    perfscope0.startNewItem("9. 100ms");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    perfscope0.stopItem();
    std::cout << ul::PerformanceProfiler::dumpAllItems<ul::PerformanceProfiler::DumpFormat::stringAndStructure>();
    const std::vector<ul::PerformanceProfiler::DumpDataset>& data = ul::PerformanceProfiler::dumpedData();
    ASSERT_EQ(9u, data.size());
    EXPECT_EQ("1. 500ms", data[0].itemName_);
    EXPECT_EQ(1u, data[0].count_);
    EXPECT_NEAR(0.5, data[0].total_, 0.02);
    EXPECT_NEAR(0.5, data[0].average_, 0.02);
    EXPECT_NEAR(0.5, data[0].mean_, 0.02);
    EXPECT_NEAR(0.0, data[0].stdDev_, 0.0);

    EXPECT_EQ("2. nest0", data[1].itemName_);
    EXPECT_EQ(1u, data[1].count_);
    EXPECT_NEAR(2.8, data[1].total_, 0.1);
    EXPECT_NEAR(2.8, data[1].average_, 0.1);
    EXPECT_NEAR(2.8, data[1].mean_, 0.1);
    EXPECT_NEAR(0.0, data[1].stdDev_, 0.0);

    EXPECT_EQ(" 3. 100ms", data[2].itemName_);
    EXPECT_EQ(1u, data[2].count_);
    EXPECT_NEAR(0.1, data[2].total_, 0.02);
    EXPECT_NEAR(0.1, data[2].average_, 0.02);
    EXPECT_NEAR(0.1, data[2].mean_, 0.02);
    EXPECT_NEAR(0.0, data[2].stdDev_, 0.0);

    EXPECT_EQ(" 4. nest1", data[3].itemName_);
    EXPECT_EQ(1u, data[3].count_);
    EXPECT_NEAR(2.7, data[3].total_, 0.1);
    EXPECT_NEAR(2.7, data[3].average_, 0.1);
    EXPECT_NEAR(2.7, data[3].mean_, 0.1);
    EXPECT_NEAR(0.0, data[3].stdDev_, 0.0);

    EXPECT_EQ("  5. for", data[4].itemName_);
    EXPECT_EQ(1u, data[4].count_);
    EXPECT_NEAR(2.5, data[4].total_, 0.1);
    EXPECT_NEAR(2.5, data[4].average_, 0.1);
    EXPECT_NEAR(2.5, data[4].mean_, 0.1);
    EXPECT_NEAR(0.0, data[4].stdDev_, 0.0);

    EXPECT_EQ("   6. 500ms", data[5].itemName_);
    EXPECT_EQ(5u, data[5].count_);
    EXPECT_NEAR(2.5, data[5].total_, 0.1);
    EXPECT_NEAR(0.5, data[5].average_, 0.02);
    EXPECT_NEAR(0.5, data[5].mean_, 0.02);
    EXPECT_NEAR(0.0, data[5].stdDev_, 0.02);

    EXPECT_EQ("  7. 200ms", data[6].itemName_);
    EXPECT_EQ(1u, data[6].count_);
    EXPECT_NEAR(0.2, data[6].total_, 0.02);
    EXPECT_NEAR(0.2, data[6].average_, 0.02);
    EXPECT_NEAR(0.2, data[6].mean_, 0.02);
    EXPECT_NEAR(0.0, data[6].stdDev_, 0.0);

    EXPECT_EQ("8. 300ms", data[7].itemName_);
    EXPECT_EQ(1u, data[7].count_);
    EXPECT_NEAR(0.3, data[7].total_, 0.02);
    EXPECT_NEAR(0.3, data[7].average_, 0.02);
    EXPECT_NEAR(0.3, data[7].mean_, 0.02);
    EXPECT_NEAR(0.0, data[7].stdDev_, 0.0);

    EXPECT_EQ("9. 100ms", data[8].itemName_);
    EXPECT_EQ(1u, data[8].count_);
    EXPECT_NEAR(0.1, data[8].total_, 0.02);
    EXPECT_NEAR(0.1, data[8].average_, 0.02);
    EXPECT_NEAR(0.1, data[8].mean_, 0.02);
    EXPECT_NEAR(0.0, data[8].stdDev_, 0.0);

    ul::PerformanceProfiler::reset();
    EXPECT_STREQ(
        ul::PerformanceProfiler::dumpAllItems<ul::PerformanceProfiler::DumpFormat::stringAndStructure>().c_str(),
        "No performance measurement data.\n");
    EXPECT_EQ(0u, data.size());
}

// NOLINTEND
