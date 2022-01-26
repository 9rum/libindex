/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright 2020-2022 9rum
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

#include <string.h>
#include <index/stack.h>

/**
 * struct btree_node - an internal node in B+-tree
 *
 * @keys:     the ordered set of keys of the node
 * @children: the ordered set of children of the node
 * @nmemb:    the number of the keys of the node
 * @type:     the type of the node
 */
struct btree_node {
  const void   **keys;
        void   **children;
        size_t nmemb;
        bool   type;
} __attribute__((aligned(__SIZEOF_POINTER__)));

/**
 * struct list_node - an external node in B+-tree
 *
 * @keys:   the ordered set of keys of the node
 * @values: the ordered set of values of the node
 * @next:   the pointer to the next node
 * @nmemb:  the number of the keys of the node
 */
struct list_node {
  const void             **keys;
        void             **values;
        struct list_node *next;
        size_t           nmemb;
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
 * bplus_insert - inserts @key and @value into @tree and @list
 *
 * @tree:  tree to insert @key and @value into
 * @list:  list to insert @key and @value into
 * @order: order of B+-tree
 * @key:   the key to insert
 * @value: the value to insert
 * @less:  operator defining the (partial) node order
 */
extern void bplus_insert(struct btree_node **restrict tree, struct list_node **restrict list, const size_t order, const void *restrict key, void *restrict value, bool (*less)(const void *restrict, const void *restrict));

/**
 * bplus_erase - erases @key from @tree and @list
 *
 * @tree:  tree to erase @key from
 * @list:  list to erase @key from
 * @order: order of B+-tree
 * @key:   the key to erase
 * @less:  operator defining the (partial) node order
 */
extern void bplus_erase(struct btree_node **restrict tree, struct list_node **restrict list, const size_t order, const void *restrict key, bool (*less)(const void *restrict, const void *restrict));

/**
 * bplus_for_each - applies @func to each node of @list sequentialwise
 *
 * @list: list to apply @func to each node of
 * @func: function to apply to each node of @list
 */
static inline void bplus_for_each(const struct list_node *restrict list, void (*func)(const void *restrict, void *restrict)) {
  for (const struct list_node *node = list; node != NULL; node = node->next)
    for (size_t idx = 0; idx < node->nmemb; ++idx)
      func(node->keys[idx], node->values[idx]);
}

#endif /* _INDEX_BPLUSTREE_H */
