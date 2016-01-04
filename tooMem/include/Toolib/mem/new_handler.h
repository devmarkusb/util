// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef NEW_HANDLER_H_INCL_n2io38zre2387xnz27r
#define NEW_HANDLER_H_INCL_n2io38zre2387xnz27r

#include <new>

namespace too
{
	//! Provides a base class to equip arbitrary classes with their *own* new handler support.
	/** Usage: \code class X : public too::NewHandlerSupport<X>{...}; \endcode*/
	template <typename T> class NewHandlerSupport
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
		static std::new_handler m_currentHandler;
	};

	// === Implementation details ===

	template <typename T> std::new_handler NewHandlerSupport<T>::set_new_handler(std::new_handler p)
	{
		std::new_handler old = m_currentHandler;
		m_currentHandler = p;
		return old;
	}

	template <typename T> void* NewHandlerSupport<T>::operator new(size_t size)
	{
		std::new_handler globalHandler = std::set_new_handler(m_currentHandler);
		void* memory;
		try { memory = ::operator new(size); }
		catch (std::bad_alloc&)
		{
			std::set_new_handler(globalHandler);
			throw;
		}
		std::set_new_handler(globalHandler);
		return memory;
	}

	// Initialize current handler with 0.
	template <typename T> std::new_handler NewHandlerSupport<T>::m_currentHandler;
}

#endif
