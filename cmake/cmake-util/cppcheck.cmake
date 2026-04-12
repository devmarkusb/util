# include in your project root once; switch on MB_UL_CPPCHECK cmake option on demand

cmake_minimum_required(VERSION 3.10)

option(MB_UL_CPPCHECK "run cppcheck" OFF)
option(
    MB_UL_CPPCHECK_AUTO_INSTALL
    "Attempt to install cppcheck with devenv/install-cppcheck.py when it is missing."
    OFF
)

function(_mb_ul_cppcheck_find_executable out_var)
    find_program(
        _mb_ul_cppcheck_executable
        NAMES cppcheck cppcheck.exe
        HINTS
            "$ENV{ProgramFiles}/Cppcheck"
            "$ENV{ProgramFiles\(x86\)}/Cppcheck"
            "$ENV{LOCALAPPDATA}/Programs/Cppcheck"
        PATH_SUFFIXES "" "bin"
    )
    set(${out_var} "${_mb_ul_cppcheck_executable}" PARENT_SCOPE)
endfunction()

function(_mb_ul_cppcheck_try_install out_var)
    set(_mb_ul_cppcheck_installer_script
        "${PROJECT_SOURCE_DIR}/devenv/install-cppcheck.py"
    )
    if(NOT EXISTS "${_mb_ul_cppcheck_installer_script}")
        message(
            WARNING
            "MB_UL_CPPCHECK_AUTO_INSTALL is ON, but installer script was not found at "
            "'${_mb_ul_cppcheck_installer_script}'."
        )
        set(${out_var} "" PARENT_SCOPE)
        return()
    endif()

    find_package(Python3 COMPONENTS Interpreter QUIET)
    set(_mb_ul_cppcheck_python "${Python3_EXECUTABLE}")
    if(NOT _mb_ul_cppcheck_python)
        find_program(_mb_ul_cppcheck_python NAMES python3 python py)
    endif()
    if(NOT _mb_ul_cppcheck_python)
        message(
            WARNING
            "MB_UL_CPPCHECK_AUTO_INSTALL is ON, but no Python interpreter was found "
            "to run '${_mb_ul_cppcheck_installer_script}'."
        )
        set(${out_var} "" PARENT_SCOPE)
        return()
    endif()

    message(
        STATUS
        "Attempting to install cppcheck with ${_mb_ul_cppcheck_installer_script}."
    )
    execute_process(
        COMMAND
            "${_mb_ul_cppcheck_python}" "${_mb_ul_cppcheck_installer_script}"
            --ensure --print-path
        RESULT_VARIABLE _mb_ul_cppcheck_result
        OUTPUT_VARIABLE _mb_ul_cppcheck_stdout
        ERROR_VARIABLE _mb_ul_cppcheck_stderr
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_STRIP_TRAILING_WHITESPACE
    )
    if(NOT _mb_ul_cppcheck_result EQUAL 0)
        message(
            WARNING
            "cppcheck install command failed (${_mb_ul_cppcheck_result}).\n"
            "stdout:\n${_mb_ul_cppcheck_stdout}\n"
            "stderr:\n${_mb_ul_cppcheck_stderr}"
        )
        set(${out_var} "" PARENT_SCOPE)
        return()
    endif()

    if(_mb_ul_cppcheck_stdout)
        file(TO_CMAKE_PATH "${_mb_ul_cppcheck_stdout}" _mb_ul_cppcheck_detected)
        if(EXISTS "${_mb_ul_cppcheck_detected}")
            set(${out_var} "${_mb_ul_cppcheck_detected}" PARENT_SCOPE)
            return()
        endif()
    endif()

    _mb_ul_cppcheck_find_executable(_mb_ul_cppcheck_detected)
    set(${out_var} "${_mb_ul_cppcheck_detected}" PARENT_SCOPE)
endfunction()

if(MB_UL_CPPCHECK)
    _mb_ul_cppcheck_find_executable(_mb_ul_cppcheck_executable)
    if(NOT _mb_ul_cppcheck_executable AND MB_UL_CPPCHECK_AUTO_INSTALL)
        _mb_ul_cppcheck_try_install(_mb_ul_cppcheck_executable)
    endif()

    if(_mb_ul_cppcheck_executable)
        set(MB_UL_CPPCHECK_EXECUTABLE
            "${_mb_ul_cppcheck_executable}"
            CACHE FILEPATH
            "Detected cppcheck executable"
            FORCE
        )
        set(CMAKE_CXX_CPPCHECK "${_mb_ul_cppcheck_executable}")
        list(
            APPEND CMAKE_CXX_CPPCHECK
            "--enable=warning"
            "--inconclusive"
            "--force"
            "--inline-suppr"
            "--suppressions-list=${PROJECT_SOURCE_DIR}/CppCheckSuppressions.txt"
            "-i${PROJECT_SOURCE_DIR}/build"
        )
        message(CMAKE_CXX_CPPCHECK: ${CMAKE_CXX_CPPCHECK})
    else()
        message(
            WARNING
            "MB_UL_CPPCHECK is ON, but cppcheck was not found. "
            "Set MB_UL_CPPCHECK_AUTO_INSTALL=ON for a best-effort install via "
            "devenv/install-cppcheck.py "
            "or install cppcheck manually."
        )
    endif()
endif()
