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
 * @key:    the key of the node
 * @value:  the value of the node
 * @parent: the address of the parent node
 * @left:   the address of the left subtree
 * @right:  the address of the right subtree
 * @tree:   the address of the tree to which the node belongs
 * @black:  the color of the node
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
        struct rb_node *parent;
        struct rb_node *left;
        struct rb_node *right;
        struct rb_root *tree;
        bool           black;
} __attribute__((aligned(__SIZEOF_POINTER__)));

struct rb_root {
  struct rb_node *root;
  bool          (*less)(const void *restrict, const void *restrict);
  size_t         size;
} __attribute__((aligned(__SIZEOF_POINTER__)));

struct rb_iter {
  const void           *key;
        void           *value;
        struct rb_node *pivot;
} __attribute__((aligned(__SIZEOF_POINTER__)));

struct rb_reverse_iter {
  const void           *key;
        void           *value;
        struct rb_node *pivot;
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
 * rb_size - returns the number of entries in @tree
 *
 * @tree: tree to get the number of entries
 */
static inline size_t rb_size(const struct rb_root tree) { return tree.size; }

/**
 * rb_empty - checks whether @tree is empty
 *
 * @tree: tree to check
 */
static inline bool rb_empty(const struct rb_root tree) { return tree.root == NULL; }

/**
 * rb_contains - checks if @tree contains an entry with @key
 *
 * @tree: tree to check
 * @key:  the key to search for
 */
static inline bool rb_contains(const struct rb_root tree, const void *key) {
  register struct rb_node *pivot = tree.root;

  while (pivot != NULL) {
    if (tree.less(key, pivot->key))      pivot = pivot->left;
    else if (tree.less(pivot->key, key)) pivot = pivot->right;
    else                                 return true;
  }

  return false;
}

/**
 * rb_find - searches @tree for an entry with @key
 *
 * @tree: tree to search
 * @key:  the key to search for
 */
extern struct rb_iter rb_find(const struct rb_root tree, const void *key);

/**
 * rb_insert - inserts an entry into @tree
 *
 * @tree:  tree to insert an entry into
 * @key:   the key of the entry to insert
 * @value: the value of the entry to insert
 */
extern struct rb_iter rb_insert(struct rb_root *restrict tree, const void *restrict key, void *restrict value);

/**
 * rb_replace - inserts an entry or assigns @value if @key already exists
 *
 * @tree:  tree to insert an entry into
 * @key:   the key of the entry to insert if not found
 * @value: the value of the entry to insert or assign
 */
static inline struct rb_iter rb_replace(struct rb_root *restrict tree, const void *restrict key, void *restrict value) {
  struct rb_iter iter = rb_insert(tree, key, value);
  iter.pivot->value   = value;
  iter.value          = value;
  return iter;
}

/**
 * rb_erase - removes the entry with @key from @tree
 *
 * @tree: tree to remove the entry from
 * @key:  the key of the entry to remove
 */
extern void *rb_erase(struct rb_root *restrict tree, const void *restrict key);

/**
 * rb_clear - erases all entries from @tree
 *
 * @tree: tree to erase all entries from
 */
extern void rb_clear(struct rb_root *tree);

/**
 * rb_iter_init - initializes an iterator of @tree
 *
 * @tree: tree to initialize an iterator of
 */
extern struct rb_iter rb_iter_init(const struct rb_root tree);

/**
 * rb_iter_prev - finds logical previous entry of @iter
 *
 * @iter: iterator to find logical previous entry of
 */
extern void rb_iter_prev(struct rb_iter *iter);

/**
 * rb_iter_next - finds logical next entry of @iter
 *
 * @iter: iterator to find logical next entry of
 */
extern void rb_iter_next(struct rb_iter *iter);

/**
 * rb_iter_end - checks if @iter reaches the end
 *
 * @iter: iterator to check
 */
static inline bool rb_iter_end(const struct rb_iter iter) { return iter.pivot == NULL; }

/**
 * rb_reverse_iter_init - initializes a reverse iterator of @tree
 *
 * @tree: tree to initialize a reverse iterator of
 */
extern struct rb_reverse_iter rb_reverse_iter_init(const struct rb_root tree);

/**
 * rb_reverse_iter_prev - finds logical previous entry of @iter
 *
 * @iter: reverse iterator to find logical previous entry of
 */
extern void rb_reverse_iter_prev(struct rb_reverse_iter *iter);

/**
 * rb_reverse_iter_next - finds logical next entry of @iter
 *
 * @iter: reverse iterator to find logical next entry of
 */
extern void rb_reverse_iter_next(struct rb_reverse_iter *iter);

/**
 * rb_reverse_iter_end - checks if @iter reaches the end
 *
 * @iter: reverse iterator to check
 */
static inline bool rb_reverse_iter_end(const struct rb_reverse_iter iter) { return iter.pivot == NULL; }

#endif /* _INDEX_RBTREE_H */
