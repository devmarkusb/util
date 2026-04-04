set(ALL_EXAMPLES usage)

message("Examples to be built: ${ALL_EXAMPLES}")

foreach(example ${ALL_EXAMPLES})
    add_executable(mb.util.examples.${example})
    target_sources(mb.util.examples.${example} PRIVATE ${example}.cpp)
    target_link_libraries(mb.util.examples.${example} PRIVATE mb::util)
    mb_devenv_set_target_defaults(mb.util.examples.${example})
endforeach()
