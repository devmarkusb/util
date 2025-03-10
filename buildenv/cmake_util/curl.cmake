if (TARGET libcurl)
    return()
endif()

find_package(CURL REQUIRED)
add_library(libcurl INTERFACE)
find_library(CURL_LIBRARIES NAMES libcurl.a curl)
message(STATUS "Use static CURL: ${CURL_LIBRARIES}")
target_link_libraries(libcurl INTERFACE ${CURL_LIBRARIES})
