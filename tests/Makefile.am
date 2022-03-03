# SPDX-License-Identifier: LGPL-2.1

TESTS = avltree_test \
        rbtree_test \
        llrbtree_test \
        btree_test \
        bplustree_test

check_PROGRAMS  = $(TESTS)
noinst_PROGRAMS = $(TESTS)

avltree_test_SOURCES = avltree_test.c
avltree_test_CFLAGS  = -std=c11 -g -O3 -I$(top_builddir)/include
avltree_test_LDFLAGS = -L$(top_builddir)/lib
avltree_test_LDADD   = $(top_builddir)/lib/libindex.a

rbtree_test_SOURCES = rbtree_test.c
rbtree_test_CFLAGS  = -std=c11 -g -O3 -I$(top_builddir)/include
rbtree_test_LDFLAGS = -L$(top_builddir)/lib
rbtree_test_LDADD   = $(top_builddir)/lib/libindex.a

llrbtree_test_SOURCES = llrbtree_test.c
llrbtree_test_CFLAGS  = -std=c11 -g -O3 -I$(top_builddir)/include
llrbtree_test_LDFLAGS = -L$(top_builddir)/lib
llrbtree_test_LDADD   = $(top_builddir)/lib/libindex.a

btree_test_SOURCES = btree_test.c
btree_test_CFLAGS  = -std=c11 -g -O3 -I$(top_builddir)/include
btree_test_LDFLAGS = -L$(top_builddir)/lib
btree_test_LDADD   = $(top_builddir)/lib/libindex.a

bplustree_test_SOURCES = bplustree_test.c
bplustree_test_CFLAGS  = -std=c11 -g -O3 -I$(top_builddir)/include
bplustree_test_LDFLAGS = -L$(top_builddir)/lib
bplustree_test_LDADD   = $(top_builddir)/lib/libindex.a
