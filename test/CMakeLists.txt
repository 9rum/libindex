cmake_minimum_required(VERSION 3.9)

include_directories(${PROJECT_SOURCE_DIR}/../include)

add_compile_options(-v -O3)

file(GLOB TESTS ${PROJECT_SOURCE_DIR}/*.c)

foreach(TEST ${TESTS})
  set(EXECUTABLE)
  get_filename_component(EXECUTABLE ${TEST} NAME_WE)
  add_executable(${EXECUTABLE} ${TEST})
  set_property(TARGET ${EXECUTABLE} PROPERTY C_STANDARD 11)
endforeach()
