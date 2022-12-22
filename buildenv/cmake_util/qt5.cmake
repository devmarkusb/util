# File to include for cmake Qt settings.
# Include this file and just add e.g.
#   find_package(Qt5 COMPONENTS Quick LinguistTools Widgets Qml) # ...

# Note: it's important not to give default values here. Otherwise the configuration could
# succeed in a wrong way. Also if you like to change these parameters later on you would have to delete
# and rebuild the CMake cache. Reason: Qt package finding should start from scratch.
set(UL_QT5_VERSION         "" CACHE STRING "string has to match the subdir under Qt install location, like 5.7 or 5.8")
set(UL_QT_COMPILER_SUBDIR  "" CACHE STRING "string has to match the subdir under Qt version dir of the install location, like mingw53_32 or msvc2013")
option(UL_QT_COMPILE_WITH_QML_DEBUGGING_ENABLER "With QQmlDebuggingEnabler for qml debugging and profiling" OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(UL_QT_VER_COMP_PATH "$ENV{dev_qt_base}/${UL_QT5_VERSION}/${UL_QT_COMPILER_SUBDIR}")
list(APPEND CMAKE_PREFIX_PATH ${UL_QT_VER_COMP_PATH})
message(UL_QT_VER_COMP_PATH: ${UL_QT_VER_COMP_PATH})
message(CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH})
