# Include this file and just add e.g.
#   find_package(Qt5Quick)
#   find_package(Qt5LinguistTools)
#   find_package(Qt5Widgets)
#   find_package(Qt5Qml)
#   # ...

# Note: it's important not to give default values here. Otherwise the configuration could
# succeed in a wrong way. Also if you like to change these parameters later on you would have to delete
# and rebuild the CMake cache. Reason: Qt package finding should start from scratch.
set(Qt5_version         "" CACHE STRING "string has to match the subdir under Qt install location, like 5.7 or 5.8")
set(Qt_compiler_subdir  "" CACHE STRING "string has to match the subdir under Qt version dir of the install location, like mingw53_32 or msvc2013")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(Qt_ver_comp_path "$ENV{dev_qt_base}\\${Qt5_version}\\${Qt_compiler_subdir}")
list(APPEND CMAKE_PREFIX_PATH ${Qt_ver_comp_path})
