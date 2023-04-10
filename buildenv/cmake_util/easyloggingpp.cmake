FetchContent_Declare(
     easyloggingpp
     GIT_REPOSITORY https://github.com/devmarkusb/easyloggingpp
     GIT_TAG        v9.96.7
)

FetchContent_MakeAvailable(easyloggingpp)

set(easyloggingpp_INCLUDE_DIRS
        ${easyloggingpp_SOURCE_DIR}/src
        CACHE INTERNAL "easyloggingpp: include directories" FORCE)

set(easyloggingpp_SOURCE_FILES
        ${easyloggingpp_SOURCE_DIR}/src/easylogging++.cc
        CACHE INTERNAL "easyloggingpp: source files" FORCE)

if (MSVC)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    string(CONCAT compileOptions
            "-Wno-sign-conversion;-Wno-implicit-int-conversion;"
            "-Wno-unused-parameter")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 10)
        string(CONCAT compileOptions
                "${compileOptions};-Wno-implicit-int-float-conversion;-Wno-range-loop-construct")
    endif ()
    set_source_files_properties(${easyloggingpp_SOURCE_FILES} DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTIES
            COMPILE_OPTIONS "${compileOptions}")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
endif ()
