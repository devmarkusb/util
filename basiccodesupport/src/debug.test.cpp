// clang-format off
//#####!!!!! DO NOT change anything from HERE { (actually from the start of the clang format directive above) ######
#include "ul/debug.h"
#include <string>
#include "gtest/gtest.h"
namespace
{
std::string g_location_info_of_f_with_some_sourcefile_location_info; // NOLINT // just testing, so please don't cry about global data

void f_with_some_sourcefile_location_info()
{
    g_location_info_of_f_with_some_sourcefile_location_info = UL_LOCATION;
}
} // namespace
//#####!!!!! TO HERE (actually to the end of the clang format directive below) } unless you also update test UL_LOCATIONTest below! ######
// clang-format on

#include "ul/macros.h"
#include "ul/warnings.h"

UL_PRAGMA_WARNINGS_PUSH
UL_WARNING_DISABLE_MSVC(4127)

namespace
{
void f_with_debug_break()
{
    UL_DEBUG_BREAK; // NOLINT
}

void f_with_debug_break_if()
{
    UL_DEBUG_BREAK_IF(1 == 1); // NOLINT
}

void f_with_debug_break_if_wont()
{
    UL_DEBUG_BREAK_IF(1 == 0); // NOLINT
}
} // namespace

TEST(debug_hTest, test)
{
#if GTEST_HAS_DEATH_TEST
    EXPECT_DEBUG_DEATH(f_with_debug_break(), "");
    EXPECT_DEBUG_DEATH(f_with_debug_break_if(), "");
#endif
    EXPECT_NO_FATAL_FAILURE(f_with_debug_break_if_wont());
}

UL_PRAGMA_WARNINGS_POP

namespace
{
std::string g_sig_of_f_with_some_sig; // NOLINT // just testing, so please don't cry about global data
} // namespace

static const int* __cdecl f_with_some_sig(int /*unused*/, bool /*unused*/, int64_t* /*unused*/, bool& /*unused*/)
{
    g_sig_of_f_with_some_sig = UL_FUNCTIONSIGN; // NOLINT
    return nullptr;
}

TEST(UL_FUNCTIONSIGNTest, test)
{
    bool b = false;
    f_with_some_sig(1, false, nullptr, b);
#if UL_COMP_MS_VISUAL_STUDIO_CPP
    std::cout << "g_sig_of_f_with_some_sig: " << g_sig_of_f_with_some_sig;
    EXPECT_TRUE(
        "const int *__cdecl f_with_some_sig(int,bool,int64_t *,bool &)" == g_sig_of_f_with_some_sig
        || "const int *__cdecl f_with_some_sig(int, bool, int64_t *, bool &)" == g_sig_of_f_with_some_sig);
#elif UL_COMP_CLANG
    EXPECT_EQ("const int *f_with_some_sig(int, bool, int64_t *, bool &)", g_sig_of_f_with_some_sig);
#elif UL_COMP_GNU_CPP || UL_COMP_MINGW
    EXPECT_EQ("const int* f_with_some_sig(int, bool, int64_t*, bool&)", g_sig_of_f_with_some_sig);
#elif UL_COMP_INTEL
    EXPECT_EQ("f_with_some_sig", g_sig_of_f_with_some_sig); // untested, don't know, what the compiler spits out
#elif UL_COMP_BORLAND_CPP
    EXPECT_EQ("f_with_some_sig", g_sig_of_f_with_some_sig); // untested, don't know, what the compiler spits out
#else
    EXPECT_EQ("f_with_some_sig", g_sig_of_f_with_some_sig);
#endif
}

TEST(UL_LOCATIONTest, test)
{
    f_with_some_sourcefile_location_info();
    const size_t pos = g_location_info_of_f_with_some_sourcefile_location_info.find("debug.test.cpp (12)");
    EXPECT_NE(std::string::npos, pos);
}
