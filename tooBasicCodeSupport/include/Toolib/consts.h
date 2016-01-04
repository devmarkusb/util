// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef CONSTS_H_INCL_cosndzcriwu4ezr3
#define CONSTS_H_INCL_cosndzcriwu4ezr3

namespace too
{
    //! Use these constants to exit your program by \code return \endcode out of \code main \endcode.
    /** (Do not use \code std::exit \endcode, because of possible circumvention of stack unwinding.)*/
    const int PROG_EXIT_SUCCESS = 0;
    const int PROG_EXIT_FAILURE = 1;
} // too

#endif
