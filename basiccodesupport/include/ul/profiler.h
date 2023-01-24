//! \file

#ifndef PROFILER_H_958x2g58sdjkfh23789hgxt34
#define PROFILER_H_958x2g58sdjkfh23789hgxt34

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

namespace mb::ul
{
using ProfilerTimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

inline ProfilerTimePoint profiler_now()
{
    return std::chrono::high_resolution_clock::now();
}

inline auto profiler_diff(ProfilerTimePoint start, ProfilerTimePoint end)
{
    return end - start;
}

namespace impl_dump_all_items
{
struct KeyData;
} // namespace impl_dump_all_items

//! Can only be used within one thread at the same time.
/** Usage: Cf. unit tests.*/
class PerformanceProfiler : private ul::NonCopyable
{
public:
    using TimeValStorageRep = double;
    using SecondsDbl = TimeValStorageRep;
    using NestingLevel = unsigned int;

    //! \param NestingLevel is just for dump visualization
    inline explicit PerformanceProfiler(std::string NewItemName, NestingLevel NestingLevel = 0);
    inline ~PerformanceProfiler();
    [[nodiscard]] SecondsDbl elapsed_currentItem() const;

    //! New item on same hierarchy/nesting level.
    inline void startNewItem(const std::string& NewItemName);
    inline void stopItem();

    enum class DumpFormat
    {
        stringOnly,
        stringAndStructure,
    };
    //! Also fills dumpedData() if fmt is DumpFormat::stringAndStructure.
    template <DumpFormat fmt = DumpFormat::stringOnly>
    static std::string dumpAllItems();
    static void reset();
    static std::string toFormattedString(const SecondsDbl& d);

    //! Only for testing.
    struct DumpDataset
    {
        std::string itemName_;
        size_t count_;
        SecondsDbl total_;
        SecondsDbl average_;
        SecondsDbl mean_;
        SecondsDbl stdDev_;
    };

    //! Filled with structured data (e.g. for testing) if dumpAllItems() was called with DumpFormat::stringAndStructure.
    static std::vector<DumpDataset>& dumpedData()
    {
        static std::vector<DumpDataset> data;
        return data;
    }

private:
    friend struct impl_dump_all_items::KeyData;
    using UniqueItemStartNr = uint64_t;

    struct ItemData
    {
        TimeValStorageRep timeVal_{};
        NestingLevel nestingLevel_{};
        UniqueItemStartNr startNr_{};

        ItemData(const TimeValStorageRep& t, NestingLevel nl, UniqueItemStartNr n)
            : timeVal_(t)
            , nestingLevel_(nl)
            , startNr_(n)
        {
        }
    };

    using ItemNameAsKey = std::string;
    using Items = std::multimap<ItemNameAsKey, ItemData>;
    using chrono_clock = std::chrono::high_resolution_clock;
    using chrono_duration =
        std::chrono::duration<TimeValStorageRep, std::ratio<1, 1>>; // means seconds stored with type TimeValStorage
    using chrono_timepoint = std::chrono::time_point<chrono_clock, chrono_duration>;

    chrono_timepoint startTime_;
    chrono_timepoint stopTime_;
    std::string itemName_;
    NestingLevel nestingLevel_ = NestingLevel{};
    UniqueItemStartNr itemStartNr_ = UniqueItemStartNr{};

    inline void startCurrentItem();
    inline void stopCurrentItem();

    static UniqueItemStartNr& uniqueItemStartNr()
    {
        static UniqueItemStartNr n = UniqueItemStartNr();
        return n;
    }

    static Items& items();

    struct match_key
    {
        explicit match_key(ItemNameAsKey key)
            : key_(std::move(key))
        {
        }

        bool operator()(const Items::value_type& rhs) const
        {
            return key_ == rhs.first;
        }

    private:
        ItemNameAsKey key_;
    };

    struct accum_key
    {
        explicit accum_key(ItemNameAsKey key)
            : key_(std::move(key))
        {
        }

        TimeValStorageRep operator()(const TimeValStorageRep& v, const Items::value_type& rhs) const
        {
            if (key_ == rhs.first)
                return rhs.second.timeVal_ + v;
            return v;
        }

    private:
        ItemNameAsKey key_;
    };
};

inline void PerformanceProfiler::startCurrentItem()
{
    itemStartNr_ = uniqueItemStartNr()++;
    startTime_ = chrono_clock::now();
}

inline void PerformanceProfiler::stopCurrentItem()
{
    ItemData d(elapsed_currentItem(), nestingLevel_, itemStartNr_);
    const auto& it = items().find(itemName_);
    // is the same item started and stopped a second time at least?
    if (it != items().end())
    {
        // if on the same nesting level, we want to keep the order of a single run through this level;
        // we don't know which representative will be picked later to generate a data record
        if (nestingLevel_ == it->second.nestingLevel_)
            d.startNr_ = it->second.startNr_;
    }
    items().insert(std::make_pair(itemName_, d));
}

inline PerformanceProfiler::PerformanceProfiler(std::string NewItemName, NestingLevel NestingLevel)
    : itemName_(std::move(NewItemName))
    , nestingLevel_(NestingLevel)
{
    startCurrentItem();
}

inline PerformanceProfiler::SecondsDbl PerformanceProfiler::elapsed_currentItem() const
{
    const chrono_timepoint now = chrono_clock::now();
    const chrono_duration elapsed = now - startTime_;
    return elapsed.count();
}

inline PerformanceProfiler::~PerformanceProfiler()
{
    stopCurrentItem();
}

inline void PerformanceProfiler::startNewItem(const std::string& NewItemName)
{
    stopCurrentItem();
    itemName_ = NewItemName;
    startCurrentItem();
}

inline void PerformanceProfiler::stopItem()
{
    stopCurrentItem();
}

namespace impl_dump_all_items
{
struct KeyData
{
    using NestingLevel = PerformanceProfiler::NestingLevel;
    using UniqueItemStartNr = PerformanceProfiler::UniqueItemStartNr;
    using ItemData = PerformanceProfiler::ItemData;

    NestingLevel nestingLevel_{};
    UniqueItemStartNr startNr_{};

    explicit KeyData(const ItemData& d)
        : nestingLevel_(d.nestingLevel_)
        , startNr_(d.startNr_)
    {
    }
};
} // namespace impl_dump_all_items

template <PerformanceProfiler::DumpFormat fmt>
std::string PerformanceProfiler::dumpAllItems()
{
    if constexpr (fmt != DumpFormat::stringOnly)
        dumpedData().clear();
    std::stringstream ret;
    if (items().empty())
    {
        ret << "No performance measurement data." << std::endl;
        return ret.str();
    }

    using TKeySet_unsorted = std::map<ItemNameAsKey, impl_dump_all_items::KeyData>;
    TKeySet_unsorted keys_unsorted;
    std::transform(
        items().begin(), items().end(), std::inserter(keys_unsorted, keys_unsorted.begin()),
        [](decltype(*items().begin())& i)
        {
            return std::make_pair(i.first, impl_dump_all_items::KeyData(i.second));
        });
    using TKeyNameAndData = std::pair<ItemNameAsKey, impl_dump_all_items::KeyData>;
    using TKeySet = std::vector<TKeyNameAndData>;
    TKeySet keys(keys_unsorted.begin(), keys_unsorted.end());
    std::sort(
        keys.begin(), keys.end(),
        [](const TKeySet::value_type& k1, const TKeySet::value_type& k2) -> bool
        {
            return k1.second.startNr_ < k2.second.startNr_;
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

    for (const auto& key : keys)
    {
        const TimeValStorageRep totalT =
            ul::accumulate(items().begin(), items().end(), TimeValStorageRep(), accum_key(key.first));
        const auto count = std::count_if(items().begin(), items().end(), match_key(key.first));
        UL_ASSERT(count >= 0);
        TimeValStorageRep avgT = 0.0;
        if (count)
            avgT = totalT / static_cast<TimeValStorageRep>(count);
        else
            avgT = std::numeric_limits<double>::infinity();

        std::vector<TimeValStorageRep> sortedItems;
        for (const auto& item : items())
        {
            if (key.first == item.first)
                sortedItems.push_back(item.second.timeVal_);
        }
        std::sort(sortedItems.begin(), sortedItems.end());
        const auto mid = static_cast<size_t>(floor(static_cast<double>(count) / 2.0));
        const double meanT =
            (count > 1 && count % 2) ? (sortedItems[mid] + sortedItems[mid + 1]) / 2.0 : sortedItems[mid];

        double variance = 0.0;
        if (count > 1)
        {
            for (const auto& time : sortedItems)
                variance += pow(time - meanT, 2.0);
        }

        const double stddev = count > 1 ? sqrt(variance / (static_cast<double>(count) - 1.0)) : 0.0;

        std::stringstream ssItemNameWithSymbolizedNestingLevel;
        const NestingLevel NESTING_LEVELS_SYMBOLIZED_BY_SPACES = 20;
        for (NestingLevel nl = 1; nl <= key.second.nestingLevel_ && nl <= NESTING_LEVELS_SYMBOLIZED_BY_SPACES; ++nl)
            ssItemNameWithSymbolizedNestingLevel << ' ';
        ssItemNameWithSymbolizedNestingLevel << key.first;

        std::string ItemNameWithSymbolizedNestingLevel(ssItemNameWithSymbolizedNestingLevel.str());
        if (ItemNameWithSymbolizedNestingLevel.length() >= COLUMN_WIDTH_HUGE)
            ItemNameWithSymbolizedNestingLevel.resize(COLUMN_WIDTH_HUGE - 1);
        ret << std::setw(COLUMN_WIDTH_HUGE) << std::setprecision(COLUMN_WIDTH_HUGE)
            << ItemNameWithSymbolizedNestingLevel << std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << count
            << std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << toFormattedString(totalT)
            << std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << toFormattedString(avgT)
            << std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << toFormattedString(meanT)
            << std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << toFormattedString(stddev) << std::endl;
        if constexpr (fmt != DumpFormat::stringOnly)
            dumpedData().push_back(
                {ItemNameWithSymbolizedNestingLevel, static_cast<size_t>(count), totalT, avgT, meanT, stddev});
    }
    ret << std::setfill('-') << std::setw(COLUMN_WIDTH_HUGE + COLUMN_WIDTH * 5) << '-' << std::endl;
    ret << std::setfill(' ');
    return ret.str();
}

inline std::string PerformanceProfiler::toFormattedString(const SecondsDbl& d)
{
    std::stringstream ret;
    SecondsDbl d_(d);
    if (d_ < 0.0)
    {
        d_ = -d_;
        ret << '-';
    }
    if (d_ == 0.0 || d_ < 0.000000000099995) // 0.0000000001
        ret << std::setprecision(2) << std::fixed << ul::math::round(d_ * 1000000000000.0, 2) << " ps";
    else if (d_ < 0.000000099995) // 0.0000001
        ret << std::setprecision(2) << std::fixed << ul::math::round(d_ * 1000000000.0, 2) << " ns";
    else if (d_ < 0.000099995) // 0.0001
        ret << std::setprecision(2) << std::fixed << ul::math::round(d_ * 1000000.0, 2) << " "
            << "\xC2\xB5"
            << "s";
    else if (d_ < 0.099995) // 0.1
        ret << std::setprecision(2) << std::fixed << ul::math::round(d_ * 1000.0, 2) << " ms";
    else if (d_ < 59.995) // 60.0
        ret << std::setprecision(2) << std::fixed << ul::math::round(d_, 2) << " s";
    else if (d_ < 3600.0) // 3600.0
        ret << std::setw(2) << std::setfill('0') << floor(d_ / 60.0) << ':' << std::setw(5) << std::fixed
            << std::setprecision(2) << std::setfill('0') << fmod(d_, 60.0);
    else if (d_ < 359999.0)
        ret << std::setw(2) << std::setfill('0') << floor(d_ / 3600.0) << ':' << std::setw(2) << std::setfill('0')
            << floor(fmod(d_, 3600.0) / 60.0) << ':' << std::setw(2) << std::setfill('0') << std::setprecision(0)
            << std::fixed << floor(fmod(d_, 60.0));
    else
        ret << ">= 100 h";
    return ret.str();
}

inline void PerformanceProfiler::reset()
{
    items().clear();
    uniqueItemStartNr() = UniqueItemStartNr{};
}

inline PerformanceProfiler::Items& PerformanceProfiler::items()
{
    static Items instance;
    return instance;
}
} // namespace mb::ul

UL_HEADER_END

#endif
