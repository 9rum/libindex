# SPDX-License-Identifier: Apache-2.0

AM_CFLAGS            = -std=c11 -O3 -I$(top_builddir)/include
lib_LIBRARIES        = libindex.a
libindex_a_SOURCES   = avltree.c rbtree.c llrbtree.c btree.c bplustree.c
indexincludedir      = $(includedir)/index
indexinclude_HEADERS = $(top_builddir)/include/index/stack.h \
                       $(top_builddir)/include/index/avltree.h \
                       $(top_builddir)/include/index/rbtree.h \
                       $(top_builddir)/include/index/llrbtree.h \
                       $(top_builddir)/include/index/btree.h \
                       $(top_builddir)/include/index/bplustree.h
