// Markus Borris, 2015
// This file is part of Toolib library. Open source.

//!
/** Taken from / inspired by the Cpp Guidelines Support Library GSL.
*/
//! \file

#ifndef NARROW_H_sk837xxgnf83xgfn3g8t34sdjn47
#define NARROW_H_sk837xxgnf83xgfn3g8t34sdjn47

#include "std/std_extensions.h"


namespace too
{
	//! Nothing else than a usual static_cast, but communicating intent of converting to a smaller type, assuming the value
	//! not to change at all.
	template <class T, class U>
	inline constexpr T narrow_cast(U u) noexcept
	{
		return static_cast<T>(u);
	}

	//! Exception thrown by narrow() when failing.
	struct narrowing_error : public std::exception {};

	//! Checked version of narrow_cast(), throwing \ref narrowing_error if the cast changed the value.
	template<class T, class U>
	inline T narrow(U u)
	{
		T t = narrow_cast<T>(u);
		if (static_cast<U>(t) != u)
			throw narrowing_error();
		return t;
	}
}

#endif
