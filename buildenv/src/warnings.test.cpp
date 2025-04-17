#include "ul/warnings.h"
#include "gtest/gtest.h"

TEST(WarningsTest, CompilerSpecific) {
    UL_PRAGMA_WARNINGS_PUSH
#if UL_COMP_MS_VISUAL_STUDIO_CPP
    UL_WARNING_DISABLE_MSVC(4189);  // 'identifier': local variable is initialized but not referenced
#elif UL_COMP_CLANG
    UL_WARNING_DISABLE_CLANG(unused-variable);
#elif UL_COMP_GNU_CPP
    UL_WARNING_DISABLE_GCC(unused-variable);
#endif
    {
        int unused = 42;  // Should not trigger warnings
    }
    UL_PRAGMA_WARNINGS_POP
}

TEST(WarningsTest, DisableAllMSVC) {
#if UL_COMP_MS_VISUAL_STUDIO_CPP
    UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC
    {
        int unused = 42;  // Should not trigger any warnings
    }
    UL_PRAGMA_WARNINGS_POP
#endif
}
