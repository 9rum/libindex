/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2020-2022 9rum
 *
 * avltree.h - generic AVL tree declaration
 *
 * The AVL tree (named after inventors Adelson-Velsky and Landis)
 * is a self-balancing binary search tree.
 *
 * In an AVL tree, the heights of the two child subtrees of any node
 * differ by at most one; if at any time they differ by more than one,
 * rebalancing is done to restore this property.
 *
 * Lookup, insertion, and deletion all take logarithmic time in both
 * the average and worst cases, where n is the number of nodes
 * in the tree prior to the operation.
 *
 * See https://zhjwpku.com/assets/pdf/AED2-10-avl-paper.pdf for more details.
 */
#ifndef _INDEX_AVLTREE_H
#define _INDEX_AVLTREE_H

#include <stdbool.h>
#include <stddef.h>

/**
 * struct avl_node - a node in AVL tree
 *
 * @key:    the key of the node
 * @value:  the value of the node
 * @left:   the pointer to the left subtree
 * @right:  the pointer to the right subtree
 * @height: the height of the subtree rooted with the node
 *
 * In a binary tree, the balance factor of a node X is defined
 * to be the height difference
 *
 *    BF(X) := Height(LeftSubtree(X)) - Height(RightSubtree(X))
 *
 * of its two child subtrees.
 * A binary tree is defined to be an AVL tree if the invariant
 *
 *    BF(X) ∈ {-1, 0, 1}
 *
 * holds for every node X in the tree.
 */
struct avl_node {
  const void            *key;
        void            *value;
        struct avl_node *left;
        struct avl_node *right;
        size_t          height;
} __attribute__((aligned(__SIZEOF_POINTER__)));

struct avl_root {
  struct avl_node *root;
  bool           (*less)(const void *restrict, const void *restrict);
  size_t          size;
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
 * avl_init - initializes an empty tree with @less
 *
 * @less: operator defining the (partial) element order
 */
static inline struct avl_root avl_init(bool (*less)(const void *restrict, const void *restrict)) {
  struct avl_root tree = {
    .root = NULL,
    .less = less,
    .size = 0,
  };
  return tree;
}

/**
 * avl_size - returns the number of elements in @tree
 *
 * @tree: tree to get the number of elements
 */
static inline size_t avl_size(const struct avl_root tree) { return tree.size; }

/**
 * avl_empty - checks whether @tree is empty
 *
 * @tree: tree to check
 */
static inline bool avl_empty(const struct avl_root tree) { return avl_size(tree) == 0; }

/**
 * avl_find - finds element from @tree with @key
 *
 * @tree: tree to find element from
 * @key:  the key to search for
 */
static inline void *avl_find(const struct avl_root tree, const void *restrict key) {
  register const struct avl_node *walk = tree.root;

  while (walk != NULL) {
    if (tree.less(key, walk->key))      walk = walk->left;
    else if (tree.less(walk->key, key)) walk = walk->right;
    else                                return walk->value;
  }

  return NULL;
}

/**
 * avl_contains - checks if @tree contains element with @key
 *
 * @tree: tree to check
 * @key:  the key to search for
 */
static inline bool avl_contains(const struct avl_root tree, const void *restrict key) {
  register const struct avl_node *walk = tree.root;

  while (walk != NULL) {
    if (tree.less(key, walk->key))      walk = walk->left;
    else if (tree.less(walk->key, key)) walk = walk->right;
    else                                return true;
  }

  return false;
}

/**
 * avl_insert - inserts an element into @tree
 *
 * @tree:  tree to insert element into
 * @key:   the key of the element to insert
 * @value: the value of the element to insert
 */
extern struct avl_node *avl_insert(struct avl_root *restrict tree, const void *restrict key, void *restrict value);

/**
 * avl_insert_or_assign - inserts an element or assigns @value if @key already exists
 *
 * @tree:  tree to insert element into
 * @key:   the key of the element to insert if not found
 * @value: the value of the element to insert or assign
 */
extern struct avl_node *avl_insert_or_assign(struct avl_root *restrict tree, const void *restrict key, void *restrict value);

/**
 * avl_erase - removes the element with @key from @tree
 *
 * @tree: tree to remove the element from
 * @key:  the key of the element to remove
 */
extern void *avl_erase(struct avl_root *restrict tree, const void *restrict key);

/**
 * avl_clear - erases all elements from @tree
 *
 * @tree: tree to erase all elements
 */
extern void avl_clear(struct avl_root *restrict tree);

/**
 * avl_preorder - applies @func to each element of @tree preorderwise
 *
 * @tree: tree to apply @func to each element of
 * @func: function to apply to each element of @tree
 */
extern void avl_preorder(const struct avl_root tree, void (*func)(const void *restrict, void *restrict));

/**
 * avl_inorder - applies @func to each element of @tree inorderwise
 *
 * @tree: tree to apply @func to each element of
 * @func: function to apply to each element of @tree
 */
extern void avl_inorder(const struct avl_root tree, void (*func)(const void *restrict, void *restrict));

/**
 * avl_postorder - applies @func to each element of @tree postorderwise
 *
 * @tree: tree to apply @func to each element of
 * @func: function to apply to each element of @tree
 */
extern void avl_postorder(const struct avl_root tree, void (*func)(const void *restrict, void *restrict));

#endif /* _INDEX_AVLTREE_H */
