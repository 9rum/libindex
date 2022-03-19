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
 * See https://sedgewick.io/wp-content/themes/sedgewick/papers/2008LLRB.pdf for more details.
 */
#ifndef _INDEX_LLRBTREE_H
#define _INDEX_LLRBTREE_H

#include <stdbool.h>
#include <stddef.h>

/**
 * struct llrb_node - a node in left-leaning red-black tree
 *
 * @key:    the key of the node
 * @value:  the value of the node
 * @parent: the address of the parent node
 * @left:   the address of the left subtree
 * @right:  the address of the right subtree
 * @tree:   the address of the tree to which the node belongs
 * @black:  the color of the node
 *
 * The traditional red-black tree represents 2-3-4 tree as a binary search tree
 * and uses internal red edges for 3-nodes and 4-nodes.
 * In addition to the properties of the traditional red-black tree,
 * left-leaning red-black tree requires that 3-nodes be left-leaning.
 */
struct llrb_node {
  const void             *key;
        void             *value;
        struct llrb_node *parent;
        struct llrb_node *left;
        struct llrb_node *right;
        struct llrb_root *tree;
        bool             black;
} __attribute__((aligned(__SIZEOF_POINTER__)));

struct llrb_root {
  struct llrb_node *root;
  bool            (*less)(const void *restrict, const void *restrict);
  size_t           size;
} __attribute__((aligned(__SIZEOF_POINTER__)));

struct llrb_iter {
  const void             *key;
        void             *value;
        struct llrb_node *node;
} __attribute__((aligned(__SIZEOF_POINTER__)));

struct llrb_reverse_iter {
  const void             *key;
        void             *value;
        struct llrb_node *node;
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
 * @less: operator defining the (partial) node order
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
 * llrb_size - returns the number of entries in @tree
 *
 * @tree: tree to get the number of entries
 */
static inline size_t llrb_size(const struct llrb_root tree) { return tree.size; }

/**
 * llrb_empty - checks whether @tree is empty
 *
 * @tree: tree to check
 */
static inline bool llrb_empty(const struct llrb_root tree) { return tree.root == NULL; }

/**
 * llrb_contains - checks if @tree contains an entry with @key
 *
 * @tree: tree to check
 * @key:  the key to search for
 */
static inline bool llrb_contains(const struct llrb_root tree, const void *key) {
  register struct llrb_node *walk = tree.root;

  while (walk != NULL) {
    if (tree.less(key, walk->key))      walk = walk->left;
    else if (tree.less(walk->key, key)) walk = walk->right;
    else                                return true;
  }

  return false;
}

/**
 * llrb_find - searches @tree for an entry with @key
 *
 * @tree: tree to search
 * @key:  the key to search for
 */
extern struct llrb_iter llrb_find(const struct llrb_root tree, const void *key);

/**
 * llrb_insert - inserts an entry into @tree
 *
 * @tree:  tree to insert an entry into
 * @key:   the key of the entry to insert
 * @value: the value of the entry to insert
 */
extern struct llrb_iter llrb_insert(struct llrb_root *restrict tree, const void *restrict key, void *restrict value);

/**
 * llrb_replace - inserts an entry or assigns @value if @key already exists
 *
 * @tree:  tree to insert an entry into
 * @key:   the key of the entry to insert if not found
 * @value: the value of the entry to insert or assign
 */
extern struct llrb_iter llrb_replace(struct llrb_root *restrict tree, const void *restrict key, void *restrict value);

/**
 * llrb_erase - removes the entry with @key from @tree
 *
 * @tree: tree to remove the entry from
 * @key:  the key of the entry to remove
 */
extern void *llrb_erase(struct llrb_root *restrict tree, const void *restrict key);

/**
 * llrb_clear - erases all entries from @tree
 *
 * @tree: tree to erase all entries from
 */
extern void llrb_clear(struct llrb_root *tree);

/**
 * llrb_iter_init - initializes an iterator of @tree
 *
 * @tree: tree to initialize an iterator of
 */
extern struct llrb_iter llrb_iter_init(const struct llrb_root tree);

/**
 * llrb_iter_prev - finds logical previous entry of @iter
 *
 * @iter: iterator to find logical previous entry of
 */
extern void llrb_iter_prev(struct llrb_iter *iter);

/**
 * llrb_iter_next - finds logical next entry of @iter
 *
 * @iter: iterator to find logical next entry of
 */
extern void llrb_iter_next(struct llrb_iter *iter);

/**
 * llrb_iter_end - checks if @iter reaches the end
 *
 * @iter: iterator to check
 */
static inline bool llrb_iter_end(const struct llrb_iter iter) { return iter.node == NULL; }

/**
 * llrb_reverse_iter_init - initializes a reverse iterator of @tree
 *
 * @tree: tree to initialize a reverse iterator of
 */
extern struct llrb_reverse_iter llrb_reverse_iter_init(const struct llrb_root tree);

/**
 * llrb_reverse_iter_prev - finds logical previous entry of @iter
 *
 * @iter: reverse iterator to find logical previous entry of
 */
extern void llrb_reverse_iter_prev(struct llrb_reverse_iter *iter);

/**
 * llrb_reverse_iter_next - finds logical next entry of @iter
 *
 * @iter: reverse iterator to find logical next entry of
 */
extern void llrb_reverse_iter_next(struct llrb_reverse_iter *iter);

/**
 * llrb_reverse_iter_end - checks if @iter reaches the end
 *
 * @iter: reverse iterator to check
 */
static inline bool llrb_reverse_iter_end(const struct llrb_reverse_iter iter) { return iter.node == NULL; }

#endif /* _INDEX_LLRBTREE_H */
