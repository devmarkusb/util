# Not yet working, don't know why.
# Usage:
# set_property(TARGET target PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

find_program(iwyu_path NAMES include-what-you-use iwyu REQUIRED)
