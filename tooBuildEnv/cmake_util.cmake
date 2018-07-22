# Export various cmake utility files of common use.

set(TOO_CMAKE_INC_BOOST ${PROJECT_SOURCE_DIR}/cmake_util/boost.cmake
    CACHE INTERNAL "TOO_CMAKE_INC_BOOST: file to include for cmake boost settings" FORCE)

set(TOO_CMAKE_INC_BUILDNR ${PROJECT_SOURCE_DIR}/cmake_util/buildnr.cmake
    CACHE INTERNAL "TOO_CMAKE_INC_BUILDNR: file to include for cmake build number settings" FORCE)
set(TOO_CMAKE_INC_DEFAULTS ${PROJECT_SOURCE_DIR}/cmake_util/defaults.cmake
    CACHE INTERNAL "TOO_CMAKE_INC_DEFAULTS: file to include for cmake default settings" FORCE)

set(TOO_CMAKE_INC_OPENMP ${PROJECT_SOURCE_DIR}/cmake_util/openmp.cmake
        CACHE INTERNAL "TOO_CMAKE_INC_OPENMP: file to include for OpenMP settings" FORCE)

set(TOO_CMAKE_INC_GOOGLETEST_AS_SUBDIR ${PROJECT_SOURCE_DIR}/cmake_util/googletest_as_subdir.cmake
    CACHE INTERNAL "TOO_CMAKE_INC_GOOGLETEST_AS_SUBDIR: file to include to add googletest as subdir plus configurations" FORCE)

set(TOO_CMAKE_INC_MATHGL2 ${PROJECT_SOURCE_DIR}/cmake_util/mathgl2.cmake
    CACHE INTERNAL "TOO_CMAKE_INC_MATHGL2: file to include for cmake mathgl settings" FORCE)
set(TOO_CMAKE_INC_MATHGL2_AS_SUBDIR ${PROJECT_SOURCE_DIR}/cmake_util/mathgl2_as_subdir.cmake
    CACHE INTERNAL "TOO_CMAKE_INC_MATHGL2_AS_SUBDIR: file to include to add mathgl as subdir plus configurations" FORCE)

set(TOO_CMAKE_INC_QT ${PROJECT_SOURCE_DIR}/cmake_util/qt.cmake
    CACHE INTERNAL "TOO_CMAKE_INC_QT: file to include for cmake Qt settings" FORCE)
set(TOO_CMAKE_INC_QT_DEPLOY_TOO_TEMP_TARGET ${PROJECT_SOURCE_DIR}/cmake_util/qt_deploy_TOO_TEMP_TARGET.cmake
    CACHE INTERNAL "TOO_CMAKE_INC_QT_DEPLOY_TOO_TEMP_TARGET: file to include for cmake Qt postbuild deployment dependency additions" FORCE)
set(TOO_CMAKE_INC_QT_LANG ${PROJECT_SOURCE_DIR}/cmake_util/qt_lang.cmake
    CACHE INTERNAL "TOO_CMAKE_INC_QT_LANG: file to include for cmake Qt language settings" FORCE)

set(TOO_CMAKE_INC_UIWRAP_AS_SUBDIR ${PROJECT_SOURCE_DIR}/cmake_util/uiwrap.cmake
    CACHE INTERNAL "TOO_CMAKE_INC_UIWRAP_AS_SUBDIR: file to include to add uiwrap as subdir plus configurations" FORCE)

set(TOO_CMAKE_INC_UWP_TOOLS ${PROJECT_SOURCE_DIR}/cmake_util/uwp_tools.cmake
    CACHE INTERNAL "TOO_CMAKE_INC_UWP_TOOLS: file to include to add UWP tools" FORCE)
set(TOO_CMAKE_INC_UWP_TOOLS_DEF_ICO ${PROJECT_SOURCE_DIR}/cmake_util/assets
    CACHE INTERNAL "can be used as default parameter for too_obtain_UWP_resources of TOO_CMAKE_INC_UWP_TOOLS" FORCE)

set(TOO_CMAKE_INC_VLD ${PROJECT_SOURCE_DIR}/cmake_util/vld.cmake
    CACHE INTERNAL "TOO_CMAKE_INC_VLD: file to include for cmake VLD settings" FORCE)

set(TOO_CMAKE_INCDIR ${PROJECT_SOURCE_DIR}/cmake_util
    CACHE INTERNAL "TOO_CMAKE_INCDIR: path to various cmake utility include files" FORCE)
