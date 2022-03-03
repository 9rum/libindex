/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2021-2022 9rum
 *
 * bplustree_test.c - generic B+-tree unit test
 */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#define CTEST_MAIN
#define CTEST_SEGFAULT
#define CTEST_COLOR_OK

#include <ctest.h>
#include <index/bplustree.h>
#include <stdint.h>
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

bool less(const void *restrict lhs, const void *restrict rhs) { return (uintptr_t)lhs < (uintptr_t)rhs; }

void concat(const void *restrict key, void *restrict value) { sprintf(src, "%" PRIuPTR, (uintptr_t)key); strcat(dest, src); }

CTEST(bplustree_test, bplus_find_test) {
  struct bplus_root tree = bplus_init(3, less);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    bplus_insert(&tree, (void *)*it, (void *)*it);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)bplus_find(tree, (void *)*it));

  bplus_clear(&tree);
  ASSERT_TRUE(bplus_empty(tree));
}

CTEST(bplustree_test, bplus_insert_odd_test) {
        struct bplus_root tree = bplus_init(3, less);
  const uintptr_t         *it  = testcases;

  for (; it < testcases + 39; ++it)
    ASSERT_NOT_NULL(bplus_insert(&tree, (void *)*it, NULL));

  ASSERT_NULL(bplus_insert(&tree, (void *)*it, NULL));

  for (++it; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    ASSERT_NOT_NULL(bplus_insert(&tree, (void *)*it, NULL));

  memset(dest, 0, sizeof(dest));
  bplus_for_each(tree, concat);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505152535455565758596061626364656667686970737577808182838488899099100", dest);
  ASSERT_EQUAL_U(sizeof(testcases)/sizeof(uintptr_t)/2-1, bplus_size(tree));

  bplus_clear(&tree);
  ASSERT_TRUE(bplus_empty(tree));
}

CTEST(bplustree_test, bplus_insert_or_assign_odd_test) {
  struct bplus_root tree = bplus_init(3, less);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    ASSERT_NOT_NULL(bplus_insert_or_assign(&tree, (void *)*it, NULL));

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    ASSERT_NOT_NULL(bplus_insert_or_assign(&tree, (void *)*it, (void *)*it));

  memset(dest, 0, sizeof(dest));
  bplus_for_each(tree, concat);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505152535455565758596061626364656667686970737577808182838488899099100", dest);
  ASSERT_EQUAL_U(sizeof(testcases)/sizeof(uintptr_t)/2-1, bplus_size(tree));

  bplus_clear(&tree);
  ASSERT_TRUE(bplus_empty(tree));
}

CTEST(bplustree_test, bplus_erase_odd_test) {
        struct bplus_root tree = bplus_init(3, less);
  const uintptr_t         *it  = testcases;

  for (; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    bplus_insert(&tree, (void *)*it, (void *)*it);

  for (; it < testcases + 99; ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)bplus_erase(&tree, (void *)*it));

  ASSERT_NULL(bplus_erase(&tree, (void *)*it));

  for (++it; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)bplus_erase(&tree, (void *)*it));

  ASSERT_TRUE(bplus_empty(tree));
}

CTEST(bplustree_test, bplus_insert_even_test) {
        struct bplus_root tree = bplus_init(4, less);
  const uintptr_t         *it  = testcases;

  for (; it < testcases + 39; ++it)
    ASSERT_NOT_NULL(bplus_insert(&tree, (void *)*it, NULL));

  ASSERT_NULL(bplus_insert(&tree, (void *)*it, NULL));

  for (++it; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    ASSERT_NOT_NULL(bplus_insert(&tree, (void *)*it, NULL));

  memset(dest, 0, sizeof(dest));
  bplus_for_each(tree, concat);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505152535455565758596061626364656667686970737577808182838488899099100", dest);
  ASSERT_EQUAL_U(sizeof(testcases)/sizeof(uintptr_t)/2-1, bplus_size(tree));

  bplus_clear(&tree);
  ASSERT_TRUE(bplus_empty(tree));
}

CTEST(bplustree_test, bplus_insert_or_assign_even_test) {
  struct bplus_root tree = bplus_init(4, less);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    ASSERT_NOT_NULL(bplus_insert_or_assign(&tree, (void *)*it, NULL));

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    ASSERT_NOT_NULL(bplus_insert_or_assign(&tree, (void *)*it, (void *)*it));

  memset(dest, 0, sizeof(dest));
  bplus_for_each(tree, concat);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505152535455565758596061626364656667686970737577808182838488899099100", dest);
  ASSERT_EQUAL_U(sizeof(testcases)/sizeof(uintptr_t)/2-1, bplus_size(tree));

  bplus_clear(&tree);
  ASSERT_TRUE(bplus_empty(tree));
}

CTEST(bplustree_test, bplus_erase_even_test) {
        struct bplus_root tree = bplus_init(4, less);
  const uintptr_t         *it  = testcases;

  for (; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    bplus_insert(&tree, (void *)*it, (void *)*it);

  for (; it < testcases + 99; ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)bplus_erase(&tree, (void *)*it));

  ASSERT_NULL(bplus_erase(&tree, (void *)*it));

  for (++it; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)bplus_erase(&tree, (void *)*it));

  ASSERT_TRUE(bplus_empty(tree));
}

CTEST(bplustree_test, bplus_range_each_test) {
  struct bplus_root tree = bplus_init(4, less);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t)/2; ++it)
    bplus_insert(&tree, (void *)*it, NULL);

  memset(dest, 0, sizeof(dest));
  bplus_range_each(tree, (void *)0, (void *)101, concat);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505152535455565758596061626364656667686970737577808182838488899099100", dest);

  memset(dest, 0, sizeof(dest));
  bplus_range_each(tree, (void *)30, (void *)76, concat);
  ASSERT_STR("3033404142434445464748495051525354555657585960616263646566676869707375", dest);

  memset(dest, 0, sizeof(dest));
  bplus_range_each(tree, (void *)19, (void *)70, concat);
  ASSERT_STR("20222425283033404142434445464748495051525354555657585960616263646566676869", dest);

  memset(dest, 0, sizeof(dest));
  bplus_range_each(tree, (void *)16, (void *)61, concat);
  ASSERT_STR("16171820222425283033404142434445464748495051525354555657585960", dest);

  bplus_clear(&tree);
  ASSERT_TRUE(bplus_empty(tree));
}

int main(int argc, const char **argv) { return ctest_main(argc, argv); }
