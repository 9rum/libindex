# SPDX-License-Identifier: LGPL-2.1

lib_LIBRARIES        = libindex.a
libindex_a_SOURCES   = avltree.c rbtree.c llrbtree.c btree.c bplustree.c
libindex_a_CFLAGS    = -std=c11 -O3 -I$(top_builddir)/include
indexincludedir      = $(includedir)/index
indexinclude_HEADERS = $(top_builddir)/include/index/avltree.h \
                       $(top_builddir)/include/index/rbtree.h \
                       $(top_builddir)/include/index/llrbtree.h \
                       $(top_builddir)/include/index/btree.h \
                       $(top_builddir)/include/index/bplustree.h
