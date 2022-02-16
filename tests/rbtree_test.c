/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2021-2022 9rum
 *
 * rbtree_test.c - generic red-black tree unit test
 */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#define CTEST_MAIN
#define CTEST_SEGFAULT
#define CTEST_COLOR_OK

#include <ctest.h>
#include <index/rbtree.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

      char      src[3];
      char      dest[41];
const uintptr_t testcases[] = {40, 11, 77, 33, 20, 90, 99, 70, 88, 80, 66, 10, 22, 30, 44, 55, 50, 60, 25, 49};

bool less(const void *restrict a, const void *restrict b) { return *(uintptr_t *)a < *(uintptr_t *)b; }

void concat(const void *restrict key, void *restrict value) { sprintf(src, "%" PRIuPTR, *(uintptr_t *)key); strcat(dest, src); }

CTEST(rbtree_test, rb_find_test) {
  struct rb_node *tree = NULL;

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    rb_insert(&tree, it, (uintptr_t *)it, less);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_DATA((const unsigned char *)it, sizeof(uintptr_t), rb_find(tree, it, less), sizeof(uintptr_t));

  rb_clear(&tree);
  ASSERT_NULL(tree);
}

CTEST(rbtree_test, rb_insert_test) {
  struct rb_node *tree = NULL;

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_NOT_NULL(rb_insert(&tree, it, NULL, less));

  memset(dest, 0, sizeof(dest));
  rb_inorder(tree, concat);
  ASSERT_STR("1011202225303340444950556066707780889099", dest);

  rb_clear(&tree);
  ASSERT_NULL(tree);
}

CTEST(rbtree_test, rb_insert_or_assign_test) {
  struct rb_node *tree = NULL;

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_NOT_NULL(rb_insert_or_assign(&tree, it, NULL, less));

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_NOT_NULL(rb_insert_or_assign(&tree, it, (uintptr_t *)it, less));

  memset(dest, 0, sizeof(dest));
  rb_inorder(tree, concat);
  ASSERT_STR("1011202225303340444950556066707780889099", dest);

  rb_clear(&tree);
  ASSERT_NULL(tree);
}

CTEST(rbtree_test, rb_erase_test) {
  struct rb_node *tree = NULL;

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    rb_insert(&tree, it, (uintptr_t *)it, less);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_DATA((const unsigned char *)it, sizeof(uintptr_t), rb_erase(&tree, it, less), sizeof(uintptr_t));

  ASSERT_NULL(tree);
}

CTEST(rbtree_test, rb_erase_reverse_test) {
  struct rb_node *tree = NULL;

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    rb_insert(&tree, it, (uintptr_t *)it, less);

  for (const uintptr_t *it = testcases + sizeof(testcases)/sizeof(uintptr_t) - 1; testcases <= it; --it)
    ASSERT_DATA((const unsigned char *)it, sizeof(uintptr_t), rb_erase(&tree, it, less), sizeof(uintptr_t));

  ASSERT_NULL(tree);
}

int main(int argc, const char **argv) { return ctest_main(argc, argv); }
