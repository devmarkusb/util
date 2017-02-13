# Include this file and just add e.g.
#   find_package(Boost 1.63.0 COMPONENTS system filesystem) # or similar
# Then include ${Boost_INCLUDE_DIRS} and link ${Boost_LIBRARIES} if necessary.
set(BOOST_ROOT $ENV{dev_sdk_path})
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_STATIC_RUNTIME    OFF)
add_definitions(-DBOOST_ALL_NO_LIB)
