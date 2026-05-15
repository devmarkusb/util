//! \file

#ifndef NEW_HANDLER_H_N2IO38ZRE2387XNZ27R
#define NEW_HANDLER_H_N2IO38ZRE2387XNZ27R

#include "mb/ul/buildenv/warnings.hpp"
#include <cstddef>
#include <new>
#include <utility>

namespace mb::ul::mem {
//! Provides a base class to equip arbitrary classes with their *own* new handler support.
/** Usage: \code class X : public ul::NewHandlerSupport<X>{...}; \endcode
    Note: array allocation (new[]) not supported.*/
template <typename T>
class NewHandlerSupport {
public:
    //! With this method you can assign a "void OutOfMem()" function to your class.
    /** This is much more convenient than wrapping every new-call in a try block.*/
    static std::new_handler set_new_handler(std::new_handler p);
    //! Class specific new, responsible for setting up the custom new handler and resetting back to the global default.
    UL_PRAGMA_WARNINGS_PUSH
    // clang-format off
    UL_WARNING_DISABLE_GCC(keyword-macro)
    // clang-format on
    #undef new
    UL_PRAGMA_WARNINGS_POP
    static void* operator new(size_t size);
    static void operator delete(void* ptr) noexcept;
    static void operator delete(void* ptr, size_t size) noexcept;

private:
    //! Class specific new handler.
    static std::new_handler& current_handler() {
        static std::new_handler the_handler = nullptr;
        return the_handler;
    }
};

//### Implementation details ###

template <typename T>
std::new_handler NewHandlerSupport<T>::set_new_handler(std::new_handler p) {
    return std::exchange(current_handler(), p);
}

template <typename T>
void* NewHandlerSupport<T>::operator new(size_t size) {
    std::new_handler global_handler = std::set_new_handler(current_handler());
    void* memory{};
    try {
        memory = ::operator new(size);
    } catch (std::bad_alloc&) {
        std::set_new_handler(global_handler);
        throw;
    }
    std::set_new_handler(global_handler);
    return memory;
}

template <typename T>
void NewHandlerSupport<T>::operator delete(void* ptr) noexcept {
    ::operator delete(ptr);
}

template <typename T>
void NewHandlerSupport<T>::operator delete(void* ptr, size_t /*size*/) noexcept {
    ::operator delete(ptr);
}
} // namespace mb::ul::mem

#endif
