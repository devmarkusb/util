include(${TOO_CMAKE_INCDIR}/mathgl2_defaults.cmake)

find_path(MATHGL2_INCLUDE_DIRS NAMES mgl2/mgl.h
  PATHS
  $ENV{dev_sdk_paths}/include
)

find_library(MathGL_LIBd NAMES mgld
  PATHS
  $ENV{dev_sdk_paths}/_build_msvc2013/src/Debug
)

find_library(MathGL_LIB NAMES mgl
  PATHS
  $ENV{dev_sdk_paths}/_build_msvc2013/src/Release
)

#find_library(MathGL_QT_LIB NAMES mgl-qt
#  PATHS
#  $ENV{dev_sdk_paths}/lib
#)

#find_library(MathGL_QT_LIBd NAMES mgl-qtd
#  PATHS
#  $ENV{dev_sdk_paths}/lib
#)


set(MATHGL2_LIBRARIES ${MathGL_LIB}) # ${MathGL_QT_LIB}
set(MATHGL2_LIBRARIESd ${MathGL_LIBd}) # ${MathGL_QT_LIBd}

if (MATHGL2_INCLUDE_DIRS AND MATHGL2_LIBRARIES)
  set(MathGL_FOUND TRUE)
  message(STATUS "MathGL found")
  message(STATUS "MathGL Include dirs:" ${MATHGL2_INCLUDE_DIRS})
  message(STATUS "MathGL Library:" ${MATHGL2_LIBRARIES})
  message(STATUS "MathGL Library dbg:" ${MATHGL2_LIBRARIESd})
else ()
  message(STATUS "MathGL was not found")
endif ()
