dnl SPDX-License-Identifier: LGPL-2.1

dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])

AC_INIT([libindex], [0.1.0])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_HEADERS([config.h])
: ${CFLAGS=""}

dnl Checks for programs.
AC_PROG_CC
LT_INIT

dnl Checks for libraries.

dnl Checks for header files.
AC_CHECK_HEADER([`pwd`/include/ctest.h], , [curl -o `pwd`/include/ctest.h https://raw.githubusercontent.com/bvdberg/ctest/master/ctest.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_C_RESTRICT
AC_TYPE_SIZE_T

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])

AC_CONFIG_FILES([Makefile
                 lib/Makefile
                 tests/Makefile])
AC_OUTPUT
