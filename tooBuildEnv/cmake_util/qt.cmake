# Include this file and just add e.g.
#   find_package(Qt5Quick)
#   find_package(Qt5LinguistTools)
#   find_package(Qt5Widgets)
#   find_package(Qt5Qml)
#   # ...

# Note: it's important not to give default values here. Otherwise the configuration could
# succeed in a wrong way. Also if you like to change these parameters later on you would have to delete
# and rebuild the CMake cache. Reason: Qt package finding should start from scratch.
set(TOO_QT5_VERSION         "" CACHE STRING "string has to match the subdir under Qt install location, like 5.7 or 5.8")
set(TOO_QT_COMPILER_SUBDIR  "" CACHE STRING "string has to match the subdir under Qt version dir of the install location, like mingw53_32 or msvc2013")
option(TOO_QT_COMPILE_WITH_QML_DEBUGGING_ENABLER "With QQmlDebuggingEnabler for qml debugging and profiling" OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(TOO_QT_VER_COMP_PATH "$ENV{dev_qt_base}/${TOO_QT5_VERSION}/${TOO_QT_COMPILER_SUBDIR}")
list(APPEND CMAKE_PREFIX_PATH ${TOO_QT_VER_COMP_PATH})
