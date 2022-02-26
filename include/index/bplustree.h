/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2020-2022 9rum
 *
 * bplustree.h - generic B+-tree declaration
 *
 * A B+-tree is an m-ary tree with large number of children per node.
 * It can be viewed as a B-tree in which each node contains only keys (not key–value pairs),
 * and to which an additional level is added at the bottom with linked leaves.
 *
 * The primary value of a B+-tree is in storing data for efficient retrieval in a block-oriented storage context
 * — in particular, filesystems.
 * This is primarily because unlike binary search trees, B+-trees have very high fanout,
 * which reduces the number of I/O operations required to find an element in the tree.
 *
 * The ReiserFS, NSS, XFS, JFS, ReFS, and BFS filesystems all use this type of tree for metadata indexing;
 * BFS also uses B+-trees for storing directories.
 * NTFS uses B+-trees for directory and security-related metadata indexing.
 * EXT4 uses extent trees for file extent indexing.
 * APFS uses B+-trees to store mappings from filesystem object IDs to their locations on disk,
 * and to store filesystem records (including directories), though these trees' leaf nodes lack sibling pointers.
 *
 * Relational database management systems such as IBM DB2, Informix, Microsoft SQL Server, Oracle 8, Sybase ASE,
 * and SQLite support this type of tree for table indices.
 * Key–value database management systems such as CouchDB and Tokyo Cabinet support this type of tree for data access.
 *
 * There is no single paper introducing the B+-tree concept.
 * Instead, the notion of maintaining all data in leaf nodes is repeatedly brought up as an interesting variant.
 * An early survey of B-trees also covering B+-trees is Douglas Comer.
 * Comer notes that the B+-tree was used in IBM's VSAM data access software and he refers to an IBM published article from 1973.
 *
 * See http://carlosproal.com/ir/papers/p121-comer.pdf for more details.
 */
#ifndef _INDEX_BPLUSTREE_H
#define _INDEX_BPLUSTREE_H

#include <stdbool.h>
#include <stddef.h>

/**
 * struct bplus_internal_node - an internal node in B+-tree
 *
 * @keys:     the ordered set of keys of the node
 * @children: the ordered set of children of the node
 * @nmemb:    the number of the keys of the node
 * @type:     the type of the node
 */
struct bplus_internal_node {
  const void   **keys;
        void   **children;
        size_t nmemb;
        bool   type;
} __attribute__((aligned(__SIZEOF_POINTER__)));

/**
 * struct bplus_external_node - an external node in B+-tree
 *
 * @keys:   the ordered set of keys of the node
 * @values: the ordered set of values of the node
 * @next:   the address of the next node
 * @nmemb:  the number of the keys of the node
 */
struct bplus_external_node {
  const void                       **keys;
        void                       **values;
        struct bplus_external_node *next;
        size_t                     nmemb;
} __attribute__((aligned(__SIZEOF_POINTER__)));

struct bplus_root {
        struct bplus_internal_node *root;
        struct bplus_external_node *head;
        bool                      (*less)(const void *restrict, const void *restrict);
        size_t                     size;
  const size_t                     order;
} __attribute__((aligned(__SIZEOF_POINTER__)));

/*
 * The below functions use the operator with 3 different
 * calling conventions. The operator denotes:
 *
 *    a < b  := less(a, b)
 *    a > b  := less(b, a)
 *    a == b := !less(a, b) && !less(b, a)
 *
 * NOTE:
 *
 * less must describe a transitive ordering:
 *  - if both less(a, b) and less(b, c) are true, then less(a, c) must be true as well
 *  - if both less(a, b) and less(b, c) are false, then less(a, c) must be false as well
 */

/**
 * bplus_init - initializes an empty tree of @order with @less
 *
 * @order: the order of tree
 * @less:  operator defining the (partial) element order
 */
static inline struct bplus_root bplus_init(const size_t order, bool (*less)(const void *restrict, const void *restrict)) {
  struct bplus_root tree = {
    .root  = NULL,
    .head  = NULL,
    .less  = less,
    .size  = 0,
    .order = order,
  };
  return tree;
}

/**
 * bplus_size - returns the number of elements in @tree
 *
 * @tree: tree to get the number of elements
 */
static inline size_t bplus_size(const struct bplus_root tree) { return tree.size; }

/**
 * bplus_empty - checks whether @tree is empty
 *
 * @tree: tree to check
 */
static inline bool bplus_empty(const struct bplus_root tree) { return bplus_size(tree) == 0; }

/**
 * bplus_find - finds element from @tree with @key
 *
 * @tree: tree to find element from
 * @key:  the key to search for
 */
extern void *bplus_find(const struct bplus_root tree, const void *restrict key);

/**
 * bplus_contains - checks if @tree contains element with @key
 *
 * @tree: tree to check
 * @key:  the key to search for
 */
extern bool bplus_contains(const struct bplus_root tree, const void *restrict key);

/**
 * bplus_insert - inserts an element into @tree
 *
 * @tree:  tree to insert element into
 * @key:   the key of the element to insert
 * @value: the value of the element to insert
 */
extern struct bplus_external_node *bplus_insert(struct bplus_root *restrict tree, const void *restrict key, void *restrict value);

/**
 * bplus_insert_or_assign - inserts an element or assigns @value if @key already exists
 *
 * @tree:  tree to insert element into
 * @key:   the key of the element to insert if not found
 * @value: the value of the element to insert or assign
 */
extern struct bplus_external_node *bplus_insert_or_assign(struct bplus_root *restrict tree, const void *restrict key, void *restrict value);

/**
 * bplus_erase - removes the element with @key from @tree
 *
 * @tree: tree to remove the element from
 * @key:  the key of the element to remove
 */
extern void *bplus_erase(struct bplus_root *restrict tree, const void *restrict key);

/**
 * bplus_clear - erases all elements from @tree
 *
 * @tree: tree to erase all elements from
 */
extern void bplus_clear(struct bplus_root *restrict tree);

/**
 * bplus_for_each - applies @func to each element of @tree sequentialwise
 *
 * @tree: tree to apply @func to each element of
 * @func: function to apply to each element of @tree
 */
static inline void bplus_for_each(const struct bplus_root tree, void (*func)(const void *restrict, void *restrict)) {
  for (register const struct bplus_external_node *node = tree.head; node != NULL; node = node->next)
    for (register size_t idx = 0; idx < node->nmemb; ++idx)
      func(node->keys[idx], node->values[idx]);
}

#endif /* _INDEX_BPLUSTREE_H */
