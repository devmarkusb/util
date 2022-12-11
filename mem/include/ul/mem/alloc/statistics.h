// 2018

//! \file

#ifndef STATISTICS_H_eorxmgh9385g257xgishuv
#define STATISTICS_H_eorxmgh9385g257xgishuv

#include "ul/mem/types.h"
#include <optional>

#include "ul/macros.h"


namespace mb::ul
{
namespace mem
{
namespace alloc
{
class Statistics
{
public:
    std::optional<Bytes> peak() const
    {
        if (peak_)
            return peak_;
        else
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
    std::optional<Bytes> peak() const
    {
        return std::nullopt;
    }

protected:
    void statsCollect_currentSize(Bytes)
    {
        UL_NOOP;
    }

    void statsCollect_alloc(Bytes)
    {
        UL_NOOP;
    }

    void statsCollect_dealloc(Bytes)
    {
        UL_NOOP;
    }
};
} // namespace alloc
} // namespace mem
} // namespace mb::ul

#include "ul/macros_end.h"

#endif
