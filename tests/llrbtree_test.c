/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2021-2022 9rum
 *
 * llrbtree_test.c - generic left-leaning red-black tree unit test
 */
#define CTEST_MAIN
#define CTEST_SEGFAULT
#define CTEST_COLOR_OK

#include <ctest.h>
#include <index/llrbtree.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

      char      src[3];
      char      dest[41];
const uintptr_t testcases[] = {40, 11, 77, 33, 20, 90, 99, 70, 88, 80, 66, 10, 22, 30, 44, 55, 50, 60, 25, 49};

bool less(const void *restrict a, const void *restrict b) { return *(uintptr_t *)a < *(uintptr_t *)b; }

void concat(const void *restrict key, void *restrict value) { sprintf(src, "%" PRIuPTR, *(uintptr_t *)key); strcat(dest, src); }

CTEST(llrbtree_test, llrb_insert_test) {
  struct llrb_node *tree = NULL;

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    llrb_insert(&tree, it, NULL, less);

  memset(dest, 0, sizeof(dest));
  llrb_inorder(tree, concat);
  ASSERT_STR("1011202225303340444950556066707780889099", dest);

  llrb_clear(&tree);
  ASSERT_NULL(tree);
}

CTEST(llrbtree_test, llrb_erase_test) {
  struct llrb_node *tree = NULL;

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    llrb_insert(&tree, it, (uintptr_t *)it, less);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    llrb_erase(&tree, it, less);

  ASSERT_NULL(tree);
}

CTEST(llrbtree_test, llrb_erase_reverse_test) {
  struct llrb_node *tree = NULL;

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    llrb_insert(&tree, it, (uintptr_t *)it, less);

  for (const uintptr_t *it = testcases + sizeof(testcases)/sizeof(uintptr_t) - 1; testcases <= it; --it)
    llrb_erase(&tree, it, less);

  ASSERT_NULL(tree);
}

int main(int argc, const char **argv) { return ctest_main(argc, argv); }
