//! \file

#ifndef NEW_HANDLER_H_N2IO38ZRE2387XNZ27R
#define NEW_HANDLER_H_N2IO38ZRE2387XNZ27R

#include <cstddef>
#include <new>

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
#undef new
    static void* operator new(size_t size);

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
    std::new_handler old = current_handler();
    current_handler() = p;
    return old;
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
} // namespace mb::ul::mem

#endif
