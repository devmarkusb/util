#define TOO_I_AM_SURE_TO_REPLACE_NEW_DELETE 1
#include "toolib/mem/new_statistics.h"
#undef TOO_I_AM_SURE_TO_REPLACE_NEW_DELETE


void* operator new(std::size_t sizeInBytes)
{
    const auto p = reinterpret_cast<uint8_t*>(std::malloc(sizeof(too::mem::StatsHeader) + sizeInBytes));
    if (!p)
        throw std::bad_alloc{};
    too::mem::Statistics::instance().newCall(too::mem::Bytes{sizeInBytes}, p);
    return p + sizeof(too::mem::StatsHeader);
}

void operator delete(void* p) noexcept
{
    p = reinterpret_cast<uint8_t*>(p) - sizeof(too::mem::StatsHeader);
    too::mem::Statistics::instance().deleteCall(p);
    std::free(p);
}

void operator delete(void* p, size_t) noexcept
{
    p = reinterpret_cast<uint8_t*>(p) - sizeof(too::mem::StatsHeader);
    too::mem::Statistics::instance().deleteCall(p);
    std::free(p);
}
