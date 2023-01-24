# Include in your project root once. Then configure UL_DOXYGEN_GEN_DOXYFILE=ON.
# For the actual generation of documentation you will need to use further tools.

option(UL_DOXYGEN_GEN_DOXYFILE "Run build with Doxyfile creation. A Doxyfile is copied to your project root. \
Before inclusion of doxygen.cmake you should set var dirs_to_doc." OFF)

if (UL_DOXYGEN_GEN_DOXYFILE)
    configure_file(
            ${CMAKE_CURRENT_LIST_DIR}/doxygen/Doxyfile.in
            ${PROJECT_SOURCE_DIR}/Doxyfile
            @ONLY)
endif()
