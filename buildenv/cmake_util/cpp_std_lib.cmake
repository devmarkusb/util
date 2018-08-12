# Declares variable TOO_CPP_STD_LIB to identify the C++ standard library type to be used for linking currently.
# Possible values are "libstdc++" for the GNU lib, "libc++" for the Clang lib and "msvc" for Microsofts implementation.

include(CheckCXXSourceCompiles)

check_cxx_source_compiles(
    "
    #include <ciso646>
    int a =
    #ifdef _LIBCPP_VERSION
        1;
    #else
        somethingthatdoesntcompile;
    #endif
    int main() { return 0; }
    "
    TOO_CHECK_USING_CLANG_CPP_STD_LIB)

check_cxx_source_compiles(
    "
    #include <cstddef>
    int a =
    #ifdef __GLIBCXX__
        1;
    #else
        somethingthatdoesntcompile;
    #endif
    int main() { return 0; }
    "
    TOO_CHECK_USING_GNU_CPP_STD_LIB)


if (TOO_CHECK_USING_CLANG_CPP_STD_LIB)
    set(TOO_CPP_STD_LIB_IMPL "libc++")
elseif (TOO_CHECK_USING_GNU_CPP_STD_LIB)
    set(TOO_CPP_STD_LIB_IMPL "libstdc++")
else ()
    set(TOO_CPP_STD_LIB_IMPL "msvc")
endif ()

set(TOO_CPP_STD_LIB ${TOO_CPP_STD_LIB_IMPL} CACHE INTERNAL "indicates C++ standard library currently in use" FORCE)
