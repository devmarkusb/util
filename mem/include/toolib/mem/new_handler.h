// Markus Borris, 2011
// This file is part of tfl library.

//!
/**
*/
//! \file

#ifndef NEW_HANDLER_H_n2io38zre2387xnz27r
#define NEW_HANDLER_H_n2io38zre2387xnz27r

#include <cstddef>
#include <new>


namespace too
{
namespace mem
{
//! Provides a base class to equip arbitrary classes with their *own* new handler support.
/** Usage: \code class X : public too::NewHandlerSupport<X>{...}; \endcode
    Note: array allocation (new[]) not supported.*/
template <typename T>
class NewHandlerSupport
{
public:
    //! With this method you can assign a "void OutOfMem()" function to your class.
    /** This is much more convenient than wrapping every new-call in a try block.*/
    static inline std::new_handler set_new_handler(std::new_handler p);
//! Class specific new, responsible for setting up the custom new handler and resetting back to the global default.
#undef new
    static inline void* operator new(size_t size);

private:
    //! Class specific new handler.
    static std::new_handler& m_currentHandler()
    {
        static std::new_handler m_currentHandler_ = nullptr;
        return m_currentHandler_;
    }
};

//### Implementation details ###

template <typename T>
std::new_handler NewHandlerSupport<T>::set_new_handler(std::new_handler p)
{
    std::new_handler old = m_currentHandler();
    m_currentHandler() = p;
    return old;
}

template <typename T>
void* NewHandlerSupport<T>::operator new(size_t size)
{
    std::new_handler globalHandler = std::set_new_handler(m_currentHandler());
    void* memory;
    try
    {
        memory = ::operator new(size);
    }
    catch (std::bad_alloc&)
    {
        std::set_new_handler(globalHandler);
        throw;
    }
    std::set_new_handler(globalHandler);
    return memory;
}
}
}

#endif
