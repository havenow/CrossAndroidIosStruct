cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

set(lib_src_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(lib_build_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${ANDROID_ABI}/obj)
file(MAKE_DIRECTORY ${lib_build_DIR})

message("########## lib_src_DIR=${lib_src_DIR}")
message("########## lib_build_DIR=${lib_build_DIR}")

add_subdirectory(${lib_src_DIR}/gmath ${lib_build_DIR}/gmath)
add_subdirectory(${lib_src_DIR}/gperf ${lib_build_DIR}/gperf)

