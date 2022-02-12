/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2021-2022 9rum
 *
 * btree_test.c - generic B-tree unit test
 */
#define CTEST_MAIN
#define CTEST_SEGFAULT
#define CTEST_COLOR_OK

#include <ctest.h>
#include <index/btree.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

      char      src[4];
      char      dest[131];
const uintptr_t testcases[] = {
  40, 11, 77, 33, 20, 90, 99, 70, 88, 80,
  66, 10, 22, 30, 44, 55, 50, 60, 100, 28,
  18, 9, 5, 17, 6, 3, 1, 4, 2, 7,
  8, 73, 12, 13, 14, 16, 15, 25, 24, 28,
  45, 49, 42, 43, 41, 47, 48, 46, 63, 68,
  61, 62, 64, 69, 67, 65, 54, 59, 58, 51,
  53, 57, 52, 56, 83, 81, 82, 84, 75, 89,
  66, 10, 22, 30, 44, 55, 50, 60, 100, 28,
  18, 9, 5, 17, 6, 3, 1, 4, 2, 7,
  8, 73, 12, 13, 14, 16, 15, 25, 24, 28,
  40, 11, 77, 33, 20, 90, 99, 70, 88, 80,
  45, 49, 42, 43, 41, 47, 48, 46, 63, 68,
  53, 57, 52, 56, 83, 81, 82, 84, 75, 89,
  61, 62, 64, 69, 67, 65, 54, 59, 58, 51,
};

bool less(const void *restrict a, const void *restrict b) { return *(uintptr_t *)a < *(uintptr_t *)b; }

void concat(const void *restrict key, void *restrict value) { sprintf(src, "%" PRIuPTR, *(uintptr_t *)key); strcat(dest, src); }

CTEST(btree_test, btree_insert_odd_test) {
        struct btree_node *tree = NULL;
  const uintptr_t         *it   = testcases;

  for (; it < testcases + 39; ++it)
    btree_insert(&tree, 3, it, NULL, less);

  btree_insert(&tree, 3, it++, NULL, less);

  for (; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    btree_insert(&tree, 3, it, NULL, less);

  memset(dest, 0, sizeof(dest));
  btree_inorder(tree, concat);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505152535455565758596061626364656667686970737577808182838488899099100", dest);

  btree_clear(&tree);
  ASSERT_NULL(tree);
}

CTEST(btree_test, btree_erase_odd_test) {
        struct btree_node *tree = NULL;
  const uintptr_t         *it   = testcases;

  for (; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    btree_insert(&tree, 3, it, (uintptr_t *)it, less);

  for (; it < testcases + 99; ++it)
    btree_erase(&tree, 3, it, less);

  btree_erase(&tree, 3, it++, less);

  for (; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    btree_erase(&tree, 3, it, less);

  ASSERT_NULL(tree);
}

CTEST(btree_test, btree_insert_even_test) {
        struct btree_node *tree = NULL;
  const uintptr_t         *it   = testcases;

  for (; it < testcases + 39; ++it)
    btree_insert(&tree, 4, it, NULL, less);

  btree_insert(&tree, 4, it++, NULL, less);

  for (; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    btree_insert(&tree, 4, it, NULL, less);

  memset(dest, 0, sizeof(dest));
  btree_inorder(tree, concat);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505152535455565758596061626364656667686970737577808182838488899099100", dest);

  btree_clear(&tree);
  ASSERT_NULL(tree);
}

CTEST(btree_test, btree_erase_even_test) {
        struct btree_node *tree = NULL;
  const uintptr_t         *it   = testcases;

  for (; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    btree_insert(&tree, 4, it, (uintptr_t *)it, less);

  for (; it < testcases + 99; ++it)
    btree_erase(&tree, 4, it, less);

  btree_erase(&tree, 4, it++, less);

  for (; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    btree_erase(&tree, 4, it, less);

  ASSERT_NULL(tree);
}

int main(int argc, const char **argv) { return ctest_main(argc, argv); }
