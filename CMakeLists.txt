# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.7)

project(libindex LANGUAGES C)

include(GNUInstallDirs)
include(CTest)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(libname "index")

add_compile_options(-O3)

file(GLOB headers ${CMAKE_SOURCE_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/index/*.h)
file(GLOB sources ${CMAKE_SOURCE_DIR}/${CMAKE_INSTALL_LIBDIR}/*.c)

add_library(${libname} STATIC ${sources})
target_include_directories(${libname} PUBLIC ${CMAKE_SOURCE_DIR}/${CMAKE_INSTALL_INCLUDEDIR})

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/index)
install(TARGETS ${libname} LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

if(NOT EXISTS ${CMAKE_SOURCE_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/ctest.h)
  execute_process(COMMAND curl -o ${CMAKE_SOURCE_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/ctest.h https://raw.githubusercontent.com/bvdberg/ctest/master/ctest.h)
endif()

enable_testing()

add_subdirectory(tests)
