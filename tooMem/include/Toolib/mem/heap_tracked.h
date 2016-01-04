// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/** Taken from Meyers' book.
*/
//! \file

namespace too
{

	class HeapTracked {                  // mixin class; keeps track of
	public:                              // ptrs returned from op. new

		class MissingAddress{};            // exception class; see below

		virtual ~HeapTracked() = 0 {}

		static void *operator new(size_t size)
		{
			void *memPtr = ::operator new(size);  // get the memory

			addresses.push_front(memPtr);         // put its address at
			// the front of the list
			return memPtr;
		}

		static void operator delete(void *ptr)
		{
			// get an "iterator" that identifies the list
			// entry containing ptr; see Item 35 for details
			list<RawAddress>::iterator it =
				find(addresses.begin(), addresses.end(), ptr);

			if (it != addresses.end()) {       // if an entry was found
				addresses.erase(it);             // remove the entry
				::operator delete(ptr);          // deallocate the memory
			} else {                           // otherwise
				throw MissingAddress();          // ptr wasn't allocated by
			}                                  // op. new, so throw an
		}                                    // exception

		bool isOnHeap() const
		{
			// get a pointer to the beginning of the memory
			// occupied by *this; see below for details
			const void *rawAddress = dynamic_cast<const void*>(this);

			// look up the pointer in the list of addresses
			// returned by operator new
			list<RawAddress>::iterator it =
				find(addresses.begin(), addresses.end(), rawAddress);

			return it != addresses.end();      // return whether it was
		}                                    // found

	private:
		typedef const void* RawAddress;
		static list<RawAddress> addresses;
	};

	list<RawAddress> HeapTracked::addresses;

	// HeapTracked's destructor is pure virtual to make the
	// class abstract (see Item E14). The destructor must still
	// be defined, however, so we provide this empty definition.
	//HeapTracked::~HeapTracked() {}


}