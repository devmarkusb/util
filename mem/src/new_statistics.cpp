#include "ul/mem/types.h"
#include "ul/warnings.h"
#include <cstddef>
#include <new>
#define UL_I_AM_SURE_TO_REPLACE_NEW_DELETE 1
#include "ul/mem/new_statistics.h"
#undef UL_I_AM_SURE_TO_REPLACE_NEW_DELETE

namespace ul = mb::ul;

UL_PRAGMA_WARNINGS_PUSH
// clang-format off
UL_WARNING_DISABLE_CLANG(unsafe-buffer-usage)
// clang-format on

void* operator new(size_t size_in_bytes) {
    auto* const p = reinterpret_cast<uint8_t*>(std::malloc(sizeof(ul::mem::StatsHeader) + size_in_bytes)); // NOLINT
    if (!p)
        throw std::bad_alloc{};
    ul::mem::Statistics::instance().new_call(ul::mem::Bytes{size_in_bytes}, p);
    return p + sizeof(ul::mem::StatsHeader); // NOLINT
}

void* operator new[](size_t size_in_bytes) {
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

void operator delete[](void* p) noexcept {
    p = reinterpret_cast<uint8_t*>(p) - sizeof(ul::mem::StatsHeader); // NOLINT
    ul::mem::Statistics::instance().delete_call(p);
    std::free(p); // NOLINT
}

// some compiler complains about no prev. prototype otherwise
void operator delete(void* p, size_t /*unused*/) noexcept;

void operator delete(void* p, size_t /*unused*/) noexcept {
    p = reinterpret_cast<uint8_t*>(p) - sizeof(ul::mem::StatsHeader); // NOLINT
    ul::mem::Statistics::instance().delete_call(p);
    std::free(p); // NOLINT
}

// some compiler complains about no prev. prototype otherwise
void operator delete[](void* p, size_t /*unused*/) noexcept;

void operator delete[](void* p, size_t /*unused*/) noexcept {
    p = reinterpret_cast<uint8_t*>(p) - sizeof(ul::mem::StatsHeader); // NOLINT
    ul::mem::Statistics::instance().delete_call(p);
    std::free(p); // NOLINT
}

UL_PRAGMA_WARNINGS_POP
