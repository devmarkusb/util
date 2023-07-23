# add ${ConsoleTable_INCLUDE_DIRS} to include dirs
# #include "ConsoleTable.hpp"
# link with ConsoleTableLib
cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(
        ConsoleTable
        GIT_REPOSITORY https://github.com/766F6964/ConsoleTable
        GIT_TAG        99c4499
        GIT_SHALLOW    ON
        GIT_PROGRESS   ON
)

FetchContent_MakeAvailable(ConsoleTable)

set(ConsoleTable_INCLUDE_DIRS
        ${consoletable_SOURCE_DIR}
        CACHE INTERNAL "ConsoleTable: include directories" FORCE)

add_library(
        ConsoleTableLib STATIC
        ${consoletable_SOURCE_DIR}/ConsoleTable.cpp
        ${consoletable_SOURCE_DIR}/ConsoleTable.h
)

set_target_properties(ConsoleTable PROPERTIES EXCLUDE_FROM_ALL TRUE)

if (MSVC)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set_source_files_properties(${consoletable_SOURCE_DIR}/ConsoleTable.cpp PROPERTIES
            COMPILE_FLAGS "-Wno-return-type -Wno-sign-conversion -Wno-shorten-64-to-32")
    set_target_properties(ConsoleTable PROPERTIES
            COMPILE_FLAGS "-Wno-sign-conversion -Wno-shorten-64-to-32")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set_source_files_properties(${consoletable_SOURCE_DIR}/ConsoleTable.cpp PROPERTIES
            COMPILE_FLAGS "-Wno-conversion")
    set_target_properties(ConsoleTable PROPERTIES
            COMPILE_FLAGS "-Wno-conversion")
endif ()
