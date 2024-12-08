find_package(TBB REQUIRED)
add_library(libtbb INTERFACE)
target_link_libraries(libtbb INTERFACE TBB::tbb)
