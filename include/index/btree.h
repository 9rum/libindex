/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2020-2022 9rum
 *
 * btree.h - generic B-tree declaration
 *
 * A B-tree is a self-balancing tree data structure that maintains sorted data and allows searches,
 * sequential access, insertions, and deletions in logarithmic time.
 * The B-tree generalizes the binary search tree, allowing for nodes with more than two children.
 * Unlike other self-balancing binary search trees, the B-tree is well suited for storage systems
 * that read and write relatively large blocks of data, such as disks.
 * It is commonly used in databases and file systems.
 *
 * B-trees were invented by Rudolf Bayer and Edward M. McCreight while working at Boeing Research Labs,
 * for the purpose of efficiently managing index pages for large random access files.
 * The basic assumption was that indexes would be so voluminous
 * that only small chunks of the tree could fit in main memory.
 * Bayer and McCreight's paper, Organization and maintenance of large ordered indices,
 * was first circulated in July 1970 and later published in Acta Informatica.
 *
 * See https://infolab.usc.edu/csci585/Spring2010/den_ar/indexing.pdf for more details.
 */
#ifndef _INDEX_BTREE_H
#define _INDEX_BTREE_H

#include <stdbool.h>
#include <stddef.h>

/**
 * struct btree_node - a node in B-tree
 *
 * @keys:     the ordered set of keys of the node
 * @values:   the ordered set of values of the node
 * @parent:   the address of the parent node
 * @children: the ordered set of children of the node
 * @tree:     the address of the tree to which the node belongs
 * @index:    the index to the parent node
 * @nmemb:    the number of the keys in the node
 *
 * According to Knuth's definition, a B-tree of order m is a tree which satisfies the following properties:
 *
 *  1. Every node has at most m children
 *  2. Every non-leaf node (except root) has at least ⌈m/2⌉ children
 *  3. The root has at least two children if it is not a leaf node
 *  4. A non-leaf node with k children contains k−1 keys
 *  5. All leaves appear in the same level and carry information
 */
struct btree_node {
  const void              **keys;
        void              **values;
        struct btree_node *parent;
        struct btree_node **children;
        struct btree_root *tree;
        size_t            index;
        size_t            nmemb;
} __attribute__((aligned(__SIZEOF_POINTER__)));

struct btree_root {
        struct btree_node *root;
        bool             (*less)(const void *restrict, const void *restrict);
        size_t            size;
  const size_t            order;
} __attribute__((aligned(__SIZEOF_POINTER__)));

struct btree_iter {
  const void              *key;
        void              *value;
        struct btree_node *pivot;
        size_t            index;
} __attribute__((aligned(__SIZEOF_POINTER__)));

struct btree_reverse_iter {
  const void              *key;
        void              *value;
        struct btree_node *pivot;
        size_t            index;
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
 * btree_init - initializes an empty tree of @order with @less
 *
 * @order: the order of tree
 * @less:  operator defining the (partial) node order
 */
static inline struct btree_root btree_init(const size_t order, bool (*less)(const void *restrict, const void *restrict)) {
  struct btree_root tree = {
    .root  = NULL,
    .less  = less,
    .size  = 0,
    .order = order,
  };
  return tree;
}

/**
 * btree_size - returns the number of entries in @tree
 *
 * @tree: tree to get the number of entries
 */
static inline size_t btree_size(const struct btree_root tree) { return tree.size; }

/**
 * btree_empty - checks whether @tree is empty
 *
 * @tree: tree to check
 */
static inline bool btree_empty(const struct btree_root tree) { return tree.root == NULL; }

/**
 * btree_contains - checks if @tree contains an entry with @key
 *
 * @tree: tree to check
 * @key:  the key to search for
 */
extern bool btree_contains(const struct btree_root tree, const void *key);

/**
 * btree_find - searches @tree for an entry with @key
 *
 * @tree: tree to search
 * @key:  the key to search for
 */
extern struct btree_iter btree_find(const struct btree_root tree, const void *key);

/**
 * btree_insert - inserts an entry into @tree
 *
 * @tree:  tree to insert an entry into
 * @key:   the key of the entry to insert
 * @value: the value of the entry to insert
 */
extern struct btree_iter btree_insert(struct btree_root *restrict tree, const void *restrict key, void *restrict value);

/**
 * btree_replace - inserts an entry or assigns @value if @key already exists
 *
 * @tree:  tree to insert an entry into
 * @key:   the key of the entry to insert if not found
 * @value: the value of the entry to insert or assign
 */
extern struct btree_iter btree_replace(struct btree_root *restrict tree, const void *restrict key, void *restrict value);

/**
 * btree_erase - removes the entry with @key from @tree
 *
 * @tree: tree to remove the entry from
 * @key:  the key of the entry to remove
 */
extern void *btree_erase(struct btree_root *restrict tree, const void *restrict key);

/**
 * btree_clear - erases all entries from @tree
 *
 * @tree: tree to erase all entries from
 */
extern void btree_clear(struct btree_root *tree);

/**
 * btree_iter_init - initializes an iterator of @tree
 *
 * @tree: tree to initialize an iterator of
 */
extern struct btree_iter btree_iter_init(const struct btree_root tree);

/**
 * btree_iter_prev - finds logical previous entry of @iter
 *
 * @iter: iterator to find logical previous entry of
 */
extern void btree_iter_prev(struct btree_iter *iter);

/**
 * btree_iter_next - finds logical next entry of @iter
 *
 * @iter: iterator to find logical next entry of
 */
extern void btree_iter_next(struct btree_iter *iter);

/**
 * btree_iter_end - checks if @iter reaches the end
 *
 * @iter: iterator to check
 */
static inline bool btree_iter_end(const struct btree_iter iter) { return iter.pivot == NULL; }

/**
 * btree_reverse_iter_init - initializes a reverse iterator of @tree
 *
 * @tree: tree to initialize a reverse iterator of
 */
extern struct btree_reverse_iter btree_reverse_iter_init(const struct btree_root tree);

/**
 * btree_reverse_iter_prev - finds logical previous entry of @iter
 *
 * @iter: reverse iterator to find logical previous entry of
 */
extern void btree_reverse_iter_prev(struct btree_reverse_iter *iter);

/**
 * btree_reverse_iter_next - finds logical next entry of @iter
 *
 * @iter: reverse iterator to find logical next entry of
 */
extern void btree_reverse_iter_next(struct btree_reverse_iter *iter);

/**
 * btree_reverse_iter_end - checks if @iter reaches the end
 *
 * @iter: reverse iterator to check
 */
static inline bool btree_reverse_iter_end(const struct btree_reverse_iter iter) { return iter.pivot == NULL; }

#endif /* _INDEX_BTREE_H */
