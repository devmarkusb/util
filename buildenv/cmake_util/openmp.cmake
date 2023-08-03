# File to include for OpenMP settings.
# You should just need to link to OpenMP::OpenMP_CXX.

find_package(OpenMP)

if (NOT OpenMP_CXX_FOUND)
    message(FATAL_ERROR "Could not find OpenMP. Hint: Interestingly it seems only a single version of "
        "libomp-<ver>-dev can be installed at a time. So just install `$ sudo apt install libomp-15-dev` e.g., "
        "and this might also automatically remove version 16 e.g.")
endif()
