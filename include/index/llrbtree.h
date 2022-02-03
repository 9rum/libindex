/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright 2021-2022 9rum
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
 * @key:   the key of the node
 * @value: the value of the node
 * @left:  the pointer to the left subtree
 * @right: the pointer to the right subtree
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
 * llrb_insert - inserts @key and @value into @tree
 *
 * @tree:  tree to insert @key and @value into
 * @key:   the key to insert
 * @value: the value to insert
 * @less:  operator defining the (partial) node order
 */
extern void llrb_insert(struct llrb_node **restrict tree, const void *restrict key, void *restrict value, bool (*less)(const void *restrict, const void *restrict));

/**
 * llrb_erase - erases @key from @tree
 *
 * @tree: tree to erase @key from
 * @key:  the key to erase
 * @less: operator defining the (partial) node order
 */
extern void llrb_erase(struct llrb_node **restrict tree, const void *restrict key, bool (*less)(const void *restrict, const void *restrict));

/**
 * llrb_preorder - applies @func to each node of @tree preorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
static inline void llrb_preorder(const struct llrb_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { func(tree->key, tree->value); llrb_preorder(tree->left, func); llrb_preorder(tree->right, func); } }

/**
 * llrb_inorder - applies @func to each node of @tree inorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
static inline void llrb_inorder(const struct llrb_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { llrb_inorder(tree->left, func); func(tree->key, tree->value); llrb_inorder(tree->right, func); } }

/**
 * llrb_postorder - applies @func to each node of @tree postorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
static inline void llrb_postorder(const struct llrb_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { llrb_postorder(tree->left, func); llrb_postorder(tree->right, func); func(tree->key, tree->value); } }

#endif /* _INDEX_LLRBTREE_H */
