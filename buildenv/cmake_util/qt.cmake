# File to include for cmake Qt settings.
# Include this file and
#   - pass something like -DCMAKE_PREFIX_PATH=$dev_sdk_path/qt_linux/6.8.2/gcc_64
#   - use find_package(Qt6 REQUIRED COMPONENTS Quick LinguistTools Widgets Qml) # ...

option(UL_QT_COMPILE_WITH_QML_DEBUGGING_ENABLER "With QQmlDebuggingEnabler for qml debugging and profiling" OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
