cmake_minimum_required(VERSION 3.5)

project(file-processing LANGUAGES C)

enable_testing()

include(CTest)

add_subdirectory(test)

file(GLOB_RECURSE headers ${CMAKE_SOURCE_DIR}/include/*.h)

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
