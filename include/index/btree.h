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
 * @children: the ordered set of children of the node
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
        struct btree_node **children;
        size_t            nmemb;
} __attribute__((aligned(__SIZEOF_POINTER__)));

struct btree_root {
        struct btree_node *root;
        bool             (*less)(const void *restrict, const void *restrict);
        size_t            size;
  const size_t            order;
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
 * @less:  operator defining the (partial) element order
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
 * btree_size - returns the number of elements in @tree
 *
 * @tree: tree to get the number of elements
 */
static inline size_t btree_size(const struct btree_root tree) { return tree.size; }

/**
 * btree_empty - checks whether @tree is empty
 *
 * @tree: tree to check
 */
static inline bool btree_empty(const struct btree_root tree) { return btree_size(tree) == 0; }

/**
 * btree_find - finds element from @tree with @key
 *
 * @tree: tree to find element from
 * @key:  the key to search for
 */
extern void *btree_find(const struct btree_root tree, const void *restrict key);

/**
 * btree_contains - checks if @tree contains element with @key
 *
 * @tree: tree to check
 * @key:  the key to search for
 */
extern bool btree_contains(const struct btree_root tree, const void *restrict key);

/**
 * btree_insert - inserts an element into @tree
 *
 * @tree:  tree to insert element into
 * @key:   the key of the element to insert
 * @value: the value of the element to insert
 */
extern struct btree_node *btree_insert(struct btree_root *restrict tree, const void *restrict key, void *restrict value);

/**
 * btree_insert_or_assign - inserts an element or assigns @value if @key already exists
 *
 * @tree:  tree to insert element into
 * @key:   the key of the element to insert if not found
 * @value: the value of the element to insert or assign
 */
extern struct btree_node *btree_insert_or_assign(struct btree_root *restrict tree, const void *restrict key, void *restrict value);

/**
 * btree_erase - removes the element with @key from @tree
 *
 * @tree: tree to remove the element from
 * @key:  the key of the element to remove
 */
extern void *btree_erase(struct btree_root *restrict tree, const void *restrict key);

/**
 * btree_clear - erases all elements from @tree
 *
 * @tree: tree to erase all elements from
 */
extern void btree_clear(struct btree_root *restrict tree);

/**
 * btree_preorder - applies @func to each element of @tree preorderwise
 *
 * @tree: tree to apply @func to each element of
 * @func: function to apply to each element of @tree
 */
extern void btree_preorder(const struct btree_root tree, void (*func)(const void *restrict, void *restrict));

/**
 * btree_inorder - applies @func to each element of @tree inorderwise
 *
 * @tree: tree to apply @func to each element of
 * @func: function to apply to each element of @tree
 */
extern void btree_inorder(const struct btree_root tree, void (*func)(const void *restrict, void *restrict));

/**
 * btree_postorder - applies @func to each element of @tree postorderwise
 *
 * @tree: tree to apply @func to each element of
 * @func: function to apply to each element of @tree
 */
extern void btree_postorder(const struct btree_root tree, void (*func)(const void *restrict, void *restrict));

#endif /* _INDEX_BTREE_H */
