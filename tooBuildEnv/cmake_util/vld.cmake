option(WITH_VISUAL_LEAK_DETECTOR "With Visual Leak Detection" OFF)

if(WITH_VISUAL_LEAK_DETECTOR AND MSVC)
  message("Enabling Visual Leak Detector")
  find_library(VLD vld
    HINTS "c:/Program Files (x86)/Visual Leak Detector/lib/Win32" "c:/Program Files/Visual Leak Detector/lib/Win32")
  find_file(VLD_H vld.h
    HINTS "c:/Program Files (x86)/Visual Leak Detector/include" "c:/Program Files/Visual Leak Detector/include")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FIwinsock2.h")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \"/FI${VLD_H}\"")
  get_filename_component(VLD_LIBRARY_DIR "${VLD}" PATH)
  link_directories("${VLD_LIBRARY_DIR}")
endif()
