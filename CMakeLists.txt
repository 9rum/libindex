# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.7)

project(file-processing LANGUAGES C)

enable_testing()

include(CTest)

add_subdirectory(test)

if(NOT EXISTS ${CMAKE_SOURCE_DIR}/include/ctest.h)
  execute_process(COMMAND curl -o ${CMAKE_SOURCE_DIR}/include/ctest.h https://raw.githubusercontent.com/bvdberg/ctest/master/ctest.h)
endif()

file(GLOB_RECURSE headers ${CMAKE_SOURCE_DIR}/include/*.h)

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
