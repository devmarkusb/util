# include in your project root once; switch on MB_UL_CPPCHECK cmake option on demand

cmake_minimum_required(VERSION 3.10)

option(MB_UL_CPPCHECK "run cppcheck" OFF)
option(
    MB_UL_CPPCHECK_AUTO_INSTALL
    "Attempt to install cppcheck with a supported host package manager when it is missing."
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

function(_mb_ul_cppcheck_run_command)
    execute_process(
        COMMAND ${ARGV}
        RESULT_VARIABLE _mb_ul_cppcheck_result
        OUTPUT_VARIABLE _mb_ul_cppcheck_stdout
        ERROR_VARIABLE _mb_ul_cppcheck_stderr
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_STRIP_TRAILING_WHITESPACE
    )
    if(NOT _mb_ul_cppcheck_result EQUAL 0)
        message(
            WARNING
            "cppcheck install command failed (${_mb_ul_cppcheck_result}): ${ARGV}\n"
            "stdout:\n${_mb_ul_cppcheck_stdout}\n"
            "stderr:\n${_mb_ul_cppcheck_stderr}"
        )
        set(MB_UL_CPPCHECK_INSTALL_SUCCEEDED OFF PARENT_SCOPE)
        return()
    endif()
    set(MB_UL_CPPCHECK_INSTALL_SUCCEEDED ON PARENT_SCOPE)
endfunction()

function(_mb_ul_cppcheck_try_install out_var)
    set(_mb_ul_cppcheck_installer "")

    if(CMAKE_HOST_WIN32)
        find_program(_mb_ul_cppcheck_winget winget)
        find_program(_mb_ul_cppcheck_choco choco)
        if(_mb_ul_cppcheck_winget)
            set(_mb_ul_cppcheck_installer
                "${_mb_ul_cppcheck_winget};install;-e;--id;Cppcheck.Cppcheck;--accept-package-agreements;--accept-source-agreements"
            )
        elseif(_mb_ul_cppcheck_choco)
            set(_mb_ul_cppcheck_installer
                "${_mb_ul_cppcheck_choco};install;cppcheck;-y"
            )
        endif()
    elseif(CMAKE_HOST_APPLE)
        find_program(_mb_ul_cppcheck_brew brew)
        if(_mb_ul_cppcheck_brew)
            set(_mb_ul_cppcheck_installer
                "${_mb_ul_cppcheck_brew};install;cppcheck"
            )
        endif()
    elseif(CMAKE_HOST_UNIX)
        execute_process(
            COMMAND id -u
            RESULT_VARIABLE _mb_ul_cppcheck_id_result
            OUTPUT_VARIABLE _mb_ul_cppcheck_uid
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
        set(_mb_ul_cppcheck_priv_prefix "")
        if(
            _mb_ul_cppcheck_id_result EQUAL 0
            AND NOT _mb_ul_cppcheck_uid STREQUAL "0"
        )
            find_program(_mb_ul_cppcheck_sudo sudo)
            if(_mb_ul_cppcheck_sudo)
                set(_mb_ul_cppcheck_priv_prefix "${_mb_ul_cppcheck_sudo};-n")
            endif()
        endif()

        find_program(_mb_ul_cppcheck_apt apt-get)
        find_program(_mb_ul_cppcheck_dnf dnf)
        find_program(_mb_ul_cppcheck_pacman pacman)
        find_program(_mb_ul_cppcheck_zypper zypper)
        find_program(_mb_ul_cppcheck_brew brew)

        if(_mb_ul_cppcheck_apt)
            message(STATUS "Attempting to install cppcheck with apt-get.")
            _mb_ul_cppcheck_run_command(${_mb_ul_cppcheck_priv_prefix} "${_mb_ul_cppcheck_apt}" update)
            if(NOT MB_UL_CPPCHECK_INSTALL_SUCCEEDED)
                set(${out_var} "" PARENT_SCOPE)
                return()
            endif()
            _mb_ul_cppcheck_run_command(
                ${_mb_ul_cppcheck_priv_prefix}
                "${_mb_ul_cppcheck_apt}"
                install
                -y
                cppcheck
            )
        elseif(_mb_ul_cppcheck_dnf)
            message(STATUS "Attempting to install cppcheck with dnf.")
            _mb_ul_cppcheck_run_command(
                ${_mb_ul_cppcheck_priv_prefix}
                "${_mb_ul_cppcheck_dnf}"
                install
                -y
                cppcheck
            )
        elseif(_mb_ul_cppcheck_pacman)
            message(STATUS "Attempting to install cppcheck with pacman.")
            _mb_ul_cppcheck_run_command(
                ${_mb_ul_cppcheck_priv_prefix}
                "${_mb_ul_cppcheck_pacman}"
                -Sy
                --noconfirm
                cppcheck
            )
        elseif(_mb_ul_cppcheck_zypper)
            message(STATUS "Attempting to install cppcheck with zypper.")
            _mb_ul_cppcheck_run_command(
                ${_mb_ul_cppcheck_priv_prefix}
                "${_mb_ul_cppcheck_zypper}"
                --non-interactive
                install
                cppcheck
            )
        elseif(_mb_ul_cppcheck_brew)
            message(STATUS "Attempting to install cppcheck with Homebrew.")
            _mb_ul_cppcheck_run_command("${_mb_ul_cppcheck_brew}" install cppcheck)
        endif()

        if(
            DEFINED MB_UL_CPPCHECK_INSTALL_SUCCEEDED
            AND MB_UL_CPPCHECK_INSTALL_SUCCEEDED
        )
            _mb_ul_cppcheck_find_executable(_mb_ul_cppcheck_detected)
            set(${out_var} "${_mb_ul_cppcheck_detected}" PARENT_SCOPE)
            return()
        endif()

        set(${out_var} "" PARENT_SCOPE)
        return()
    endif()

    if(_mb_ul_cppcheck_installer)
        message(
            STATUS
            "Attempting to install cppcheck with ${_mb_ul_cppcheck_installer}."
        )
        _mb_ul_cppcheck_run_command(${_mb_ul_cppcheck_installer})
        if(
            DEFINED MB_UL_CPPCHECK_INSTALL_SUCCEEDED
            AND MB_UL_CPPCHECK_INSTALL_SUCCEEDED
        )
            _mb_ul_cppcheck_find_executable(_mb_ul_cppcheck_detected)
            set(${out_var} "${_mb_ul_cppcheck_detected}" PARENT_SCOPE)
            return()
        endif()
    endif()

    set(${out_var} "" PARENT_SCOPE)
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
            "Set MB_UL_CPPCHECK_AUTO_INSTALL=ON for a best-effort package-manager install "
            "or install cppcheck manually."
        )
    endif()
endif()
