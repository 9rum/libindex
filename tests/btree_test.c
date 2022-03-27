/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2021-2022 9rum
 *
 * btree_test.c - generic B-tree unit test
 */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#define CTEST_MAIN
#define CTEST_SEGFAULT
#define CTEST_COLOR_OK

#include <ctest.h>
#include <index/btree.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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

bool less(const void *restrict lhs, const void *restrict rhs) { return (uintptr_t)lhs < (uintptr_t)rhs; }

CTEST(btree_test, btree_find_test) {
  struct btree_root tree = btree_init(3, less);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    btree_insert(&tree, (void *)*it, (void *)*it);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)btree_find(tree, (void *)*it).value);

  btree_clear(&tree);
  ASSERT_TRUE(btree_empty(tree));
}

CTEST(btree_test, btree_insert_odd_test) {
  struct btree_root tree = btree_init(3, less);
  char              src[4];
  char              dest[131];

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)btree_insert(&tree, (void *)*it, NULL).key);

  memset(dest, 0, sizeof(dest));
  for (struct btree_iter iter = btree_iter_init(tree); !btree_iter_end(iter); btree_iter_next(&iter)) {
    sprintf(src, "%" PRIuPTR, (uintptr_t)iter.key);
    strcat(dest, src);
  }
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505152535455565758596061626364656667686970737577808182838488899099100", dest);
  ASSERT_EQUAL_U(sizeof(testcases)/sizeof(uintptr_t)/2-1, btree_size(tree));

  btree_clear(&tree);
  ASSERT_TRUE(btree_empty(tree));
}

CTEST(btree_test, btree_replace_odd_test) {
  struct btree_root tree = btree_init(3, less);
  char              src[4];
  char              dest[131];

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    ASSERT_NULL(btree_replace(&tree, (void *)*it, NULL).value);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)btree_replace(&tree, (void *)*it, (void *)*it).value);

  memset(dest, 0, sizeof(dest));
  for (struct btree_iter iter = btree_iter_init(tree); !btree_iter_end(iter); btree_iter_next(&iter)) {
    sprintf(src, "%" PRIuPTR, (uintptr_t)iter.key);
    strcat(dest, src);
  }
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505152535455565758596061626364656667686970737577808182838488899099100", dest);
  ASSERT_EQUAL_U(sizeof(testcases)/sizeof(uintptr_t)/2-1, btree_size(tree));

  btree_clear(&tree);
  ASSERT_TRUE(btree_empty(tree));
}

CTEST(btree_test, btree_erase_odd_test) {
        struct btree_root tree = btree_init(3, less);
  const uintptr_t         *it  = testcases;

  for (; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    btree_insert(&tree, (void *)*it, (void *)*it);

  for (; it < testcases + 99; ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)btree_erase(&tree, (void *)*it));

  ASSERT_NULL(btree_erase(&tree, (void *)*it));

  for (++it; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)btree_erase(&tree, (void *)*it));

  ASSERT_TRUE(btree_empty(tree));
}

CTEST(btree_test, btree_insert_even_test) {
  struct btree_root tree = btree_init(4, less);
  char              src[4];
  char              dest[131];

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)btree_insert(&tree, (void *)*it, NULL).key);

  memset(dest, 0, sizeof(dest));
  for (struct btree_iter iter = btree_iter_init(tree); !btree_iter_end(iter); btree_iter_next(&iter)) {
    sprintf(src, "%" PRIuPTR, (uintptr_t)iter.key);
    strcat(dest, src);
  }
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505152535455565758596061626364656667686970737577808182838488899099100", dest);
  ASSERT_EQUAL_U(sizeof(testcases)/sizeof(uintptr_t)/2-1, btree_size(tree));

  btree_clear(&tree);
  ASSERT_TRUE(btree_empty(tree));
}

CTEST(btree_test, btree_replace_even_test) {
  struct btree_root tree = btree_init(4, less);
  char              src[4];
  char              dest[131];

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    ASSERT_NULL(btree_replace(&tree, (void *)*it, NULL).value);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)btree_replace(&tree, (void *)*it, (void *)*it).value);

  memset(dest, 0, sizeof(dest));
  for (struct btree_iter iter = btree_iter_init(tree); !btree_iter_end(iter); btree_iter_next(&iter)) {
    sprintf(src, "%" PRIuPTR, (uintptr_t)iter.key);
    strcat(dest, src);
  }
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505152535455565758596061626364656667686970737577808182838488899099100", dest);
  ASSERT_EQUAL_U(sizeof(testcases)/sizeof(uintptr_t)/2-1, btree_size(tree));

  btree_clear(&tree);
  ASSERT_TRUE(btree_empty(tree));
}

CTEST(btree_test, btree_erase_even_test) {
        struct btree_root tree = btree_init(4, less);
  const uintptr_t         *it  = testcases;

  for (; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    btree_insert(&tree, (void *)*it, (void *)*it);

  for (; it < testcases + 99; ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)btree_erase(&tree, (void *)*it));

  ASSERT_NULL(btree_erase(&tree, (void *)*it));

  for (++it; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)btree_erase(&tree, (void *)*it));

  ASSERT_TRUE(btree_empty(tree));
}

int main(int argc, const char **argv) { return ctest_main(argc, argv); }
