# File to include for OpenMP settings.
# You should just need to link to OpenMP::OpenMP_CXX.

find_package(OpenMP)

if (NOT OpenMP_CXX_FOUND)
    message(FATAL_ERROR "Could not find OpenMP")
endif()
