cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(quickjs_zephyr)

get_filename_component(QUICKJS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)

target_sources(app PRIVATE
  src/main.c
  ${QUICKJS_ROOT}/quickjs.c
  ${QUICKJS_ROOT}/dtoa.c
  ${QUICKJS_ROOT}/libregexp.c
  ${QUICKJS_ROOT}/libunicode.c
)
target_include_directories(app PRIVATE ${QUICKJS_ROOT})
target_compile_definitions(app PRIVATE _GNU_SOURCE)
