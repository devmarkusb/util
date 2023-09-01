//! \file

#ifndef STATISTICS_H_EORXMGH9385G257XGISHUV
#define STATISTICS_H_EORXMGH9385G257XGISHUV

#include "ul/macros.h"
#include "ul/mem/types.h"
#include <optional>

namespace mb::ul::mem::alloc {
class Statistics {
public:
    [[nodiscard]] std::optional<Bytes> peak() const {
        if (peak_)
            return peak_;
        return std::nullopt;
    }

protected:
    /** If you have data of the currently allocated size in total and just need to obtain the peak allocation size,
        it is sufficient to call this function (and this one even in allocation, size increasing, context only).
        Using the other two statsCollect_alloc/statsCollect_dealloc would make calling this one obsolete.*/
    void stats_collect_current_size(Bytes current_size) {
        if (current_size > peak_)
            peak_ = current_size;
    }

    void stats_collect_alloc(Bytes size) {
        curr_size_ += size;
        stats_collect_current_size(curr_size_);
    }

    void stats_collect_dealloc(Bytes size) {
        curr_size_ -= size;
    }

private:
    Bytes peak_;
    Bytes curr_size_;
};

class NoStatistics {
public:
    [[nodiscard]] static std::optional<Bytes> peak() {
        return std::nullopt;
    }

protected:
    static void stats_collect_current_size(Bytes /*unused*/) {
        UL_NOOP;
    }

    static void stats_collect_alloc(Bytes /*unused*/) {
        UL_NOOP;
    }

    static void stats_collect_dealloc(Bytes /*unused*/) {
        UL_NOOP;
    }
};
} // namespace mb::ul::mem::alloc

#endif
