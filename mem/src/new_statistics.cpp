#define UL_I_AM_SURE_TO_REPLACE_NEW_DELETE 1
#include "ul/mem/new_statistics.h"
#undef UL_I_AM_SURE_TO_REPLACE_NEW_DELETE

namespace ul = mb::ul;

void* operator new(std::size_t size_in_bytes) {
    auto* const p = reinterpret_cast<uint8_t*>(std::malloc(sizeof(ul::mem::StatsHeader) + size_in_bytes)); // NOLINT
    if (!p)
        throw std::bad_alloc{};
    ul::mem::Statistics::instance().new_call(ul::mem::Bytes{size_in_bytes}, p);
    return p + sizeof(ul::mem::StatsHeader); // NOLINT
}

void operator delete(void* p) noexcept {
    p = reinterpret_cast<uint8_t*>(p) - sizeof(ul::mem::StatsHeader); // NOLINT
    ul::mem::Statistics::instance().delete_call(p);
    std::free(p); // NOLINT
}

void operator delete(void* p, size_t /*unused*/) noexcept {
    p = reinterpret_cast<uint8_t*>(p) - sizeof(ul::mem::StatsHeader); // NOLINT
    ul::mem::Statistics::instance().delete_call(p);
    std::free(p); // NOLINT
}
