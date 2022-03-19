/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2021-2022 9rum
 *
 * llrbtree_test.c - generic left-leaning red-black tree unit test
 */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#define CTEST_MAIN
#define CTEST_SEGFAULT
#define CTEST_COLOR_OK

#include <ctest.h>
#include <index/llrbtree.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

const uintptr_t testcases[] = {40, 11, 77, 33, 20, 90, 99, 70, 88, 80, 66, 10, 22, 30, 44, 55, 50, 60, 25, 49};

bool less(const void *restrict lhs, const void *restrict rhs) { return (uintptr_t)lhs < (uintptr_t)rhs; }

CTEST(llrbtree_test, llrb_find_test) {
  struct llrb_root tree = llrb_init(less);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    llrb_insert(&tree, (void *)*it, (void *)*it);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)llrb_find(tree, (void *)*it).value);

  llrb_clear(&tree);
  ASSERT_TRUE(llrb_empty(tree));
}

CTEST(llrbtree_test, llrb_insert_test) {
  struct llrb_root tree = llrb_init(less);
  char             src[3];
  char             dest[41];

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)llrb_insert(&tree, (void *)*it, NULL).key);

  memset(dest, 0, sizeof(dest));
  for (struct llrb_iter iter = llrb_iter_init(tree); !llrb_iter_end(iter); llrb_iter_next(&iter)) {
    sprintf(src, "%" PRIuPTR, (uintptr_t)iter.key);
    strcat(dest, src);
  }
  ASSERT_STR("1011202225303340444950556066707780889099", dest);
  ASSERT_EQUAL_U(sizeof(testcases)/sizeof(uintptr_t), llrb_size(tree));

  llrb_clear(&tree);
  ASSERT_TRUE(llrb_empty(tree));
}

CTEST(llrbtree_test, llrb_replace_test) {
  struct llrb_root tree = llrb_init(less);
  char             src[3];
  char             dest[41];

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_NULL(llrb_replace(&tree, (void *)*it, NULL).value);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)llrb_replace(&tree, (void *)*it, (void *)*it).value);

  memset(dest, 0, sizeof(dest));
  for (struct llrb_iter iter = llrb_iter_init(tree); !llrb_iter_end(iter); llrb_iter_next(&iter)) {
    sprintf(src, "%" PRIuPTR, (uintptr_t)iter.key);
    strcat(dest, src);
  }
  ASSERT_STR("1011202225303340444950556066707780889099", dest);
  ASSERT_EQUAL_U(sizeof(testcases)/sizeof(uintptr_t), llrb_size(tree));

  llrb_clear(&tree);
  ASSERT_TRUE(llrb_empty(tree));
}

CTEST(llrbtree_test, llrb_erase_test) {
  struct llrb_root tree = llrb_init(less);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    llrb_insert(&tree, (void *)*it, (void *)*it);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)llrb_erase(&tree, (void *)*it));

  ASSERT_TRUE(llrb_empty(tree));
}

CTEST(llrbtree_test, llrb_erase_reverse_test) {
  struct llrb_root tree = llrb_init(less);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    llrb_insert(&tree, (void *)*it, (void *)*it);

  for (const uintptr_t *it = testcases + sizeof(testcases)/sizeof(uintptr_t) - 1; testcases <= it; --it)
    ASSERT_EQUAL_U(*it, (uintptr_t)llrb_erase(&tree, (void *)*it));

  ASSERT_TRUE(llrb_empty(tree));
}

int main(int argc, const char **argv) { return ctest_main(argc, argv); }
