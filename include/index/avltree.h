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
 * avl_insert - inserts @key and @value into @tree
 *
 * @tree:  tree to insert @key and @value into
 * @key:   the key to insert
 * @value: the value to insert
 * @less:  operator defining the (partial) node order
 */
extern void avl_insert(struct avl_node **restrict tree, const void *restrict key, void *restrict value, bool (*less)(const void *restrict, const void *restrict));

/**
 * avl_erase - erases @key from @tree
 *
 * @tree: tree to erase @key from
 * @key:  the key to erase
 * @less: operator defining the (partial) node order
 */
extern void avl_erase(struct avl_node **restrict tree, const void *restrict key, bool (*less)(const void *restrict, const void *restrict));

/**
 * avl_clear - empties @tree
 *
 * @tree: tree to empty
 */
extern void avl_clear(struct avl_node **restrict tree);

/**
 * avl_preorder - applies @func to each node of @tree preorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
static inline void avl_preorder(const struct avl_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { func(tree->key, tree->value); avl_preorder(tree->left, func); avl_preorder(tree->right, func); } }

/**
 * avl_inorder - applies @func to each node of @tree inorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
static inline void avl_inorder(const struct avl_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { avl_inorder(tree->left, func); func(tree->key, tree->value); avl_inorder(tree->right, func); } }

/**
 * avl_postorder - applies @func to each node of @tree postorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
static inline void avl_postorder(const struct avl_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { avl_postorder(tree->left, func); avl_postorder(tree->right, func); func(tree->key, tree->value); } }

#endif /* _INDEX_AVLTREE_H */
