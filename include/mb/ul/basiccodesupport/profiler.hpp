//! \file

#ifndef PROFILER_H_958X2G58SDJKFH23789HGXT34
#define PROFILER_H_958X2G58SDJKFH23789HGXT34

#include "assert.hpp"
#include "round.hpp"
#include "type/non_copyable.hpp"

#include "mb/ul/buildenv/config.hpp"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace mb::ul {
using ProfilerTimePoint = std::chrono::time_point<std::chrono::steady_clock>;

//! Returns the current monotonic time point for lightweight elapsed-time checks.
[[nodiscard]] inline ProfilerTimePoint profiler_now() {
    return std::chrono::steady_clock::now();
}

//! Returns the duration between two profiler time points.
[[nodiscard]] inline auto profiler_diff(ProfilerTimePoint start, ProfilerTimePoint end) {
    return end - start;
}

//! Monotonic clock used by PerformanceProfiler by default.
struct ProfilerSteadyClock {
    using rep = double;
    using period = std::ratio<1>;
    using duration = std::chrono::duration<rep, period>;
    using time_point = std::chrono::time_point<ProfilerSteadyClock, duration>;

    [[nodiscard]] static time_point now() noexcept {
        const auto elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now().time_since_epoch());
        return time_point{duration{elapsed.count()}};
    }
};

using ProfilerSystemClock = ProfilerSteadyClock;

//! Manual clock for deterministic PerformanceProfiler tests.
struct ManualProfilerClock {
    using rep = double;
    using period = std::ratio<1>;
    using duration = std::chrono::duration<rep, period>;
    using time_point = std::chrono::time_point<ManualProfilerClock, duration>;

    [[nodiscard]] static time_point now() noexcept {
        return time_point{duration{elapsed_seconds()}};
    }

    static void advance(double seconds) {
        elapsed_seconds() += seconds;
    }

    static void reset() noexcept {
        elapsed_seconds() = 0.0;
    }

private:
    [[nodiscard]] static double& elapsed_seconds() {
        static double elapsed{};
        return elapsed;
    }
};

template <typename Clock>
concept ProfilerClock = requires {
    typename Clock::duration;
    typename Clock::time_point;
    { Clock::now() } noexcept -> std::same_as<typename Clock::time_point>;
};

//! Can only be used within one thread at the same time.
/** Usage: Cf. unit tests.*/
template <ProfilerClock Clock = ProfilerSteadyClock>
class PerformanceProfiler : private NonCopyable {
public:
    using TimeValStorageRep = double;
    using SecondsDbl = TimeValStorageRep;
    using NestingLevel = unsigned int;

    //! \param nesting_level is just for dump visualization
    explicit PerformanceProfiler(std::string_view new_item_name, NestingLevel nesting_level = 0);
    ~PerformanceProfiler() noexcept;
    [[nodiscard]] SecondsDbl elapsed_current_item() const;

    //! New item on same hierarchy/nesting level.
    void start_new_item(std::string_view new_item_name);
    void stop_item();

    enum class DumpFormat : uint8_t {
        string_only,
        string_and_structure,
    };

    //! Also fills dumped_data() if fmt is DumpFormat::string_and_structure.
    template <DumpFormat fmt = DumpFormat::string_only>
    [[nodiscard]] static std::string dump_all_items();

    static void reset();
    [[nodiscard]] static std::string to_formatted_string(SecondsDbl seconds);

    //! Only for testing.
    struct DumpDataset {
        std::string item_name;
        size_t count{};
        SecondsDbl total{};
        SecondsDbl average{};
        SecondsDbl mean{};
        SecondsDbl std_dev{};
    };

    //! Filled with structured data (e.g. for testing) if dump_all_items() was called with
    //! DumpFormat::string_and_structure.
    [[nodiscard]] static std::vector<DumpDataset>& dumped_data() {
        static std::vector<DumpDataset> data;
        return data;
    }

private:
    struct KeyData;
    using UniqueItemStartNr = uint64_t;

    struct ItemData {
        TimeValStorageRep time_val{};
        NestingLevel nesting_level{};
        UniqueItemStartNr start_nr{};

        ItemData(TimeValStorageRep time_val_, NestingLevel nesting_level_, UniqueItemStartNr start_nr_)
            : time_val{time_val_}
            , nesting_level{nesting_level_}
            , start_nr{start_nr_} {
        }
    };

    using ItemNameAsKey = std::string;
    using Items = std::multimap<ItemNameAsKey, ItemData>;
    using ChronoDuration = std::chrono::duration<TimeValStorageRep, std::ratio<1, 1>>;
    using ChronoTimepoint = typename Clock::time_point;

    ChronoTimepoint start_time_{};
    std::string item_name_;
    NestingLevel nesting_level_{};
    UniqueItemStartNr item_start_nr_{};

    void start_current_item();
    void stop_current_item();

    [[nodiscard]] static UniqueItemStartNr& unique_item_start_nr() {
        static UniqueItemStartNr next_item_start_nr{};
        return next_item_start_nr;
    }

    [[nodiscard]] static Items& items();

    struct MatchKey {
        explicit MatchKey(ItemNameAsKey key)
            : key_{std::move(key)} {
        }

        [[nodiscard]] bool operator()(const Items::value_type& rhs) const {
            return key_ == rhs.first;
        }

    private:
        ItemNameAsKey key_;
    };

    struct AccumKey {
        explicit AccumKey(ItemNameAsKey key)
            : key_{std::move(key)} {
        }

        [[nodiscard]] TimeValStorageRep operator()(TimeValStorageRep value, const Items::value_type& rhs) const {
            if (key_ == rhs.first) {
                return rhs.second.time_val + value;
            }
            return value;
        }

    private:
        ItemNameAsKey key_;
    };

    struct KeyData {
        NestingLevel nesting_level{};
        UniqueItemStartNr start_nr{};

        explicit KeyData(const ItemData& item_data)
            : nesting_level{item_data.nesting_level}
            , start_nr{item_data.start_nr} {
        }
    };

    struct KeyEntryMaker {
        [[nodiscard]] std::pair<ItemNameAsKey, KeyData> operator()(const Items::value_type& item) const {
            return {item.first, KeyData{item.second}};
        }
    };

    struct StartNrLess {
        template <typename KeyNameAndData>
        [[nodiscard]] bool operator()(const KeyNameAndData& left, const KeyNameAndData& right) const {
            return left.second.start_nr < right.second.start_nr;
        }
    };
};

template <ProfilerClock Clock>
void PerformanceProfiler<Clock>::start_current_item() {
    item_start_nr_ = unique_item_start_nr()++;
    start_time_ = Clock::now();
}

template <ProfilerClock Clock>
void PerformanceProfiler<Clock>::stop_current_item() {
    ItemData item_data{elapsed_current_item(), nesting_level_, item_start_nr_};
    const auto item_it = items().find(item_name_);
    if (item_it != items().end()) {
        // if on the same nesting level, we want to keep the order of a single run through this level;
        // we don't know which representative will be picked later to generate a data record
        if (nesting_level_ == item_it->second.nesting_level) {
            item_data.start_nr = item_it->second.start_nr;
        }
    }
    items().emplace(item_name_, item_data);
}

template <ProfilerClock Clock>
PerformanceProfiler<Clock>::PerformanceProfiler(std::string_view new_item_name, NestingLevel nesting_level)
    : item_name_{new_item_name}
    , nesting_level_{nesting_level} {
    start_current_item();
}

template <ProfilerClock Clock>
PerformanceProfiler<Clock>::SecondsDbl PerformanceProfiler<Clock>::elapsed_current_item() const {
    const auto now = Clock::now();
    const ChronoDuration elapsed = now - start_time_;
    return elapsed.count();
}

template <ProfilerClock Clock>
PerformanceProfiler<Clock>::~PerformanceProfiler() noexcept {
    try {
        stop_current_item();
    } catch (...) {
    }
}

template <ProfilerClock Clock>
void PerformanceProfiler<Clock>::start_new_item(std::string_view new_item_name) {
    stop_current_item();
    item_name_ = new_item_name;
    start_current_item();
}

template <ProfilerClock Clock>
void PerformanceProfiler<Clock>::stop_item() {
    stop_current_item();
}

namespace impl_profiler_dump {
[[nodiscard]] inline std::string item_name_with_nesting(
    std::string_view item_name, unsigned int nesting_level, size_t column_width_huge) {
    std::ostringstream item_name_stream;
    constexpr unsigned int nesting_levels_symbolized_by_spaces{20U};
    for (
        unsigned int nesting_level_index{1U};
        nesting_level_index <= nesting_level && nesting_level_index <= nesting_levels_symbolized_by_spaces;
        ++nesting_level_index) {
        item_name_stream << ' ';
    }
    item_name_stream << item_name;
    auto item_name_with_nesting_level{item_name_stream.str()};
    if (item_name_with_nesting_level.length() >= column_width_huge) {
        item_name_with_nesting_level.resize(column_width_huge - 1U);
    }
    return item_name_with_nesting_level;
}

[[nodiscard]] inline double variance(const std::vector<double>& sorted_items, double mean_seconds) {
    double sum{};
    for (const auto item_seconds : sorted_items) {
        sum += std::pow(item_seconds - mean_seconds, 2.0);
    }
    return sum;
}
} // namespace impl_profiler_dump

// NOLINTBEGIN
template <ProfilerClock Clock>
template <typename PerformanceProfiler<Clock>::DumpFormat fmt>
std::string PerformanceProfiler<Clock>::dump_all_items() {
    if constexpr (fmt != DumpFormat::string_only) {
        dumped_data().clear();
    }
    std::ostringstream output;
    if (items().empty()) {
        output << "No performance measurement data." << std::endl;
        return output.str();
    }

    using KeySetUnsorted = std::map<ItemNameAsKey, KeyData>;
    KeySetUnsorted keys_unsorted;
    std::transform(
        items().begin(), items().end(), std::inserter(keys_unsorted, keys_unsorted.begin()), KeyEntryMaker{});
    using KeyNameAndData = std::pair<ItemNameAsKey, KeyData>;
    using KeySet = std::vector<KeyNameAndData>;
    KeySet keys{keys_unsorted.begin(), keys_unsorted.end()};
    std::sort(keys.begin(), keys.end(), StartNrLess{});

    constexpr size_t column_width{10U};
    constexpr size_t column_width_huge{29U};

    output << std::left;
    output << std::setfill('-') << std::setw(static_cast<int>(column_width_huge + column_width * 5U)) << '-'
           << std::endl;
    output << std::setfill(' ');
    output << std::setw(static_cast<int>(column_width_huge)) << std::setprecision(static_cast<int>(column_width_huge))
           << "Item" << std::setw(static_cast<int>(column_width)) << std::setprecision(static_cast<int>(column_width))
           << "Count" << std::setw(static_cast<int>(column_width)) << std::setprecision(static_cast<int>(column_width))
           << "Total" << std::setw(static_cast<int>(column_width)) << std::setprecision(static_cast<int>(column_width))
           << "Average" << std::setw(static_cast<int>(column_width))
           << std::setprecision(static_cast<int>(column_width)) << "Mean" << std::setw(static_cast<int>(column_width))
           << std::setprecision(static_cast<int>(column_width)) << "StdDev" << std::endl;
    output << std::setfill('-') << std::setw(static_cast<int>(column_width_huge + column_width * 5U)) << '-'
           << std::endl;
    output << std::setfill(' ');

    for (const auto& [item_name, key_data] : keys) {
        const auto total_seconds =
            std::accumulate(items().begin(), items().end(), TimeValStorageRep{}, AccumKey{item_name});
        const auto count = std::count_if(items().begin(), items().end(), MatchKey{item_name});
        UL_ASSERT(count >= 0);
        auto average_seconds = TimeValStorageRep{};
        if (count) {
            average_seconds = total_seconds / static_cast<TimeValStorageRep>(count);
        } else {
            average_seconds = std::numeric_limits<double>::infinity();
        }

        std::vector<TimeValStorageRep> sorted_items;
        for (const auto& [current_item_name, current_item_data] : items()) {
            if (item_name == current_item_name) {
                sorted_items.push_back(current_item_data.time_val);
            }
        }
        std::sort(sorted_items.begin(), sorted_items.end());
        const auto mid = static_cast<size_t>(std::floor(static_cast<double>(count) / 2.0));
        const auto mean_seconds =
            (count > 1 && count % 2) ? (sorted_items[mid] + sorted_items[mid + 1U]) / 2.0 : sorted_items[mid];

        const auto variance_seconds = count > 1 ? impl_profiler_dump::variance(sorted_items, mean_seconds) : 0.0;
        const auto stddev_seconds = count > 1 ? std::sqrt(variance_seconds / (static_cast<double>(count) - 1.0)) : 0.0;

        const auto item_name_with_nesting_level =
            impl_profiler_dump::item_name_with_nesting(item_name, key_data.nesting_level, column_width_huge);
        output << std::setw(static_cast<int>(column_width_huge))
               << std::setprecision(static_cast<int>(column_width_huge)) << item_name_with_nesting_level
               << std::setw(static_cast<int>(column_width)) << std::setprecision(static_cast<int>(column_width))
               << count << std::setw(static_cast<int>(column_width))
               << std::setprecision(static_cast<int>(column_width)) << to_formatted_string(total_seconds)
               << std::setw(static_cast<int>(column_width)) << std::setprecision(static_cast<int>(column_width))
               << to_formatted_string(average_seconds) << std::setw(static_cast<int>(column_width))
               << std::setprecision(static_cast<int>(column_width)) << to_formatted_string(mean_seconds)
               << std::setw(static_cast<int>(column_width)) << std::setprecision(static_cast<int>(column_width))
               << to_formatted_string(stddev_seconds) << std::endl;
        if constexpr (fmt != DumpFormat::string_only) {
            dumped_data().push_back(
                {item_name_with_nesting_level,
                 static_cast<size_t>(count),
                 total_seconds,
                 average_seconds,
                 mean_seconds,
                 stddev_seconds});
        }
    }
    output << std::setfill('-') << std::setw(static_cast<int>(column_width_huge + column_width * 5U)) << '-'
           << std::endl;
    output << std::setfill(' ');
    return output.str();
}

// NOLINTEND

template <ProfilerClock Clock>
std::string PerformanceProfiler<Clock>::to_formatted_string(SecondsDbl seconds) {
    std::ostringstream output;
    auto absolute_seconds{seconds};
    if (absolute_seconds < 0.0) {
        absolute_seconds = -absolute_seconds;
        output << '-';
    }
    // NOLINTBEGIN
    if (absolute_seconds == 0.0 || absolute_seconds < 0.000000000099995) { // 0.0000000001
        output << std::setprecision(2) << std::fixed << math::round(absolute_seconds * 1000000000000.0, 2) << " ps";
    } else if (absolute_seconds < 0.000000099995) { // 0.0000001
        output << std::setprecision(2) << std::fixed << math::round(absolute_seconds * 1000000000.0, 2) << " ns";
    } else if (absolute_seconds < 0.000099995) { // 0.0001
        output << std::setprecision(2) << std::fixed << math::round(absolute_seconds * 1000000.0, 2) << " "
               << "\xC2\xB5"
               << "s";
    } else if (absolute_seconds < 0.099995) { // 0.1
        output << std::setprecision(2) << std::fixed << math::round(absolute_seconds * 1000.0, 2) << " ms";
    } else if (absolute_seconds < 59.995) { // 60.0
        output << std::setprecision(2) << std::fixed << math::round(absolute_seconds, 2) << " s";
    } else if (absolute_seconds < 3600.0) { // 3600.0
        output << std::setw(2) << std::setfill('0') << std::floor(absolute_seconds / 60.0) << ':' << std::setw(5)
               << std::fixed << std::setprecision(2) << std::setfill('0') << std::fmod(absolute_seconds, 60.0);
    } else if (absolute_seconds < 359999.0) {
        output << std::setw(2) << std::setfill('0') << std::floor(absolute_seconds / 3600.0) << ':' << std::setw(2)
               << std::setfill('0') << std::floor(std::fmod(absolute_seconds, 3600.0) / 60.0) << ':' << std::setw(2)
               << std::setfill('0') << std::setprecision(0) << std::fixed
               << std::floor(std::fmod(absolute_seconds, 60.0));
    } else {
        output << ">= 100 h";
    }
    // NOLINTEND
    return output.str();
}

template <ProfilerClock Clock>
void PerformanceProfiler<Clock>::reset() {
    items().clear();
    unique_item_start_nr() = UniqueItemStartNr{};
}

template <ProfilerClock Clock>
typename PerformanceProfiler<Clock>::Items& PerformanceProfiler<Clock>::items() {
    static Items instance;
    return instance;
}
} // namespace mb::ul

UL_HEADER_END

#endif
