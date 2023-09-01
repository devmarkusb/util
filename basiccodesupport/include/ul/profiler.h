//! \file

#ifndef PROFILER_H_958X2G58SDJKFH23789HGXT34
#define PROFILER_H_958X2G58SDJKFH23789HGXT34

#include "assert.h"
#include "round.h"
#include "type/non_copyable.h"

#include "ul/config.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iterator>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace mb::ul {
using ProfilerTimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

inline ProfilerTimePoint profiler_now() {
    return std::chrono::high_resolution_clock::now();
}

inline auto profiler_diff(ProfilerTimePoint start, ProfilerTimePoint end) {
    return end - start;
}

namespace impl_dump_all_items {
struct KeyData;
} // namespace impl_dump_all_items

//! Can only be used within one thread at the same time.
/** Usage: Cf. unit tests.*/
class PerformanceProfiler : private ul::NonCopyable {
public:
    using TimeValStorageRep = double;
    using SecondsDbl = TimeValStorageRep;
    using NestingLevel = unsigned int;

    //! \param nesting_level is just for dump visualization
    inline explicit PerformanceProfiler(std::string new_item_name, NestingLevel nesting_level = 0);
    inline ~PerformanceProfiler();
    [[nodiscard]] SecondsDbl elapsed_current_item() const;

    //! New item on same hierarchy/nesting level.
    inline void start_new_item(const std::string& new_item_name);
    inline void stop_item();

    enum class DumpFormat {
        string_only,
        string_and_structure,
    };
    //! Also fills dumpedData() if fmt is DumpFormat::stringAndStructure.
    template <DumpFormat fmt = DumpFormat::string_only>
    static std::string dump_all_items();
    static void reset();
    static std::string to_formatted_string(const SecondsDbl& d);

    //! Only for testing.
    struct DumpDataset {
        std::string item_name;
        size_t count;
        SecondsDbl total;
        SecondsDbl average;
        SecondsDbl mean;
        SecondsDbl std_dev;
    };

    //! Filled with structured data (e.g. for testing) if dumpAllItems() was called with DumpFormat::stringAndStructure.
    static std::vector<DumpDataset>& dumped_data() {
        static std::vector<DumpDataset> data;
        return data;
    }

private:
    friend struct impl_dump_all_items::KeyData;
    using UniqueItemStartNr = uint64_t;

    struct ItemData {
        TimeValStorageRep time_val{};
        NestingLevel nesting_level{};
        UniqueItemStartNr start_nr{};

        ItemData(const TimeValStorageRep& t, NestingLevel nl, UniqueItemStartNr n)
            : time_val(t)
            , nesting_level(nl)
            , start_nr(n) {
        }
    };

    using ItemNameAsKey = std::string;
    using Items = std::multimap<ItemNameAsKey, ItemData>;
    using ChronoClock = std::chrono::high_resolution_clock;
    using ChronoDuration =
        std::chrono::duration<TimeValStorageRep, std::ratio<1, 1>>; // means seconds stored with type TimeValStorage
    using ChronoTimepoint = std::chrono::time_point<ChronoClock, ChronoDuration>;

    ChronoTimepoint start_time_;
    ChronoTimepoint stop_time_;
    std::string item_name_;
    NestingLevel nesting_level_ = NestingLevel{};
    UniqueItemStartNr item_start_nr_ = UniqueItemStartNr{};

    inline void start_current_item();
    inline void stop_current_item();

    static UniqueItemStartNr& unique_item_start_nr() {
        static UniqueItemStartNr n = UniqueItemStartNr();
        return n;
    }

    static Items& items();

    struct MatchKey {
        explicit MatchKey(ItemNameAsKey key)
            : key_(std::move(key)) {
        }

        bool operator()(const Items::value_type& rhs) const {
            return key_ == rhs.first;
        }

    private:
        ItemNameAsKey key_;
    };

    struct AccumKey {
        explicit AccumKey(ItemNameAsKey key)
            : key_(std::move(key)) {
        }

        TimeValStorageRep operator()(const TimeValStorageRep& v, const Items::value_type& rhs) const {
            if (key_ == rhs.first)
                return rhs.second.time_val + v;
            return v;
        }

    private:
        ItemNameAsKey key_;
    };
};

inline void PerformanceProfiler::start_current_item() {
    item_start_nr_ = unique_item_start_nr()++;
    start_time_ = ChronoClock::now();
}

inline void PerformanceProfiler::stop_current_item() {
    ItemData d(elapsed_current_item(), nesting_level_, item_start_nr_);
    const auto& it = items().find(item_name_);
    // is the same item started and stopped a second time at least?
    if (it != items().end()) {
        // if on the same nesting level, we want to keep the order of a single run through this level;
        // we don't know which representative will be picked later to generate a data record
        if (nesting_level_ == it->second.nesting_level)
            d.start_nr = it->second.start_nr;
    }
    items().insert(std::make_pair(item_name_, d));
}

inline PerformanceProfiler::PerformanceProfiler(std::string new_item_name, NestingLevel nesting_level)
    : item_name_(std::move(new_item_name))
    , nesting_level_(nesting_level) {
    start_current_item();
}

inline PerformanceProfiler::SecondsDbl PerformanceProfiler::elapsed_current_item() const {
    const ChronoTimepoint now = ChronoClock::now();
    const ChronoDuration elapsed = now - start_time_;
    return elapsed.count();
}

inline PerformanceProfiler::~PerformanceProfiler() {
    stop_current_item();
}

inline void PerformanceProfiler::start_new_item(const std::string& new_item_name) {
    stop_current_item();
    item_name_ = new_item_name;
    start_current_item();
}

inline void PerformanceProfiler::stop_item() {
    stop_current_item();
}

namespace impl_dump_all_items {
struct KeyData {
    using NestingLevel = PerformanceProfiler::NestingLevel;
    using UniqueItemStartNr = PerformanceProfiler::UniqueItemStartNr;
    using ItemData = PerformanceProfiler::ItemData;

    NestingLevel nesting_level{};
    UniqueItemStartNr start_nr{};

    explicit KeyData(const ItemData& d)
        : nesting_level(d.nesting_level)
        , start_nr(d.start_nr) {
    }
};
} // namespace impl_dump_all_items

// NOLINTBEGIN
template <PerformanceProfiler::DumpFormat fmt>
std::string PerformanceProfiler::dump_all_items() {
    if constexpr (fmt != DumpFormat::string_only)
        dumped_data().clear();
    std::stringstream ret;
    if (items().empty()) {
        ret << "No performance measurement data." << std::endl;
        return ret.str();
    }

    using TKeySet_unsorted = std::map<ItemNameAsKey, impl_dump_all_items::KeyData>;
    TKeySet_unsorted keys_unsorted;
    std::transform(
        items().begin(), items().end(), std::inserter(keys_unsorted, keys_unsorted.begin()),
        [](decltype(*items().begin())& i) {
            return std::make_pair(i.first, impl_dump_all_items::KeyData(i.second));
        });
    using TKeyNameAndData = std::pair<ItemNameAsKey, impl_dump_all_items::KeyData>;
    using TKeySet = std::vector<TKeyNameAndData>;
    TKeySet keys(keys_unsorted.begin(), keys_unsorted.end());
    std::sort(keys.begin(), keys.end(), [](const TKeySet::value_type& k1, const TKeySet::value_type& k2) -> bool {
        return k1.second.start_nr < k2.second.start_nr;
    });

    static const size_t COLUMN_WIDTH = 10;
    static const size_t COLUMN_WIDTH_HUGE = 29;

    ret << std::left;
    ret << std::setfill('-') << std::setw(COLUMN_WIDTH_HUGE + COLUMN_WIDTH * 5) << '-' << std::endl;
    ret << std::setfill(' ');
    ret << std::setw(COLUMN_WIDTH_HUGE) << std::setprecision(COLUMN_WIDTH_HUGE) << "Item" << std::setw(COLUMN_WIDTH)
        << std::setprecision(COLUMN_WIDTH) << "Count" << std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH)
        << "Total" << std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << "Average" << std::setw(COLUMN_WIDTH)
        << std::setprecision(COLUMN_WIDTH) << "Mean" << std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH)
        << "StdDev" << std::endl;
    ret << std::setfill('-') << std::setw(COLUMN_WIDTH_HUGE + COLUMN_WIDTH * 5) << '-' << std::endl;
    ret << std::setfill(' ');

    for (const auto& key : keys) {
        const TimeValStorageRep totalT =
            ul::accumulate(items().begin(), items().end(), TimeValStorageRep(), AccumKey(key.first));
        const auto count = std::count_if(items().begin(), items().end(), MatchKey(key.first));
        UL_ASSERT(count >= 0);
        TimeValStorageRep avgT = 0.0;
        if (count)
            avgT = totalT / static_cast<TimeValStorageRep>(count);
        else
            avgT = std::numeric_limits<double>::infinity();

        std::vector<TimeValStorageRep> sortedItems;
        for (const auto& item : items()) {
            if (key.first == item.first)
                sortedItems.push_back(item.second.time_val);
        }
        std::sort(sortedItems.begin(), sortedItems.end());
        const auto mid = static_cast<size_t>(floor(static_cast<double>(count) / 2.0));
        const double meanT =
            (count > 1 && count % 2) ? (sortedItems[mid] + sortedItems[mid + 1]) / 2.0 : sortedItems[mid];

        double variance = 0.0;
        if (count > 1) {
            for (const auto& time : sortedItems)
                variance += pow(time - meanT, 2.0);
        }

        const double stddev = count > 1 ? sqrt(variance / (static_cast<double>(count) - 1.0)) : 0.0;

        std::stringstream ssItemNameWithSymbolizedNestingLevel;
        const NestingLevel NESTING_LEVELS_SYMBOLIZED_BY_SPACES = 20;
        for (NestingLevel nl = 1; nl <= key.second.nesting_level && nl <= NESTING_LEVELS_SYMBOLIZED_BY_SPACES; ++nl)
            ssItemNameWithSymbolizedNestingLevel << ' ';
        ssItemNameWithSymbolizedNestingLevel << key.first;

        std::string ItemNameWithSymbolizedNestingLevel(ssItemNameWithSymbolizedNestingLevel.str());
        if (ItemNameWithSymbolizedNestingLevel.length() >= COLUMN_WIDTH_HUGE)
            ItemNameWithSymbolizedNestingLevel.resize(COLUMN_WIDTH_HUGE - 1);
        ret << std::setw(COLUMN_WIDTH_HUGE) << std::setprecision(COLUMN_WIDTH_HUGE)
            << ItemNameWithSymbolizedNestingLevel << std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << count
            << std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << to_formatted_string(totalT)
            << std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << to_formatted_string(avgT)
            << std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << to_formatted_string(meanT)
            << std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << to_formatted_string(stddev) << std::endl;
        if constexpr (fmt != DumpFormat::string_only)
            dumped_data().push_back(
                {ItemNameWithSymbolizedNestingLevel, static_cast<size_t>(count), totalT, avgT, meanT, stddev});
    }
    ret << std::setfill('-') << std::setw(COLUMN_WIDTH_HUGE + COLUMN_WIDTH * 5) << '-' << std::endl;
    ret << std::setfill(' ');
    return ret.str();
}

// NOLINTEND

inline std::string PerformanceProfiler::to_formatted_string(const SecondsDbl& sd) {
    std::stringstream ret;
    SecondsDbl d(sd);
    if (d < 0.0) {
        d = -d;
        ret << '-';
    }
    // NOLINTBEGIN
    if (d == 0.0 || d < 0.000000000099995) // 0.0000000001
        ret << std::setprecision(2) << std::fixed << ul::math::round(d * 1000000000000.0, 2) << " ps";
    else if (d < 0.000000099995) // 0.0000001
        ret << std::setprecision(2) << std::fixed << ul::math::round(d * 1000000000.0, 2) << " ns";
    else if (d < 0.000099995) // 0.0001
        ret << std::setprecision(2) << std::fixed << ul::math::round(d * 1000000.0, 2) << " "
            << "\xC2\xB5"
            << "s";
    else if (d < 0.099995) // 0.1
        ret << std::setprecision(2) << std::fixed << ul::math::round(d * 1000.0, 2) << " ms";
    else if (d < 59.995) // 60.0
        ret << std::setprecision(2) << std::fixed << ul::math::round(d, 2) << " s";
    else if (d < 3600.0) // 3600.0
        ret << std::setw(2) << std::setfill('0') << floor(d / 60.0) << ':' << std::setw(5) << std::fixed
            << std::setprecision(2) << std::setfill('0') << fmod(d, 60.0);
    else if (d < 359999.0)
        ret << std::setw(2) << std::setfill('0') << floor(d / 3600.0) << ':' << std::setw(2) << std::setfill('0')
            << floor(fmod(d, 3600.0) / 60.0) << ':' << std::setw(2) << std::setfill('0') << std::setprecision(0)
            << std::fixed << floor(fmod(d, 60.0));
    else
        ret << ">= 100 h";
    // NOLINTEND
    return ret.str();
}

inline void PerformanceProfiler::reset() {
    items().clear();
    unique_item_start_nr() = UniqueItemStartNr{};
}

inline PerformanceProfiler::Items& PerformanceProfiler::items() {
    static Items instance;
    return instance;
}
} // namespace mb::ul

UL_HEADER_END

#endif
