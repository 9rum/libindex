/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2021-2022 9rum
 *
 * llrbtree.h - generic left-leaning red-black tree declaration
 *
 * A left-leaning red–black tree is a type of self-balancing binary search tree.
 * It is a variant of the red–black tree and guarantees the same asymptotic complexity for operations,
 * but is designed to be easier to implement.
 *
 * See https://www.cs.princeton.edu/~rs/talks/LLRB/LLRB.pdf for more details.
 */
#ifndef _INDEX_LLRBTREE_H
#define _INDEX_LLRBTREE_H

#include <stdbool.h>
#include <stddef.h>

/**
 * struct llrb_node - a node in left-leaning red-black tree
 *
 * @key:   the key of the element
 * @value: the value of the element
 * @left:  the address of the left subtree
 * @right: the address of the right subtree
 * @color: the color of the node
 *
 * The traditional red-black tree represents 2-3-4 tree as a binary search tree
 * and uses internal red edges for 3-nodes and 4-nodes.
 * In addition to the properties of the traditional red-black tree,
 * left-leaning red-black tree requires that 3-nodes be left-leaning.
 */
struct llrb_node {
  const void             *key;
        void             *value;
        struct llrb_node *left;
        struct llrb_node *right;
        bool             color;
} __attribute__((aligned(__SIZEOF_POINTER__)));

struct llrb_root {
  struct llrb_node *root;
  bool            (*less)(const void *restrict, const void *restrict);
  size_t           size;
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
 * llrb_init - initializes an empty tree with @less
 *
 * @less: operator defining the (partial) element order
 */
static inline struct llrb_root llrb_init(bool (*less)(const void *restrict, const void *restrict)) {
  struct llrb_root tree = {
    .root = NULL,
    .less = less,
    .size = 0,
  };
  return tree;
}

/**
 * llrb_size - returns the number of elements in @tree
 *
 * @tree: tree to get the number of elements
 */
static inline size_t llrb_size(const struct llrb_root tree) { return tree.size; }

/**
 * llrb_empty - checks whether @tree is empty
 *
 * @tree: tree to check
 */
static inline bool llrb_empty(const struct llrb_root tree) { return llrb_size(tree) == 0; }

/**
 * llrb_find - finds element from @tree with @key
 *
 * @tree: tree to find element from
 * @key:  the key to search for
 */
static inline void *llrb_find(const struct llrb_root tree, const void *restrict key) {
  register const struct llrb_node *walk = tree.root;

  while (walk != NULL) {
    if (tree.less(key, walk->key))      walk = walk->left;
    else if (tree.less(walk->key, key)) walk = walk->right;
    else                                return walk->value;
  }

  return NULL;
}

/**
 * llrb_contains - checks if @tree contains element with @key
 *
 * @tree: tree to check
 * @key:  the key to search for
 */
static inline bool llrb_contains(const struct llrb_root tree, const void *restrict key) {
  register const struct llrb_node *walk = tree.root;

  while (walk != NULL) {
    if (tree.less(key, walk->key))      walk = walk->left;
    else if (tree.less(walk->key, key)) walk = walk->right;
    else                                return true;
  }

  return false;
}

/**
 * llrb_insert - inserts an element into @tree
 *
 * @tree:  tree to insert element into
 * @key:   the key of the element to insert
 * @value: the value of the element to insert
 */
extern struct llrb_node *llrb_insert(struct llrb_root *restrict tree, const void *restrict key, void *restrict value);

/**
 * llrb_insert_or_assign - inserts an element or assigns @value if @key already exists
 *
 * @tree:  tree to insert element into
 * @key:   the key of the element to insert if not found
 * @value: the value of the element to insert or assign
 */
extern struct llrb_node *llrb_insert_or_assign(struct llrb_root *restrict tree, const void *restrict key, void *restrict value);

/**
 * llrb_erase - removes the element with @key from @tree
 *
 * @tree: tree to remove the element from
 * @key:  the key of the element to remove
 */
extern void *llrb_erase(struct llrb_root *restrict tree, const void *restrict key);

/**
 * llrb_clear - erases all elements from @tree
 *
 * @tree: tree to erase all elements from
 */
extern void llrb_clear(struct llrb_root *restrict tree);

/**
 * llrb_preorder - applies @func to each element of @tree preorderwise
 *
 * @tree: tree to apply @func to each element of
 * @func: function to apply to each element of @tree
 */
extern void llrb_preorder(const struct llrb_root tree, void (*func)(const void *restrict, void *restrict));

/**
 * llrb_inorder - applies @func to each element of @tree inorderwise
 *
 * @tree: tree to apply @func to each element of
 * @func: function to apply to each element of @tree
 */
extern void llrb_inorder(const struct llrb_root tree, void (*func)(const void *restrict, void *restrict));

/**
 * llrb_postorder - applies @func to each element of @tree postorderwise
 *
 * @tree: tree to apply @func to each element of
 * @func: function to apply to each element of @tree
 */
extern void llrb_postorder(const struct llrb_root tree, void (*func)(const void *restrict, void *restrict));

#endif /* _INDEX_LLRBTREE_H */
