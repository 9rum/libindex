/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright 2021-2022 9rum
 *
 * bplustree_test.c - generic B+-tree unit test
 */
#define CTEST_MAIN
#define CTEST_SEGFAULT
#define CTEST_COLOR_OK

#include <ctest.h>
#include <index/bplustree.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

      char              __s[131];
      char              __c[4];
const uintptr_t         testcases[] = {
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
const uintptr_t         *__it       = testcases;
      struct btree_node *tree       = NULL;
      struct list_node  *list       = NULL;

bool less(const void *restrict a, const void *restrict b) { return *(uintptr_t *)a < *(uintptr_t *)b; }

void __f(const void *restrict key, void *restrict value) { sprintf(__c, "%" PRIuPTR, *(uintptr_t *)key); strcat(__s, __c); }

CTEST(bplustree_test, test1) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("40", __s);
}

CTEST(bplustree_test, test2) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1140", __s);
}

CTEST(bplustree_test, test3) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("114077", __s);
}

CTEST(bplustree_test, test4) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("11334077", __s);
}

CTEST(bplustree_test, test5) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1120334077", __s);
}

CTEST(bplustree_test, test6) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("112033407790", __s);
}

CTEST(bplustree_test, test7) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("11203340779099", __s);
}

CTEST(bplustree_test, test8) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1120334070779099", __s);
}

CTEST(bplustree_test, test9) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("112033407077889099", __s);
}

CTEST(bplustree_test, test10) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("11203340707780889099", __s);
}

CTEST(bplustree_test, test11) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1120334066707780889099", __s);
}

CTEST(bplustree_test, test12) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("101120334066707780889099", __s);
}

CTEST(bplustree_test, test13) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("10112022334066707780889099", __s);
}

CTEST(bplustree_test, test14) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1011202230334066707780889099", __s);
}

CTEST(bplustree_test, test15) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("101120223033404466707780889099", __s);
}

CTEST(bplustree_test, test16) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("10112022303340445566707780889099", __s);
}

CTEST(bplustree_test, test17) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1011202230334044505566707780889099", __s);
}

CTEST(bplustree_test, test18) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("101120223033404450556066707780889099", __s);
}

CTEST(bplustree_test, test19) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("101120223033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test20) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("10112022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test21) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1011182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test22) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("91011182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test23) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("591011182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test24) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("59101117182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test25) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("569101117182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test26) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("3569101117182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test27) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("13569101117182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test28) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("134569101117182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test29) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234569101117182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test30) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345679101117182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test31) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101117182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test32) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111718202228303340445055606670737780889099100", __s);
}

CTEST(bplustree_test, test33) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121718202228303340445055606670737780889099100", __s);
}

CTEST(bplustree_test, test34) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131718202228303340445055606670737780889099100", __s);
}

CTEST(bplustree_test, test35) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141718202228303340445055606670737780889099100", __s);
}

CTEST(bplustree_test, test36) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314161718202228303340445055606670737780889099100", __s);
}

CTEST(bplustree_test, test37) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202228303340445055606670737780889099100", __s);
}

CTEST(bplustree_test, test38) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222528303340445055606670737780889099100", __s);
}

CTEST(bplustree_test, test39) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340445055606670737780889099100", __s);
}

CTEST(bplustree_test, test40) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340445055606670737780889099100", __s);
}

CTEST(bplustree_test, test41) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334044455055606670737780889099100", __s);
}

CTEST(bplustree_test, test42) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404445495055606670737780889099100", __s);
}

CTEST(bplustree_test, test43) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340424445495055606670737780889099100", __s);
}

CTEST(bplustree_test, test44) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334042434445495055606670737780889099100", __s);
}

CTEST(bplustree_test, test45) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445495055606670737780889099100", __s);
}

CTEST(bplustree_test, test46) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444547495055606670737780889099100", __s);
}

CTEST(bplustree_test, test47) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334041424344454748495055606670737780889099100", __s);
}

CTEST(bplustree_test, test48) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445464748495055606670737780889099100", __s);
}

CTEST(bplustree_test, test49) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505560636670737780889099100", __s);
}

CTEST(bplustree_test, test50) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334041424344454647484950556063666870737780889099100", __s);
}

CTEST(bplustree_test, test51) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445464748495055606163666870737780889099100", __s);
}

CTEST(bplustree_test, test52) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505560616263666870737780889099100", __s);
}

CTEST(bplustree_test, test53) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334041424344454647484950556061626364666870737780889099100", __s);
}

CTEST(bplustree_test, test54) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445464748495055606162636466686970737780889099100", __s);
}

CTEST(bplustree_test, test55) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505560616263646667686970737780889099100", __s);
}

CTEST(bplustree_test, test56) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334041424344454647484950556061626364656667686970737780889099100", __s);
}

CTEST(bplustree_test, test57) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445464748495054556061626364656667686970737780889099100", __s);
}

CTEST(bplustree_test, test58) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505455596061626364656667686970737780889099100", __s);
}

CTEST(bplustree_test, test59) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334041424344454647484950545558596061626364656667686970737780889099100", __s);
}

CTEST(bplustree_test, test60) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445464748495051545558596061626364656667686970737780889099100", __s);
}

CTEST(bplustree_test, test61) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505153545558596061626364656667686970737780889099100", __s);
}

CTEST(bplustree_test, test62) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334041424344454647484950515354555758596061626364656667686970737780889099100", __s);
}

CTEST(bplustree_test, test63) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445464748495051525354555758596061626364656667686970737780889099100", __s);
}

CTEST(bplustree_test, test64) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505152535455565758596061626364656667686970737780889099100", __s);
}

CTEST(bplustree_test, test65) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334041424344454647484950515253545556575859606162636465666768697073778083889099100", __s);
}

CTEST(bplustree_test, test66) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445464748495051525354555657585960616263646566676869707377808183889099100", __s);
}

CTEST(bplustree_test, test67) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505152535455565758596061626364656667686970737780818283889099100", __s);
}

CTEST(bplustree_test, test68) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334041424344454647484950515253545556575859606162636465666768697073778081828384889099100", __s);
}

CTEST(bplustree_test, test69) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445464748495051525354555657585960616263646566676869707375778081828384889099100", __s);
}

CTEST(bplustree_test, test70) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 3, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505152535455565758596061626364656667686970737577808182838488899099100", __s);
}

CTEST(bplustree_test, test71) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445464748495051525354555657585960616263646567686970737577808182838488899099100", __s);
}

CTEST(bplustree_test, test72) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789111213141516171820222425283033404142434445464748495051525354555657585960616263646567686970737577808182838488899099100", __s);
}

CTEST(bplustree_test, test73) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891112131415161718202425283033404142434445464748495051525354555657585960616263646567686970737577808182838488899099100", __s);
}

CTEST(bplustree_test, test74) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678911121314151617182024252833404142434445464748495051525354555657585960616263646567686970737577808182838488899099100", __s);
}

CTEST(bplustree_test, test75) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789111213141516171820242528334041424345464748495051525354555657585960616263646567686970737577808182838488899099100", __s);
}

CTEST(bplustree_test, test76) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891112131415161718202425283340414243454647484950515253545657585960616263646567686970737577808182838488899099100", __s);
}

CTEST(bplustree_test, test77) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678911121314151617182024252833404142434546474849515253545657585960616263646567686970737577808182838488899099100", __s);
}

CTEST(bplustree_test, test78) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789111213141516171820242528334041424345464748495152535456575859616263646567686970737577808182838488899099100", __s);
}

CTEST(bplustree_test, test79) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789111213141516171820242528334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test80) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891112131415161718202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test81) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678911121314151617202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test82) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567811121314151617202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test83) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123467811121314151617202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test84) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234678111213141516202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test85) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123478111213141516202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test86) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12478111213141516202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test87) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("2478111213141516202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test88) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("278111213141516202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test89) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("78111213141516202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test90) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("8111213141516202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test91) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("111213141516202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test92) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1112131415162024253340414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test93) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("11131415162024253340414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test94) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("111415162024253340414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test95) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1115162024253340414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test96) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("11152024253340414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test97) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("112024253340414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test98) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1120243340414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test99) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("11203340414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test100) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("11203340414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test101) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("112033414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test102) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("2033414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test103) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("20334142434546474849515253545657585961626364656768697075808182838488899099", __s);
}

CTEST(bplustree_test, test104) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("204142434546474849515253545657585961626364656768697075808182838488899099", __s);
}

CTEST(bplustree_test, test105) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("4142434546474849515253545657585961626364656768697075808182838488899099", __s);
}

CTEST(bplustree_test, test106) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("41424345464748495152535456575859616263646567686970758081828384888999", __s);
}

CTEST(bplustree_test, test107) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("414243454647484951525354565758596162636465676869707580818283848889", __s);
}

CTEST(bplustree_test, test108) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("4142434546474849515253545657585961626364656768697580818283848889", __s);
}

CTEST(bplustree_test, test109) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("41424345464748495152535456575859616263646567686975808182838489", __s);
}

CTEST(bplustree_test, test110) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("414243454647484951525354565758596162636465676869758182838489", __s);
}

CTEST(bplustree_test, test111) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("4142434647484951525354565758596162636465676869758182838489", __s);
}

CTEST(bplustree_test, test112) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("41424346474851525354565758596162636465676869758182838489", __s);
}

CTEST(bplustree_test, test113) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("414346474851525354565758596162636465676869758182838489", __s);
}

CTEST(bplustree_test, test114) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("4146474851525354565758596162636465676869758182838489", __s);
}

CTEST(bplustree_test, test115) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("46474851525354565758596162636465676869758182838489", __s);
}

CTEST(bplustree_test, test116) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("464851525354565758596162636465676869758182838489", __s);
}

CTEST(bplustree_test, test117) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("4651525354565758596162636465676869758182838489", __s);
}

CTEST(bplustree_test, test118) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("51525354565758596162636465676869758182838489", __s);
}

CTEST(bplustree_test, test119) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("515253545657585961626465676869758182838489", __s);
}

CTEST(bplustree_test, test120) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("5152535456575859616264656769758182838489", __s);
}

CTEST(bplustree_test, test121) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("51525456575859616264656769758182838489", __s);
}

CTEST(bplustree_test, test122) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("515254565859616264656769758182838489", __s);
}

CTEST(bplustree_test, test123) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("5154565859616264656769758182838489", __s);
}

CTEST(bplustree_test, test124) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("51545859616264656769758182838489", __s);
}

CTEST(bplustree_test, test125) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("515458596162646567697581828489", __s);
}

CTEST(bplustree_test, test126) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("5154585961626465676975828489", __s);
}

CTEST(bplustree_test, test127) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("51545859616264656769758489", __s);
}

CTEST(bplustree_test, test128) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("515458596162646567697589", __s);
}

CTEST(bplustree_test, test129) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("5154585961626465676989", __s);
}

CTEST(bplustree_test, test130) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("51545859616264656769", __s);
}

CTEST(bplustree_test, test131) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("515458596264656769", __s);
}

CTEST(bplustree_test, test132) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("5154585964656769", __s);
}

CTEST(bplustree_test, test133) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("51545859656769", __s);
}

CTEST(bplustree_test, test134) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("515458596567", __s);
}

CTEST(bplustree_test, test135) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("5154585965", __s);
}

CTEST(bplustree_test, test136) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("51545859", __s);
}

CTEST(bplustree_test, test137) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("515859", __s);
}

CTEST(bplustree_test, test138) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("5158", __s);
}

CTEST(bplustree_test, test139) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("51", __s);
}

CTEST(bplustree_test, test140) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 3, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("", __s);
}

CTEST(bplustree_test, test141) {
  __it = testcases;
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("40", __s);
}

CTEST(bplustree_test, test142) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1140", __s);
}

CTEST(bplustree_test, test143) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("114077", __s);
}

CTEST(bplustree_test, test144) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("11334077", __s);
}

CTEST(bplustree_test, test145) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1120334077", __s);
}

CTEST(bplustree_test, test146) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("112033407790", __s);
}

CTEST(bplustree_test, test147) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("11203340779099", __s);
}

CTEST(bplustree_test, test148) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1120334070779099", __s);
}

CTEST(bplustree_test, test149) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("112033407077889099", __s);
}

CTEST(bplustree_test, test150) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("11203340707780889099", __s);
}

CTEST(bplustree_test, test151) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1120334066707780889099", __s);
}

CTEST(bplustree_test, test152) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("101120334066707780889099", __s);
}

CTEST(bplustree_test, test153) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("10112022334066707780889099", __s);
}

CTEST(bplustree_test, test154) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1011202230334066707780889099", __s);
}

CTEST(bplustree_test, test155) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("101120223033404466707780889099", __s);
}

CTEST(bplustree_test, test156) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("10112022303340445566707780889099", __s);
}

CTEST(bplustree_test, test157) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1011202230334044505566707780889099", __s);
}

CTEST(bplustree_test, test158) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("101120223033404450556066707780889099", __s);
}

CTEST(bplustree_test, test159) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("101120223033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test160) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("10112022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test161) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1011182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test162) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("91011182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test163) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("591011182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test164) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("59101117182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test165) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("569101117182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test166) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("3569101117182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test167) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("13569101117182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test168) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("134569101117182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test169) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234569101117182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test170) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345679101117182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test171) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101117182022283033404450556066707780889099100", __s);
}

CTEST(bplustree_test, test172) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111718202228303340445055606670737780889099100", __s);
}

CTEST(bplustree_test, test173) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121718202228303340445055606670737780889099100", __s);
}

CTEST(bplustree_test, test174) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131718202228303340445055606670737780889099100", __s);
}

CTEST(bplustree_test, test175) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141718202228303340445055606670737780889099100", __s);
}

CTEST(bplustree_test, test176) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314161718202228303340445055606670737780889099100", __s);
}

CTEST(bplustree_test, test177) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202228303340445055606670737780889099100", __s);
}

CTEST(bplustree_test, test178) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222528303340445055606670737780889099100", __s);
}

CTEST(bplustree_test, test179) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340445055606670737780889099100", __s);
}

CTEST(bplustree_test, test180) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340445055606670737780889099100", __s);
}

CTEST(bplustree_test, test181) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334044455055606670737780889099100", __s);
}

CTEST(bplustree_test, test182) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404445495055606670737780889099100", __s);
}

CTEST(bplustree_test, test183) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340424445495055606670737780889099100", __s);
}

CTEST(bplustree_test, test184) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334042434445495055606670737780889099100", __s);
}

CTEST(bplustree_test, test185) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445495055606670737780889099100", __s);
}

CTEST(bplustree_test, test186) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444547495055606670737780889099100", __s);
}

CTEST(bplustree_test, test187) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334041424344454748495055606670737780889099100", __s);
}

CTEST(bplustree_test, test188) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445464748495055606670737780889099100", __s);
}

CTEST(bplustree_test, test189) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505560636670737780889099100", __s);
}

CTEST(bplustree_test, test190) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334041424344454647484950556063666870737780889099100", __s);
}

CTEST(bplustree_test, test191) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445464748495055606163666870737780889099100", __s);
}

CTEST(bplustree_test, test192) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505560616263666870737780889099100", __s);
}

CTEST(bplustree_test, test193) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334041424344454647484950556061626364666870737780889099100", __s);
}

CTEST(bplustree_test, test194) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445464748495055606162636466686970737780889099100", __s);
}

CTEST(bplustree_test, test195) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505560616263646667686970737780889099100", __s);
}

CTEST(bplustree_test, test196) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334041424344454647484950556061626364656667686970737780889099100", __s);
}

CTEST(bplustree_test, test197) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445464748495054556061626364656667686970737780889099100", __s);
}

CTEST(bplustree_test, test198) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505455596061626364656667686970737780889099100", __s);
}

CTEST(bplustree_test, test199) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334041424344454647484950545558596061626364656667686970737780889099100", __s);
}

CTEST(bplustree_test, test200) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445464748495051545558596061626364656667686970737780889099100", __s);
}

CTEST(bplustree_test, test201) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505153545558596061626364656667686970737780889099100", __s);
}

CTEST(bplustree_test, test202) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334041424344454647484950515354555758596061626364656667686970737780889099100", __s);
}

CTEST(bplustree_test, test203) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445464748495051525354555758596061626364656667686970737780889099100", __s);
}

CTEST(bplustree_test, test204) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505152535455565758596061626364656667686970737780889099100", __s);
}

CTEST(bplustree_test, test205) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334041424344454647484950515253545556575859606162636465666768697073778083889099100", __s);
}

CTEST(bplustree_test, test206) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445464748495051525354555657585960616263646566676869707377808183889099100", __s);
}

CTEST(bplustree_test, test207) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505152535455565758596061626364656667686970737780818283889099100", __s);
}

CTEST(bplustree_test, test208) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789101112131415161718202224252830334041424344454647484950515253545556575859606162636465666768697073778081828384889099100", __s);
}

CTEST(bplustree_test, test209) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445464748495051525354555657585960616263646566676869707375778081828384889099100", __s);
}

CTEST(bplustree_test, test210) {
  memset(__s, 0, sizeof(__s));
  bplus_insert(&tree, &list, 4, __it++, NULL, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891011121314151617182022242528303340414243444546474849505152535455565758596061626364656667686970737577808182838488899099100", __s);
}

CTEST(bplustree_test, test211) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678910111213141516171820222425283033404142434445464748495051525354555657585960616263646567686970737577808182838488899099100", __s);
}

CTEST(bplustree_test, test212) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789111213141516171820222425283033404142434445464748495051525354555657585960616263646567686970737577808182838488899099100", __s);
}

CTEST(bplustree_test, test213) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891112131415161718202425283033404142434445464748495051525354555657585960616263646567686970737577808182838488899099100", __s);
}

CTEST(bplustree_test, test214) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678911121314151617182024252833404142434445464748495051525354555657585960616263646567686970737577808182838488899099100", __s);
}

CTEST(bplustree_test, test215) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789111213141516171820242528334041424345464748495051525354555657585960616263646567686970737577808182838488899099100", __s);
}

CTEST(bplustree_test, test216) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891112131415161718202425283340414243454647484950515253545657585960616263646567686970737577808182838488899099100", __s);
}

CTEST(bplustree_test, test217) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678911121314151617182024252833404142434546474849515253545657585960616263646567686970737577808182838488899099100", __s);
}

CTEST(bplustree_test, test218) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789111213141516171820242528334041424345464748495152535456575859616263646567686970737577808182838488899099100", __s);
}

CTEST(bplustree_test, test219) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123456789111213141516171820242528334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test220) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567891112131415161718202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test221) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12345678911121314151617202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test222) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234567811121314151617202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test223) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123467811121314151617202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test224) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1234678111213141516202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test225) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("123478111213141516202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test226) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("12478111213141516202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test227) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("2478111213141516202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test228) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("278111213141516202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test229) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("78111213141516202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test230) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("8111213141516202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test231) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("111213141516202425334041424345464748495152535456575859616263646567686970737577808182838488899099", __s);
}

CTEST(bplustree_test, test232) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1112131415162024253340414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test233) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("11131415162024253340414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test234) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("111415162024253340414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test235) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1115162024253340414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test236) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("11152024253340414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test237) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("112024253340414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test238) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("1120243340414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test239) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("11203340414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test240) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("11203340414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test241) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("112033414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test242) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("2033414243454647484951525354565758596162636465676869707577808182838488899099", __s);
}

CTEST(bplustree_test, test243) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("20334142434546474849515253545657585961626364656768697075808182838488899099", __s);
}

CTEST(bplustree_test, test244) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("204142434546474849515253545657585961626364656768697075808182838488899099", __s);
}

CTEST(bplustree_test, test245) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("4142434546474849515253545657585961626364656768697075808182838488899099", __s);
}

CTEST(bplustree_test, test246) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("41424345464748495152535456575859616263646567686970758081828384888999", __s);
}

CTEST(bplustree_test, test247) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("414243454647484951525354565758596162636465676869707580818283848889", __s);
}

CTEST(bplustree_test, test248) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("4142434546474849515253545657585961626364656768697580818283848889", __s);
}

CTEST(bplustree_test, test249) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("41424345464748495152535456575859616263646567686975808182838489", __s);
}

CTEST(bplustree_test, test250) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("414243454647484951525354565758596162636465676869758182838489", __s);
}

CTEST(bplustree_test, test251) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("4142434647484951525354565758596162636465676869758182838489", __s);
}

CTEST(bplustree_test, test252) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("41424346474851525354565758596162636465676869758182838489", __s);
}

CTEST(bplustree_test, test253) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("414346474851525354565758596162636465676869758182838489", __s);
}

CTEST(bplustree_test, test254) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("4146474851525354565758596162636465676869758182838489", __s);
}

CTEST(bplustree_test, test255) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("46474851525354565758596162636465676869758182838489", __s);
}

CTEST(bplustree_test, test256) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("464851525354565758596162636465676869758182838489", __s);
}

CTEST(bplustree_test, test257) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("4651525354565758596162636465676869758182838489", __s);
}

CTEST(bplustree_test, test258) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("51525354565758596162636465676869758182838489", __s);
}

CTEST(bplustree_test, test259) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("515253545657585961626465676869758182838489", __s);
}

CTEST(bplustree_test, test260) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("5152535456575859616264656769758182838489", __s);
}

CTEST(bplustree_test, test261) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("51525456575859616264656769758182838489", __s);
}

CTEST(bplustree_test, test262) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("515254565859616264656769758182838489", __s);
}

CTEST(bplustree_test, test263) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("5154565859616264656769758182838489", __s);
}

CTEST(bplustree_test, test264) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("51545859616264656769758182838489", __s);
}

CTEST(bplustree_test, test265) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("515458596162646567697581828489", __s);
}

CTEST(bplustree_test, test266) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("5154585961626465676975828489", __s);
}

CTEST(bplustree_test, test267) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("51545859616264656769758489", __s);
}

CTEST(bplustree_test, test268) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("515458596162646567697589", __s);
}

CTEST(bplustree_test, test269) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("5154585961626465676989", __s);
}

CTEST(bplustree_test, test270) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("51545859616264656769", __s);
}

CTEST(bplustree_test, test271) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("515458596264656769", __s);
}

CTEST(bplustree_test, test272) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("5154585964656769", __s);
}

CTEST(bplustree_test, test273) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("51545859656769", __s);
}

CTEST(bplustree_test, test274) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("515458596567", __s);
}

CTEST(bplustree_test, test275) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("5154585965", __s);
}

CTEST(bplustree_test, test276) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("51545859", __s);
}

CTEST(bplustree_test, test277) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("515859", __s);
}

CTEST(bplustree_test, test278) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("5158", __s);
}

CTEST(bplustree_test, test279) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("51", __s);
}

CTEST(bplustree_test, test280) {
  memset(__s, 0, sizeof(__s));
  bplus_erase(&tree, &list, 4, __it++, less);
  bplus_for_each(list, __f);
  ASSERT_STR("", __s);
}

int main(int argc, const char **argv) { return ctest_main(argc, argv); }
