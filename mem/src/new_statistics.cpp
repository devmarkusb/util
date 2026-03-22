#include "ul/mem/new_statistics.h"
#include "ul/mem/types.h"
#include "ul/warnings.h"
#include <cstddef>
#include <new>

namespace ul = mb::ul;

namespace {
[[nodiscard]] void* allocate_with_stats(std::size_t size_in_bytes) noexcept {
    auto* const p = reinterpret_cast<uint8_t*>(
        std::malloc(sizeof(ul::mem::StatsHeader) + size_in_bytes)); // NOLINT
    if (!p)
        return nullptr;
    ul::mem::Statistics::instance().new_call(ul::mem::Bytes{size_in_bytes}, p);
    return p + sizeof(ul::mem::StatsHeader); // NOLINT
}
} // namespace

UL_PRAGMA_WARNINGS_PUSH
// clang-format off
UL_WARNING_DISABLE_CLANG(unsafe-buffer-usage)
// clang-format on

void* operator new(std::size_t size_in_bytes) {
    void* const p = allocate_with_stats(size_in_bytes);
    if (!p)
        throw std::bad_alloc{};
    return p;
}

void* operator new(std::size_t size_in_bytes, const std::nothrow_t&) noexcept {
    return allocate_with_stats(size_in_bytes);
}

void* operator new[](std::size_t size_in_bytes) {
    void* const p = allocate_with_stats(size_in_bytes);
    if (!p)
        throw std::bad_alloc{};
    return p;
}

void* operator new[](std::size_t size_in_bytes, const std::nothrow_t&) noexcept {
    return allocate_with_stats(size_in_bytes);
}

void operator delete(void* p) noexcept {
    if (!p)
        return;
    p = reinterpret_cast<uint8_t*>(p) - sizeof(ul::mem::StatsHeader); // NOLINT
    ul::mem::Statistics::instance().delete_call(p);
    std::free(p); // NOLINT
}

void operator delete[](void* p) noexcept {
    if (!p)
        return;
    p = reinterpret_cast<uint8_t*>(p) - sizeof(ul::mem::StatsHeader); // NOLINT
    ul::mem::Statistics::instance().delete_call(p);
    std::free(p); // NOLINT
}

// some compiler complains about no prev. prototype otherwise
void operator delete(void* p, std::size_t /*unused*/) noexcept;

void operator delete(void* p, std::size_t /*unused*/) noexcept {
    if (!p)
        return;
    p = reinterpret_cast<uint8_t*>(p) - sizeof(ul::mem::StatsHeader); // NOLINT
    ul::mem::Statistics::instance().delete_call(p);
    std::free(p); // NOLINT
}

// some compiler complains about no prev. prototype otherwise
void operator delete[](void* p, std::size_t /*unused*/) noexcept;

void operator delete[](void* p, std::size_t /*unused*/) noexcept {
    if (!p)
        return;
    p = reinterpret_cast<uint8_t*>(p) - sizeof(ul::mem::StatsHeader); // NOLINT
    ul::mem::Statistics::instance().delete_call(p);
    std::free(p); // NOLINT
}

UL_PRAGMA_WARNINGS_POP
