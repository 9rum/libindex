# SPDX-License-Identifier: Apache-2.0

include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

add_compile_options(-O3)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-fno-toplevel-reorder)
endif()

add_test(NAME build COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR})
set_tests_properties(build PROPERTIES FIXTURES_SETUP fixture)

file(GLOB_RECURSE tests ${CMAKE_SOURCE_DIR}/test/*.c)

foreach(test ${tests})
  get_filename_component(tname ${test} NAME_WE)
  add_executable(${tname} ${test})
  add_test(NAME ${tname} COMMAND ${tname})
  set_tests_properties(${tname} PROPERTIES FIXTURES_REQUIRED fixture)
endforeach()
