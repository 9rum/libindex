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
#include <stdint.h>
#include <stdio.h>
#include <string.h>

const uintptr_t testcases[] = {40, 11, 77, 33, 20, 90, 99, 70, 88, 80, 66, 10, 22, 30, 44, 55, 50, 60, 25, 49};

bool less(const void *restrict lhs, const void *restrict rhs) { return (uintptr_t)lhs < (uintptr_t)rhs; }

CTEST(rbtree_test, rb_find_test) {
  struct rb_root tree = rb_init(less);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    rb_insert(&tree, (void *)*it, (void *)*it);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)rb_find(tree, (void *)*it).value);

  rb_clear(&tree);
  ASSERT_TRUE(rb_empty(tree));
}

CTEST(rbtree_test, rb_insert_test) {
  struct rb_root tree = rb_init(less);
  char           src[3];
  char           dest[41];

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)rb_insert(&tree, (void *)*it, NULL).key);

  memset(dest, 0, sizeof(dest));
  for (struct rb_iter iter = rb_iter_init(tree); !rb_iter_end(iter); rb_iter_next(&iter)) {
    sprintf(src, "%" PRIuPTR, (uintptr_t)iter.key);
    strcat(dest, src);
  }
  ASSERT_STR("1011202225303340444950556066707780889099", dest);
  ASSERT_EQUAL_U(sizeof(testcases)/sizeof(uintptr_t), rb_size(tree));

  rb_clear(&tree);
  ASSERT_TRUE(rb_empty(tree));
}

CTEST(rbtree_test, rb_replace_test) {
  struct rb_root tree = rb_init(less);
  char           src[3];
  char           dest[41];

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_NULL(rb_replace(&tree, (void *)*it, NULL).value);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)rb_replace(&tree, (void *)*it, (void *)*it).value);

  memset(dest, 0, sizeof(dest));
  for (struct rb_iter iter = rb_iter_init(tree); !rb_iter_end(iter); rb_iter_next(&iter)) {
    sprintf(src, "%" PRIuPTR, (uintptr_t)iter.key);
    strcat(dest, src);
  }
  ASSERT_STR("1011202225303340444950556066707780889099", dest);
  ASSERT_EQUAL_U(sizeof(testcases)/sizeof(uintptr_t), rb_size(tree));

  rb_clear(&tree);
  ASSERT_TRUE(rb_empty(tree));
}

CTEST(rbtree_test, rb_erase_test) {
  struct rb_root tree = rb_init(less);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    rb_insert(&tree, (void *)*it, (void *)*it);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    ASSERT_EQUAL_U(*it, (uintptr_t)rb_erase(&tree, (void *)*it));

  ASSERT_TRUE(rb_empty(tree));
}

CTEST(rbtree_test, rb_erase_reverse_test) {
  struct rb_root tree = rb_init(less);

  for (const uintptr_t *it = testcases; it < testcases + sizeof(testcases)/sizeof(uintptr_t); ++it)
    rb_insert(&tree, (void *)*it, (void *)*it);

  for (const uintptr_t *it = testcases + sizeof(testcases)/sizeof(uintptr_t) - 1; testcases <= it; --it)
    ASSERT_EQUAL_U(*it, (uintptr_t)rb_erase(&tree, (void *)*it));

  ASSERT_TRUE(rb_empty(tree));
}

int main(int argc, const char **argv) { return ctest_main(argc, argv); }
