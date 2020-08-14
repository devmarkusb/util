// Markus Borris, 2011-20
// This file is part of tfl library.

//!
/** Most common and basic preprocessor defines.
 */
//! \file

#ifndef UNDEF_MIN_MAX_H_3t94xth3m89htx4
#define UNDEF_MIN_MAX_H_3t94xth3m89htx4

#include "../macros.h"


#if TOO_COMP_MS_VISUAL_STUDIO_CPP
// You shouldn't f***ing care restoring original defs for these...
#undef max
#undef min
#endif

#endif
