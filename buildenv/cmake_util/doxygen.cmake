# Include in your project root once. Then configure UL_DOXYGEN=ON.
# Before including the file, set var dirs_to_doc.
# For the actual generation of documentation build target ${PROJECT_NAME}-doxygen.
# The generated doc can be found under ${CMAKE_BUILD_DIR}/doc/html/index.html

option(UL_DOXYGEN "Creates doxygen target. Before inclusion of doxygen.cmake you should set var dirs_to_doc." OFF)

if (UL_DOXYGEN)
    find_package(Doxygen OPTIONAL_COMPONENTS dot)

    set(DOXYGEN_BUILTIN_STL_SUPPORT YES) # YES?
    set(DOXYGEN_CASE_SENSE_NAMES NO)
    set(DOXYGEN_DOT_GRAPH_MAX_NODES 200)
    set(DOXYGEN_ENABLE_PREPROCESSING YES)
    set(DOXYGEN_EXCLUDE_PATTERNS "*/.test.cpp/*") # ? hopeless, give up
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_LATEX NO)
    set(DOXYGEN_GENERATE_MAN NO)
    set(DOXYGEN_HTML_TIMESTAMP YES)
    set(DOXYGEN_OUTPUT_DIRECTORY "doc")
    set(DOXYGEN_PROJECT_NAME ${PROJECT_NAME})
    set(DOXYGEN_QUIET YES)
    set(DOXYGEN_RECURSIVE YES) # ?
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "README.md") # note, you also have to add it under dirs_to_doc
    set(DOXYGEN_WARNINGS NO)
    set(DOXYGEN_WARN_IF_DOC_ERROR NO)
    set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)

    doxygen_add_docs(
        ${PROJECT_NAME}-doxygen
        ${dirs_to_doc}
        COMMENT "doxygen generate doc"
    )
endif()

