//! \file

#ifndef STATISTICS_H_eorxmgh9385g257xgishuv
#define STATISTICS_H_eorxmgh9385g257xgishuv

#include "ul/macros.h"
#include "ul/mem/types.h"
#include <optional>

namespace mb::ul::mem::alloc
{
class Statistics
{
public:
    [[nodiscard]] std::optional<Bytes> peak() const
    {
        if (peak_)
            return peak_;
        return std::nullopt;
    }

protected:
    /** If you have data of the currently allocated size in total and just need to obtain the peak allocation size,
        it is sufficient to call this function (and this one even in allocation, size increasing, context only).
        Using the other two statsCollect_alloc/statsCollect_dealloc would make calling this one obsolete.*/
    void statsCollect_currentSize(Bytes currentSize)
    {
        if (currentSize > peak_)
            peak_ = currentSize;
    }

    void statsCollect_alloc(Bytes size)
    {
        currSize_ += size;
        statsCollect_currentSize(currSize_);
    }

    void statsCollect_dealloc(Bytes size)
    {
        currSize_ -= size;
    }

private:
    Bytes peak_;
    Bytes currSize_;
};

class NoStatistics
{
public:
    [[nodiscard]] static std::optional<Bytes> peak()
    {
        return std::nullopt;
    }

protected:
    static void statsCollect_currentSize(Bytes /*unused*/)
    {
        UL_NOOP;
    }

    static void statsCollect_alloc(Bytes /*unused*/)
    {
        UL_NOOP;
    }

    static void statsCollect_dealloc(Bytes /*unused*/)
    {
        UL_NOOP;
    }
};
} // namespace mb::ul::mem::alloc

#endif
