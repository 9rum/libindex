/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2021-2022 9rum
 *
 * avltree_test.c - generic AVL tree unit test
 */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#define CTEST_MAIN
#define CTEST_SEGFAULT
#define CTEST_COLOR_OK

#include <ctest.h>
#include <index/avltree.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

      char      src[3];
      char      dest[41];
const uintptr_t testcases[] = {40, 11, 77, 33, 20, 90, 99, 70, 88, 80, 66, 10, 22, 30, 44, 55, 50, 60, 25, 49};

bool less(const void *restrict lhs, const void *restrict rhs) { return *(uintptr_t *)lhs < *(uintptr_t *)rhs; }

void concat(const void *restrict key, void *restrict value) { sprintf(src, "%" PRIuPTR, *(uintptr_t *)key); strcat(dest, src); }

CTEST(avltree_test, avl_find_test) {
  struct avl_root tree = avl_init(less);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    avl_insert(&tree, it, (uintptr_t *)it);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_DATA((const unsigned char *)it, sizeof(uintptr_t), avl_find(tree, it), sizeof(uintptr_t));

  avl_clear(&tree);
  ASSERT_NULL(tree.root);
}

CTEST(avltree_test, avl_insert_test) {
  struct avl_root tree = avl_init(less);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_NOT_NULL(avl_insert(&tree, it, NULL));

  memset(dest, 0, sizeof(dest));
  avl_inorder(tree, concat);
  ASSERT_STR("1011202225303340444950556066707780889099", dest);

  avl_clear(&tree);
  ASSERT_NULL(tree.root);
}

CTEST(avltree_test, avl_insert_or_assign_test) {
  struct avl_root tree = avl_init(less);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_NOT_NULL(avl_insert_or_assign(&tree, it, NULL));

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_NOT_NULL(avl_insert_or_assign(&tree, it, (uintptr_t *)it));

  memset(dest, 0, sizeof(dest));
  avl_inorder(tree, concat);
  ASSERT_STR("1011202225303340444950556066707780889099", dest);

  avl_clear(&tree);
  ASSERT_NULL(tree.root);
}

CTEST(avltree_test, avl_erase_test) {
  struct avl_root tree = avl_init(less);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    avl_insert(&tree, it, (uintptr_t *)it);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_DATA((const unsigned char *)it, sizeof(uintptr_t), avl_erase(&tree, it), sizeof(uintptr_t));

  ASSERT_NULL(tree.root);
}

CTEST(avltree_test, avl_erase_reverse_test) {
  struct avl_root tree = avl_init(less);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    avl_insert(&tree, it, (uintptr_t *)it);

  for (const uintptr_t *it = testcases + sizeof(testcases)/sizeof(uintptr_t) - 1; testcases <= it; --it)
    ASSERT_DATA((const unsigned char *)it, sizeof(uintptr_t), avl_erase(&tree, it), sizeof(uintptr_t));

  ASSERT_NULL(tree.root);
}

int main(int argc, const char **argv) { return ctest_main(argc, argv); }
