// Markus Borris, 2011-20
// This file is part of tfl library.

//!
/** Most common and basic preprocessor defines.
 *   If you include this file within a header you should also include macros_end.h when finished.
 */
//! \file

// intentionally without include guard

#include "../macros.h"


#if TOO_COMP_MS_VISUAL_STUDIO_CPP
// You shouldn't f***ing care restoring original defs for these...
#undef max
#undef min
#endif

#include "../macros_end.h"
