// 2011-20

/** \file
    Most common and basic preprocessor defines.
    If you include this file within a header you should also include macros_end.h when finished.*/

// intentionally without include guard

#include "../macros.h"


#if UL_COMP_MS_VISUAL_STUDIO_CPP
// You shouldn't f***ing care restoring original defs for these...
#undef max
#undef min
#endif
