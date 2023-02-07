# include in your project root once; switch on UL_CPPCHECK cmake option on demand

cmake_minimum_required(VERSION 3.10)

option(UL_CPPCHECK "run cppcheck" OFF)

if (UL_CPPCHECK)
    find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
    if (CMAKE_CXX_CPPCHECK)
        list(
            APPEND CMAKE_CXX_CPPCHECK
            "--enable=warning"
            "--inconclusive"
            "--force"
            "--inline-suppr"
            "--suppressions-list=${PROJECT_SOURCE_DIR}/CppCheckSuppressions.txt"
        )
        message(CMAKE_CXX_CPPCHECK: ${CMAKE_CXX_CPPCHECK})
    endif()
endif()
