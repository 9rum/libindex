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
 * btree_insert - inserts @key and @value into @tree
 *
 * @tree:  tree to insert @key and @value into
 * @order: order of B-tree
 * @key:   the key to insert
 * @value: the value to insert
 * @less:  operator defining the (partial) node order
 */
extern struct btree_node *btree_insert(struct btree_node **restrict tree, const size_t order, const void *restrict key, void *restrict value, bool (*less)(const void *restrict, const void *restrict));

/**
 * btree_erase - erases @key from @tree
 *
 * @tree:  tree to erase @key from
 * @order: order of B-tree
 * @key:   the key to erase
 * @less:  operator defining the (partial) node order
 */
extern void *btree_erase(struct btree_node **restrict tree, const size_t order, const void *restrict key, bool (*less)(const void *restrict, const void *restrict));

/**
 * btree_clear - empties @tree
 *
 * @tree: tree to empty
 */
extern void btree_clear(struct btree_node **restrict tree);

/**
 * btree_preorder - applies @func to each node of @tree preorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
static inline void btree_preorder(const struct btree_node *restrict tree, void (*func)(const void *restrict, void *restrict)) {
  if (tree != NULL) {
    for (size_t idx = 0; idx < tree->nmemb; ++idx) {
      func(tree->keys[idx], tree->values[idx]);
      btree_preorder(tree->children[idx], func);
    }
    btree_preorder(tree->children[tree->nmemb], func);
  }
}

/**
 * btree_inorder - applies @func to each node of @tree inorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
static inline void btree_inorder(const struct btree_node *restrict tree, void (*func)(const void *restrict, void *restrict)) {
  if (tree != NULL) {
    for (size_t idx = 0; idx < tree->nmemb; ++idx) {
      btree_inorder(tree->children[idx], func);
      func(tree->keys[idx], tree->values[idx]);
    }
    btree_inorder(tree->children[tree->nmemb], func);
  }
}

/**
 * btree_postorder - applies @func to each node of @tree postorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
static inline void btree_postorder(const struct btree_node *restrict tree, void (*func)(const void *restrict, void *restrict)) {
  if (tree != NULL) {
    btree_postorder(tree->children[0], func);
    for (size_t idx = 0; idx < tree->nmemb; ++idx) {
      btree_postorder(tree->children[idx+1], func);
      func(tree->keys[idx], tree->values[idx]);
    }
  }
}

#endif /* _INDEX_BTREE_H */
