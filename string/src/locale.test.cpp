#include "ul/string/locale.h"
#include "gtest/gtest.h"

namespace ul = mb::ul;

TEST(consoleTest, set_global_localeTest_localenc)
{
    const std::string s = "\xe4"
                          "hnlich"; // latin1 encoding
    std::stringstream ss;
    std::cout << s << "\n";
    ss << s;
    std::string res1(ss.str());
    std::cout << res1 << "\n";

// #locale_facet-exception
// For mingw 50300 the following exception is thrown:
// C++ exception with description "locale::facet::_S_create_c_locale name not valid" thrown in the test body.
// Strange stuff: Happens only during unit test as part of compilation. Not failing when
// starting the test exe separately - so no debugging possible :/
// Also the build server doesn't fail!
#if !(UL_COMP_MINGW && UL_COMP_MINGW_VER <= 50300)
    ul::set_global_locale_scoped loc{ul::Global_locale::user_preferred};
    std::locale first = loc.get_original_locale();
    EXPECT_EQ(std::locale::classic(), first);
    /*std::locale userpref = */ ul::set_global_locale(ul::Global_locale::default_classic);
    std::locale classictest = ul::set_global_locale(ul::Global_locale::user_preferred);
    EXPECT_EQ(std::locale::classic(), classictest);
#endif

    std::stringstream ss2;
    std::cout << s << "\n";
    ss2 << s;
    std::string res2(ss2.str());
    std::cout << res2 << "\n";

    // even ul::set_global_locale_scoped loc{"German_Germany.UTF-8"}; // doesn't work for
    // const std::string s = "\xc3\xa4"hnlich";
}
