// Markus Borris, 2017
// This file is part of tfl library.

//!
/** Cf. std_extensions.h
    */
//! \file

#ifndef STD_EXT_FILESYSTEM_H_wueihfiwufy3u48732t
#define STD_EXT_FILESYSTEM_H_wueihfiwufy3u48732t

#include "toolib/PPDEFS.h"

#include <filesystem>
namespace too
{
#if TOO_COMP_MS_VISUAL_STUDIO_CPP && TOO_COMP_MS_VS_VER <= 1914
namespace std_fs = std::experimental::filesystem;
#else
namespace std_fs = std::filesystem;
#endif
}

#endif
