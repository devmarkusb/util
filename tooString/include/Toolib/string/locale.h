// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef LOCALE_H_INCL_condwhxnu23mhy8mr
#define LOCALE_H_INCL_condwhxnu23mhy8mr

#include <locale>

namespace too
{
void set_global_locale(const char* locname = "German_germany")
{
    std::locale::global(std::locale(locname));
}
} // too

#endif
