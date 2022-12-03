# File to include for using gperftools.
# Usage:
# 1) Add to your app's CMakeLists.txt
#       too_include(gperftools.cmake)
#   and add to target with e.g.
#       too_use_gperftools(${TargetAppNameAsLibForTest} ${CMAKE_SOURCE_DIR}/3rdparty/gperftools-2.6.3.tar.gz)
# 2) Add CMake parameter -DTOO_ENABLE_PROFILING_GPERF=ON
# 3) Run the target with environment variable CPUPROFILE=result.prof (if that doesn't work in
#   CLion, consult your psychologist, or find the right place for the var to actually work).
#   Alternatively add the following to your code:
#   #if TOO_ENABLE_PROFILING_GPERF
#   #include "gperftools/profiler.h"
#   #endif
#   ...
#   #if TOO_ENABLE_PROFILING_GPERF
#       ProfilerStart("./app.prof");
#       auto autoStopProfiling = ul::finally([](){ ProfilerStop(); });
#   #endif
# 4) Analyse results by
#   -changing dir to bin dir, where the binary and the prof file reside
#   -running
#       <path-to-pprof>/pprof --gv targetbinary app.prof
#       path-to-pprof could e.g. be builddir/sdks/gpertools/bin

macro(too_use_gperftools target_to_profile path_to_zip)
    option(TOO_ENABLE_PROFILING_GPERF "enable profiling using gperftools" OFF)

    if (TOO_ENABLE_PROFILING_GPERF AND NOT TOO_DEPLOYMENT_BUILD AND NOT WIN32)
        include(ExternalProject)
        set(gperftools_PREFIX ${CMAKE_BINARY_DIR}/3rdparty/gperftools)
        ExternalProject_Add(gperftools
                URL                ${path_to_zip}
                SOURCE_DIR         ${gperftools_PREFIX}/src
                PREFIX             ${gperftools_PREFIX}
                STAMP_DIR          ${gperftools_PREFIX}/stamp
                BINARY_DIR         ${gperftools_PREFIX}/_build
                INSTALL_DIR        ${gperftools_PREFIX}
                CONFIGURE_COMMAND
                CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} CFLAGS=-g <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --enable-frame-pointers
                BUILD_COMMAND      make -j ${TOO_NPROC}
                INSTALL_COMMAND    make install)
        add_library(gperftools_profiler SHARED IMPORTED GLOBAL)
        set(gperftools_profiler_INCLUDE "${gperftools_PREFIX}/include")
        file(MAKE_DIRECTORY ${gperftools_profiler_INCLUDE}) # not there yet at generation time
        set_target_properties(gperftools_profiler PROPERTIES
                IMPORTED_LOCATION "${gperftools_PREFIX}/lib/libprofiler.so"
                INTERFACE_INCLUDE_DIRECTORIES ${gperftools_profiler_INCLUDE})

        add_dependencies(${target_to_profile} gperftools)
        target_include_directories(${target_to_profile} SYSTEM PUBLIC ${gperftools_profiler_INCLUDE})
        target_link_libraries(${target_to_profile} PUBLIC gperftools_profiler)
        target_compile_definitions(${target_to_profile} PUBLIC TOO_ENABLE_PROFILING_GPERF=1)
    endif ()
endmacro()
