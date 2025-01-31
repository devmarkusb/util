if (TARGET tiny-process-library)
    return()
endif()

find_package(tiny-process-library REQUIRED)
add_library(libtiny-process-library INTERFACE)
find_library(TINY_PROCESS_LIBRARY_LIBRARIES NAMES libtiny-process-library.a)
message(STATUS "use static tiny-process-library: ${TINY_PROCESS_LIBRARY_LIBRARIES}")
target_link_libraries(tiny-process-library INTERFACE ${TINY_PROCESS_LIBRARY_LIBRARIES})
