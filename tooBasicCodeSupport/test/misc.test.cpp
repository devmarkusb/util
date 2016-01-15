//#####!!!!! DO NOT change anything from HERE { ######
#include "gtest/gtest.h"
#include <string>
#include "Toolib/debug.h"
namespace
{
	std::string g_location_info_of__f_with_some_sourcefile_location_info; // just testing, so please don't cry about global data

	void f_with_some_sourcefile_location_info()
	{
		g_location_info_of__f_with_some_sourcefile_location_info = TOO_LOCATION;
	}
}
//#####!!!!! TO HERE } unless you also update test TOO_LOCATIONTest below! ######


#include <typeinfo>
#include "Toolib/types.h"
#include "Toolib/trace.h"
#include "Toolib/ptr2ref.h"
#include "Toolib/enum_cast.h"
#include "Toolib/debug.h"
#include "Toolib/error.h"
#include "Toolib/c_array.h"
#include "Toolib/PPDEFS.h"
#include "Toolib/std/std_extensions.h"


TEST(tooTypesDefinitionTest, correctness)
{
	EXPECT_TRUE(typeid(too::uchar) == typeid(unsigned char));
	EXPECT_TRUE(typeid(too::byte) == typeid(unsigned char));
	EXPECT_TRUE(typeid(too::schar) == typeid(signed char));
	EXPECT_TRUE(typeid(too::ushort) == typeid(unsigned short));
	EXPECT_TRUE(typeid(too::sshort) == typeid(signed short));
	EXPECT_TRUE(typeid(too::uint) == typeid(unsigned int));
	EXPECT_TRUE(typeid(too::sint) == typeid(signed int));
	EXPECT_TRUE(typeid(too::ulong) == typeid(unsigned long));
	EXPECT_TRUE(typeid(too::slong) == typeid(signed long));
}

TEST(ptr2refTest, test)
{
	int i = 5;
	int* pi = &i;
	int& refi = too::Ptr2Ref(pi);
	EXPECT_TRUE(&refi == pi);
	const int& crefi = too::Ptr2RefC(pi);
	EXPECT_TRUE(&crefi == pi);
}

TEST(EnumCast_as_numberTest, test)
{
	enum class EC
	{
		zeroth,
		first,
		second,
	};
	auto number = too::as_number(EC::first);
	EXPECT_EQ(1, number);
	enum class E
	{
		zeroth,
		first,
		second,
	};
	auto number_ = too::as_number(E::first);
	EXPECT_EQ(1, number_);
}

TEST(EnumCast_as_numberTest, TypeSpecific)
{
	enum class EC : unsigned long
	{
		zeroth,
		first,
		second,
	};
	auto number = too::as_number(EC::first);
	EXPECT_EQ(1ul, number);
	EXPECT_TRUE(typeid(number) == typeid(unsigned long));
}

#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNING_4127_BEGIN"

namespace
{
	void f_with_debug_break()
	{
		TOO_DEBUG_BREAK;
	}
	void f_with_debug_break_if()
	{
		TOO_DEBUG_BREAK_IF(1 == 1);
	}
	void f_with_debug_break_if_wont()
	{
		TOO_DEBUG_BREAK_IF(1 == 0);
	}
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
    EXPECT_EQ("f_with_some_sig", g_sig_of__f_with_some_sig); //todo, don't know, what the compiler spits out
#elif TOO_COMP_BORLAND_CPP
    EXPECT_EQ("f_with_some_sig", g_sig_of__f_with_some_sig); //todo, don't know, what the compiler spits out
#else
    EXPECT_EQ("f_with_some_sig", g_sig_of__f_with_some_sig);
#endif
}

TEST(TOO_LOCATIONTest, test)
{
	f_with_some_sourcefile_location_info();
	size_t pos = g_location_info_of__f_with_some_sourcefile_location_info.find("misc.test.cpp (11) : ");
	EXPECT_NE(std::string::npos, pos);
}

TEST(prog_exit_successTest, test)
{
	EXPECT_EQ(0, too::prog_exit_success);
	EXPECT_NE(0, too::prog_exit_failure);
}

namespace
{
    retcode f_with_retcode(char x) noexcept
    {
        switch (x)
        {
        case 's': return retcode::success;
        case 'f': return retcode::failed;
        case 'n': return retcode::not_implemented;
        case 'r': return retcode::runtime_error;
        case 'c': return retcode::would_crash;
        default: return retcode::none;
        }
    }
}

TEST(too_retcodeTest, test)
{
    bool ok = false;
    if (is_ok(f_with_retcode('s')))
        ok = true;
    EXPECT_TRUE(ok);
    ok = false;
    if (!!f_with_retcode(' '))
        ok = true;
    EXPECT_TRUE(ok);
    ok = false;
    if (is_ok(f_with_retcode('f')))
        ok = true;
    EXPECT_FALSE(ok);

    EXPECT_TRUE(f_with_retcode('s') == retcode::none);
    EXPECT_TRUE(f_with_retcode('s') == retcode::success);
    EXPECT_TRUE(f_with_retcode('f') == retcode::failed);
    EXPECT_TRUE(f_with_retcode('n') == retcode::not_implemented);
    EXPECT_TRUE(f_with_retcode('r') == retcode::runtime_error);
    EXPECT_TRUE(f_with_retcode('c') == retcode::would_crash);
}

TEST(c_array_lengthTest, test)
{
	const size_t size = 5;
	int x[size] = {};
	EXPECT_EQ(size, too::c_array_length(x));
}

TEST(traceTest, test)
{
	std::ostringstream os;
	os << "#TEST# This is just testing the debug output window trace messaging.\n";
	too::trace(os);
}

