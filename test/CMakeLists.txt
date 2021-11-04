include_directories(${CMAKE_SOURCE_DIR}/include)

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-O3)
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-O3 -fno-toplevel-reorder)
endif()

file(GLOB_RECURSE tests ${CMAKE_SOURCE_DIR}/test/*.c)

foreach(test ${tests})
  get_filename_component(tname ${test} NAME_WE)
  add_executable(${tname} ${test})
  add_test(NAME ${tname} COMMAND ${tname})
  set_property(TARGET ${tname} PROPERTY C_STANDARD 11)
endforeach()
