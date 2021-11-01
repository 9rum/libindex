/*
 * Copyright 2020 9rum
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * File Processing, 2020
 *
 * rbtree_test.c - generic red-black tree unit test
 */
#define CTEST_MAIN
#define CTEST_SEGFAULT
#define CTEST_COLOR_OK

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <rbtree.h>
#include <ctest.h>

      char           __s[41];
      char           __c[3];
const uintptr_t      testcases[] = {40, 11, 77, 33, 20, 90, 99, 70, 88, 80, 66, 10, 22, 30, 44, 55, 50, 60, 25, 49};
const uintptr_t      *__it       = testcases;
      struct rb_node *tree       = NULL;

bool less(const void *a, const void *b) { return *(uintptr_t *)a < *(uintptr_t *)b; }

void __f(const void *restrict key, void *restrict value) { sprintf(__c, "%" PRIuPTR, *(uintptr_t *)key); strcat(__s, __c); }

CTEST(rbtree_test, test1) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "40");
}

CTEST(rbtree_test, test2) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1140");
}

CTEST(rbtree_test, test3) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "114077");
}

CTEST(rbtree_test, test4) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "11334077");
}

CTEST(rbtree_test, test5) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1120334077");
}

CTEST(rbtree_test, test6) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "112033407790");
}

CTEST(rbtree_test, test7) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "11203340779099");
}

CTEST(rbtree_test, test8) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1120334070779099");
}

CTEST(rbtree_test, test9) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "112033407077889099");
}

CTEST(rbtree_test, test10) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "11203340707780889099");
}

CTEST(rbtree_test, test11) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1120334066707780889099");
}

CTEST(rbtree_test, test12) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "101120334066707780889099");
}

CTEST(rbtree_test, test13) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "10112022334066707780889099");
}

CTEST(rbtree_test, test14) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1011202230334066707780889099");
}

CTEST(rbtree_test, test15) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "101120223033404466707780889099");
}

CTEST(rbtree_test, test16) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "10112022303340445566707780889099");
}

CTEST(rbtree_test, test17) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1011202230334044505566707780889099");
}

CTEST(rbtree_test, test18) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "101120223033404450556066707780889099");
}

CTEST(rbtree_test, test19) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "10112022253033404450556066707780889099");
}

CTEST(rbtree_test, test20) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1011202225303340444950556066707780889099");
}

CTEST(rbtree_test, test21) {
  __it = testcases;
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "10112022253033444950556066707780889099");
}

CTEST(rbtree_test, test22) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "102022253033444950556066707780889099");
}

CTEST(rbtree_test, test23) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1020222530334449505560667080889099");
}

CTEST(rbtree_test, test24) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "10202225304449505560667080889099");
}

CTEST(rbtree_test, test25) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "102225304449505560667080889099");
}

CTEST(rbtree_test, test26) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1022253044495055606670808899");
}

CTEST(rbtree_test, test27) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "10222530444950556066708088");
}

CTEST(rbtree_test, test28) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "102225304449505560668088");
}

CTEST(rbtree_test, test29) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1022253044495055606680");
}

CTEST(rbtree_test, test30) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "10222530444950556066");
}

CTEST(rbtree_test, test31) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "102225304449505560");
}

CTEST(rbtree_test, test32) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "2225304449505560");
}

CTEST(rbtree_test, test33) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "25304449505560");
}

CTEST(rbtree_test, test34) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "254449505560");
}

CTEST(rbtree_test, test35) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "2549505560");
}

CTEST(rbtree_test, test36) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "25495060");
}

CTEST(rbtree_test, test37) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "254960");
}

CTEST(rbtree_test, test38) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "2549");
}

CTEST(rbtree_test, test39) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "49");
}

CTEST(rbtree_test, test40) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it++, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "");
}

CTEST(rbtree_test, test41) {
  __it = testcases;
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "40");
}

CTEST(rbtree_test, test42) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1140");
}

CTEST(rbtree_test, test43) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "114077");
}

CTEST(rbtree_test, test44) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "11334077");
}

CTEST(rbtree_test, test45) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1120334077");
}

CTEST(rbtree_test, test46) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "112033407790");
}

CTEST(rbtree_test, test47) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "11203340779099");
}

CTEST(rbtree_test, test48) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1120334070779099");
}

CTEST(rbtree_test, test49) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "112033407077889099");
}

CTEST(rbtree_test, test50) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "11203340707780889099");
}

CTEST(rbtree_test, test51) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1120334066707780889099");
}

CTEST(rbtree_test, test52) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "101120334066707780889099");
}

CTEST(rbtree_test, test53) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "10112022334066707780889099");
}

CTEST(rbtree_test, test54) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1011202230334066707780889099");
}

CTEST(rbtree_test, test55) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "101120223033404466707780889099");
}

CTEST(rbtree_test, test56) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "10112022303340445566707780889099");
}

CTEST(rbtree_test, test57) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1011202230334044505566707780889099");
}

CTEST(rbtree_test, test58) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "101120223033404450556066707780889099");
}

CTEST(rbtree_test, test59) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "10112022253033404450556066707780889099");
}

CTEST(rbtree_test, test60) {
  memset(__s, 0, sizeof(__s));
  rb_insert(&tree, __it++, NULL, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1011202225303340444950556066707780889099");
}

CTEST(rbtree_test, test61) {
  __it = testcases + sizeof(testcases)/sizeof(uintptr_t) - 1;
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "10112022253033404450556066707780889099");
}

CTEST(rbtree_test, test62) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "101120223033404450556066707780889099");
}

CTEST(rbtree_test, test63) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1011202230334044505566707780889099");
}

CTEST(rbtree_test, test64) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "10112022303340445566707780889099");
}

CTEST(rbtree_test, test65) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "101120223033404466707780889099");
}

CTEST(rbtree_test, test66) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1011202230334066707780889099");
}

CTEST(rbtree_test, test67) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "10112022334066707780889099");
}

CTEST(rbtree_test, test68) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "101120334066707780889099");
}

CTEST(rbtree_test, test69) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1120334066707780889099");
}

CTEST(rbtree_test, test70) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "11203340707780889099");
}

CTEST(rbtree_test, test71) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "112033407077889099");
}

CTEST(rbtree_test, test72) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1120334070779099");
}

CTEST(rbtree_test, test73) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "11203340779099");
}

CTEST(rbtree_test, test74) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "112033407790");
}

CTEST(rbtree_test, test75) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1120334077");
}

CTEST(rbtree_test, test76) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "11334077");
}

CTEST(rbtree_test, test77) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "114077");
}

CTEST(rbtree_test, test78) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "1140");
}

CTEST(rbtree_test, test79) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "40");
}

CTEST(rbtree_test, test80) {
  memset(__s, 0, sizeof(__s));
  rb_erase(&tree, __it--, less);
  rb_inorder(tree, __f);
  ASSERT_STR(__s, "");
}

int main(int argc, const char **argv) { return ctest_main(argc, argv); }
