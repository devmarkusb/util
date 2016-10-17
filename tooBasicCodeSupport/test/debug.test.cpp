// clang-format off
//#####!!!!! DO NOT change anything from HERE { (actually from the start of the clang format directive above) ######
#include "Toolib/debug.h"
#include <string>
#include "gtest/gtest.h"
namespace
{
std::string g_location_info_of__f_with_some_sourcefile_location_info; // just testing, so please don't cry about global data

void f_with_some_sourcefile_location_info()
{
    g_location_info_of__f_with_some_sourcefile_location_info = TOO_LOCATION;
}
}
//#####!!!!! TO HERE (actually to the end of the clang format directive below) } unless you also update test TOO_LOCATIONTest below! ######
// clang-format on

#include "Toolib/PPDEFS.h"

#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNING_4127_BEGIN"

namespace
{
void f_with_debug_break() { TOO_DEBUG_BREAK; }
void f_with_debug_break_if() { TOO_DEBUG_BREAK_IF(1 == 1); }
void f_with_debug_break_if_wont() { TOO_DEBUG_BREAK_IF(1 == 0); }
}

TEST(debug_hTest, test)
{
    EXPECT_DEBUG_DEATH(f_with_debug_break(), "");
    EXPECT_DEBUG_DEATH(f_with_debug_break_if(), "");
    EXPECT_NO_FATAL_FAILURE(f_with_debug_break_if_wont());
}

#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNING_END"

namespace
{
std::string g_sig_of__f_with_some_sig; // just testing, so please don't cry about global data
}

static const int* __cdecl f_with_some_sig(int, bool, long*, bool&)
{
    g_sig_of__f_with_some_sig = TOO_FUNCTIONSIGN;
    return nullptr;
}

TEST(TOO_FUNCTIONSIGNTest, test)
{
    bool b = false;
    f_with_some_sig(1, false, nullptr, b);
#if TOO_COMP_MS_VISUAL_STUDIO_CPP
    EXPECT_EQ("const int *__cdecl f_with_some_sig(int,bool,long *,bool &)", g_sig_of__f_with_some_sig);
#elif TOO_COMP_GNU_CPP || TOO_COMP_MINGW
    EXPECT_EQ("const int* f_with_some_sig(int, bool, long int*, bool&)", g_sig_of__f_with_some_sig);
#elif TOO_COMP_INTEL
    EXPECT_EQ("f_with_some_sig", g_sig_of__f_with_some_sig); // todo, don't know, what the compiler spits out
#elif TOO_COMP_BORLAND_CPP
    EXPECT_EQ("f_with_some_sig", g_sig_of__f_with_some_sig); // todo, don't know, what the compiler spits out
#else
    EXPECT_EQ("f_with_some_sig", g_sig_of__f_with_some_sig);
#endif
}

TEST(TOO_LOCATIONTest, test)
{
    f_with_some_sourcefile_location_info();
    size_t pos = g_location_info_of__f_with_some_sourcefile_location_info.find("debug.test.cpp (12) : ");
    EXPECT_NE(std::string::npos, pos);
}
