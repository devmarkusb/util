// Markus Borris, 2017
// This file is part of Toolib library.

//!
/** Cf. std_extensions.h
    */
//! \file

#ifndef STD_EXT_FILESYSTEM_H_wueihfiwufy3u48732t
#define STD_EXT_FILESYSTEM_H_wueihfiwufy3u48732t

#include "Toolib/PPDEFS.h"
#if TOO_OS_WINDOWS_UWP_APP && TOO_COMP_MS_VISUAL_STUDIO_CPP
#if TOO_COMP_MS_VS_VER != 1912
#error "this might need an adpation"
#endif
#include <experimental/filesystem>
namespace too
{
namespace std_fs = std::experimental::filesystem::v1;
}
#elif !TOO_IS_BOOST_DISABLED
#include "Toolib/PPDefs/GCC/SUPPRESS_WARNING_deprecated-declarations_BEGIN"
#include "Toolib/PPDefs/GCC/SUPPRESS_WARNING_unused-variable_BEGIN"
#include <boost/filesystem/operations.hpp>
#include "Toolib/PPDefs/GCC/SUPPRESS_WARNING_END"
#include <boost/filesystem/path.hpp>
namespace too
{
namespace std_fs = boost::filesystem;
}
#endif

#endif
