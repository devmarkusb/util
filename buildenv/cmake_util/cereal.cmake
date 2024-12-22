if (TARGET cereal)
    return()
endif()

find_package(cereal REQUIRED)
add_library(cereal INTERFACE)
