# SPDX-License-Identifier: LGPL-2.1

TESTS = avltree_test \
        rbtree_test \
        llrbtree_test \
        btree_test \
        bplustree_test

check_PROGRAMS = $(TESTS)

avltree_test_SOURCES   = avltree_test.c
rbtree_test_SOURCES    = rbtree_test.c
llrbtree_test_SOURCES  = llrbtree_test.c
btree_test_SOURCES     = btree_test.c
bplustree_test_SOURCES = bplustree_test.c

AM_CFLAGS = -std=c11 -O3 -I$(top_builddir)/include
LDADD     = $(top_builddir)/lib/libindex.a
