# Collection of prominent CMake vars, printing all of them.

#if you are building in - source, this is the same as CMAKE_SOURCE_DIR, otherwise
#this is the top level directory of your build tree
cmake_print_variables(CMAKE_BINARY_DIR)

#if you are building in - source, this is the same as CMAKE_CURRENT_SOURCE_DIR, otherwise this
#is the directory where the compiled or generated files from the current CMakeLists.txt will go to
cmake_print_variables(CMAKE_CURRENT_BINARY_DIR)

#this is the directory, from which cmake was started, i.e.the top level source directory
cmake_print_variables(CMAKE_SOURCE_DIR)

#this is the directory where the currently processed CMakeLists.txt is located in
cmake_print_variables(CMAKE_CURRENT_SOURCE_DIR)

#contains the full path to the top level directory of your build tree
cmake_print_variables(PROJECT_BINARY_DIR)

#contains the full path to the root of your project source directory,
#i.e.to the nearest directory where CMakeLists.txt contains the PROJECT() command
cmake_print_variables(PROJECT_SOURCE_DIR)

#set this variable to specify a common place where CMake should put all executable files
#(instead of CMAKE_CURRENT_BINARY_DIR)
cmake_print_variables(EXECUTABLE_OUTPUT_PATH)

#set this variable to specify a common place where CMake should put all libraries
#(instead of CMAKE_CURRENT_BINARY_DIR)
cmake_print_variables(LIBRARY_OUTPUT_PATH)

#tell CMake to search first in directories listed in CMAKE_MODULE_PATH
#when you use FIND_PACKAGE() or INCLUDE()
cmake_print_variables(CMAKE_MODULE_PATH)

#this is the complete path of the cmake which runs currently(e.g./ usr / local / bin / cmake)
cmake_print_variables(CMAKE_COMMAND)

#this is the CMake installation directory
cmake_print_variables(CMAKE_ROOT)

#this is the filename including the complete path of the file where this variable is used.
cmake_print_variables(CMAKE_CURRENT_LIST_FILE)

#this is linenumber where the variable is used
cmake_print_variables(CMAKE_CURRENT_LIST_LINE)

#this is used when searching for include files e.g.using the FIND_PATH() command.
cmake_print_variables(CMAKE_INCLUDE_PATH)

#this is used when searching for libraries e.g.using the FIND_LIBRARY() command.
cmake_print_variables(CMAKE_LIBRARY_PATH)

#the complete system name, e.g."Linux-2.4.22", "FreeBSD-5.4-RELEASE" or "Windows 5.1"
cmake_print_variables(CMAKE_SYSTEM)

#the short system name, e.g."Linux", "FreeBSD" or "Windows"
cmake_print_variables(CMAKE_SYSTEM_NAME)

#only the version part of CMAKE_SYSTEM
cmake_print_variables(CMAKE_SYSTEM_VERSION)

#the processor name(e.g."Intel(R) Pentium(R) M processor 2.00GHz")
cmake_print_variables(CMAKE_SYSTEM_PROCESSOR)

#is TRUE on all UNIX - like OS's, including Apple OS X and CygWin
cmake_print_variables(UNIX)

#is TRUE on Windows, including CygWin
cmake_print_variables(WIN32)

#is TRUE on Apple OS X
cmake_print_variables(APPLE)

#is TRUE when using the MinGW compiler in Windows
cmake_print_variables(MINGW)

#is TRUE on Windows when using the CygWin version of cmake
cmake_print_variables(CYGWIN)

#is TRUE on Windows when using a Borland compiler
cmake_print_variables(BORLAND)

#Microsoft compiler
cmake_print_variables(MSVC)
cmake_print_variables(MSVC_IDE)
cmake_print_variables(MSVC60)
cmake_print_variables(MSVC70)
cmake_print_variables(MSVC71)
cmake_print_variables(MSVC80)
cmake_print_variables(CMAKE_COMPILER_2005)


#set this to true if you don't want to rebuild the object files if the rules have changed, 
#but not the actual source files or headers(e.g.if you changed the some compiler switches)
cmake_print_variables(CMAKE_SKIP_RULE_DEPENDENCY)

#since CMake 2.1 the install rule depends on all, i.e.everything will be built before installing.
#If you don't like this, set this one to true.
cmake_print_variables(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY)

#If set, runtime paths are not added when using shared libraries.Default it is set to OFF
cmake_print_variables(CMAKE_SKIP_RPATH)

#set this to true if you are using makefiles and want to see the full compile and link
#commands instead of only the shortened ones
cmake_print_variables(CMAKE_VERBOSE_MAKEFILE)

#this will cause CMake to not put in the rules that re - run CMake.This might be useful if
#you want to use the generated build files on another machine.
cmake_print_variables(CMAKE_SUPPRESS_REGENERATION)


#A simple way to get switches to the compiler is to use ADD_DEFINITIONS().
#But there are also two variables exactly for this purpose:

#the compiler flags for compiling C sources
cmake_print_variables(CMAKE_C_FLAGS)

#the compiler flags for compiling C++ sources
cmake_print_variables(CMAKE_CXX_FLAGS)


#Choose the type of build.Example : SET(CMAKE_BUILD_TYPE Debug)
cmake_print_variables(CMAKE_BUILD_TYPE)

#if this is set to ON, then all libraries are built as shared libraries by default.
cmake_print_variables(BUILD_SHARED_LIBS)

#the compiler used for C files
cmake_print_variables(CMAKE_C_COMPILER)

#the compiler used for C++ files
cmake_print_variables(CMAKE_CXX_COMPILER)

#if the compiler is a variant of gcc, this should be set to 1
cmake_print_variables(CMAKE_COMPILER_IS_GNUCC)

#if the compiler is a variant of g++, this should be set to 1
cmake_print_variables(CMAKE_COMPILER_IS_GNUCXX )

#the tools for creating libraries
cmake_print_variables(CMAKE_AR)
cmake_print_variables(CMAKE_RANLIB)
