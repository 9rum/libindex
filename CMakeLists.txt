# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.7)

project(libindex LANGUAGES C)

include(GNUInstallDirs)
include(CTest)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_compile_options(-O3)

file(GLOB_RECURSE headers ${CMAKE_SOURCE_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/index/*.h)
file(GLOB_RECURSE sources ${CMAKE_SOURCE_DIR}/${CMAKE_INSTALL_LIBDIR}/*.c)

add_library(index STATIC ${sources})
target_include_directories(index PUBLIC ${CMAKE_SOURCE_DIR}/${CMAKE_INSTALL_INCLUDEDIR})

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/index)
install(TARGETS index LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

if(NOT EXISTS ${CMAKE_SOURCE_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/ctest.h)
  execute_process(COMMAND curl -o ${CMAKE_SOURCE_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/ctest.h https://raw.githubusercontent.com/bvdberg/ctest/master/ctest.h)
endif()

enable_testing()

add_subdirectory(test)
