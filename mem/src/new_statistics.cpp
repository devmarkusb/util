#define UL_I_AM_SURE_TO_REPLACE_NEW_DELETE 1
#include "ul/mem/new_statistics.h"
#undef UL_I_AM_SURE_TO_REPLACE_NEW_DELETE

namespace ul = mb::ul;

void* operator new(std::size_t sizeInBytes)
{
    const auto p = reinterpret_cast<uint8_t*>(std::malloc(sizeof(ul::mem::StatsHeader) + sizeInBytes));
    if (!p)
        throw std::bad_alloc{};
    ul::mem::Statistics::instance().newCall(ul::mem::Bytes{sizeInBytes}, p);
    return p + sizeof(ul::mem::StatsHeader);
}

void operator delete(void* p) noexcept
{
    p = reinterpret_cast<uint8_t*>(p) - sizeof(ul::mem::StatsHeader);
    ul::mem::Statistics::instance().deleteCall(p);
    std::free(p);
}

void operator delete(void* p, size_t) noexcept
{
    p = reinterpret_cast<uint8_t*>(p) - sizeof(ul::mem::StatsHeader);
    ul::mem::Statistics::instance().deleteCall(p);
    std::free(p);
}
