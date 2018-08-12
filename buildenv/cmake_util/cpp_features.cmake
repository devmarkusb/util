# Collection of C++ feature tests:
# TOO_HAS_CPP_FILESYSTEM

include(CheckCXXSourceCompiles)

check_cxx_source_compiles(
    "
    int a =
    #if __has_include(<filesystem>)
        1;
    #else
        somethingthatdoesntcompile;
    #endif
    int main() { return 0; }
    "
    TOO_HAS_CPP_FILESYSTEM_IMPL)

set(TOO_HAS_CPP_FILESYSTEM ${TOO_HAS_CPP_FILESYSTEM_IMPL} CACHE INTERNAL "" FORCE)
