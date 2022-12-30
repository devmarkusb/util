# include in your project root once; use cmake parameter -DCMAKE_CXX_CPPCHECK:FILEPATH=cppcheck

cmake_minimum_required(VERSION 3.10)

find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
if (CMAKE_CXX_CPPCHECK)
    list(
        APPEND CMAKE_CXX_CPPCHECK
        "--enable=warning"
        "--inconclusive"
        "--force"
        "--inline-suppr"
        "--suppressions-list=${CMAKE_PROJECT_DIR}/CppCheckSuppressions.txt"
    )
    message(CMAKE_CXX_CPPCHECK: ${CMAKE_CXX_CPPCHECK})
endif()
