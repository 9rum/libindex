/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2021-2022 9rum
 *
 * rbtree.h - generic red-black tree declaration
 *
 * The red-black tree is a kind of self-balancing binary search tree.
 * Each node stores an extra bit representing "color" ("red" or "black"),
 * used to ensure that the tree remains balanced during insertions and deletions.
 *
 * When the tree is modified, the new tree is rearranged and recolored
 * to restore the coloring properties that constrain how unbalanced
 * the tree can become in the worst case.
 *
 * The rebalancing is not perfect, but guarantees searching in logarithmic time,
 * where n is the number of nodes in the tree.
 * The insertion and deletion, along with the tree rearranging and recoloring,
 * are also performed in logarithmic time.
 *
 * See https://docs.lib.purdue.edu/cgi/viewcontent.cgi?article=1457&context=cstech for more details.
 */
#ifndef _INDEX_RBTREE_H
#define _INDEX_RBTREE_H

#include <stdbool.h>
#include <stddef.h>

/**
 * struct rb_node - a node in red-black tree
 *
 * @key:   the key of the node
 * @value: the value of the node
 * @left:  the pointer to the left subtree
 * @right: the pointer to the right subtree
 * @color: the color of the node
 *
 * In addition to the requirements imposed on a binary search tree,
 * the following must be satisfied by a red–black tree:
 *
 *  1. Each node is either red or black
 *  2. The root node is black
 *  3. All NIL leaves are black
 *  4. Both children of every red node are black
 *  5. Every simple path from a given node to any of its descendant NIL leaves
 *     goes through the same number of black nodes
 */
struct rb_node {
  const void           *key;
        void           *value;
        struct rb_node *left;
        struct rb_node *right;
        bool           color;
} __attribute__((aligned(__SIZEOF_POINTER__)));

struct rb_root {
  struct rb_node *root;
  bool          (*less)(const void *restrict, const void *restrict);
  size_t         size;
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
 * rb_init - initializes an empty tree with @less
 *
 * @less: operator defining the (partial) node order
 */
static inline struct rb_root rb_init(bool (*less)(const void *restrict, const void *restrict)) {
  struct rb_root tree = {
    .root = NULL,
    .less = less,
    .size = 0,
  };
  return tree;
}

/**
 * rb_find - finds element from @tree with @key
 *
 * @tree: tree to find element from
 * @key:  the key to search for
 */
static inline void *rb_find(const struct rb_root tree, const void *restrict key) {
  register const struct rb_node *walk = tree.root;

  while (walk != NULL) {
    if (tree.less(key, walk->key))      walk = walk->left;
    else if (tree.less(walk->key, key)) walk = walk->right;
    else                                return walk->value;
  }

  return NULL;
}

/**
 * rb_insert - inserts @key and @value into @tree
 *
 * @tree:  tree to insert @key and @value into
 * @key:   the key to insert
 * @value: the value to insert
 */
extern struct rb_node *rb_insert(struct rb_root *restrict tree, const void *restrict key, void *restrict value);

/**
 * rb_insert_or_assign - inserts @key and @value into @tree or assigns @value if @key already exists
 *
 * @tree:  tree to insert @key and @value into
 * @key:   the key to insert if not found
 * @value: the value to insert or assign
 */
extern struct rb_node *rb_insert_or_assign(struct rb_root *restrict tree, const void *restrict key, void *restrict value);

/**
 * rb_erase - erases @key from @tree
 *
 * @tree: tree to erase @key from
 * @key:  the key to erase
 */
extern void *rb_erase(struct rb_root *restrict tree, const void *restrict key);

/**
 * rb_clear - clears @tree
 *
 * @tree: tree to clear
 */
extern void rb_clear(struct rb_root *restrict tree);

/**
 * rb_preorder - applies @func to each node of @tree preorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
extern void rb_preorder(const struct rb_root tree, void (*func)(const void *restrict, void *restrict));

/**
 * rb_inorder - applies @func to each node of @tree inorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
extern void rb_inorder(const struct rb_root tree, void (*func)(const void *restrict, void *restrict));

/**
 * rb_postorder - applies @func to each node of @tree postorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
extern void rb_postorder(const struct rb_root tree, void (*func)(const void *restrict, void *restrict));

#endif /* _INDEX_RBTREE_H */
