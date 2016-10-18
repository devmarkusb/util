#include "Toolib/string/locale.h"
#include "gtest/gtest.h"


TEST(consoleTest, set_global_localeTest_localenc)
{
    const std::string s = "\xe4""hnlich"; // latin1 encoding
    std::stringstream ss;
    std::cout << s << "\n";
    ss << s;
    std::string res1(ss.str());
    std::cout << res1 << "\n";

    too::set_global_locale_scoped loc{too::Global_locale::user_preferred};
    std::locale first = loc.get_original_locale();
    EXPECT_EQ(std::locale::classic(), first);
    std::locale userpref = too::set_global_locale(too::Global_locale::default_classic);
    std::locale classictest = too::set_global_locale(too::Global_locale::user_preferred);
    EXPECT_EQ(std::locale::classic(), classictest);

    std::stringstream ss2;
    std::cout << s << "\n";
    ss2 << s;
    std::string res2(ss2.str());
    std::cout << res2 << "\n";

    //even too::set_global_locale_scoped loc{"German_Germany.UTF-8"}; // doesn't work for
    //const std::string s = "\xc3\xa4"hnlich";
}
