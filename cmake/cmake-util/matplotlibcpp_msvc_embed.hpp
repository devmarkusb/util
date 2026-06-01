#pragma once

// On MSVC Debug, pyconfig.h maps _DEBUG -> Py_DEBUG, which pulls in refcount helpers
// (Py_NegativeRefcount, Py_DECREF_DecRefTotal) that exist only in pythonXY_d.lib.
// Official/GHA Python installs ship release libs only, so embedders must not enable
// Py_DEBUG while still compiling their own code as Debug.
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MB_MATPLOTLIBCPP_MSVC_RELEASE_PYTHON)
#pragma push_macro("_DEBUG")
#undef _DEBUG
#endif

#include "matplotlibcpp.h"

#if defined(_MSC_VER) && defined(_DEBUG) && defined(MB_MATPLOTLIBCPP_MSVC_RELEASE_PYTHON)
#pragma pop_macro("_DEBUG")
#endif
