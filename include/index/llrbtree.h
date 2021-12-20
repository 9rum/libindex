/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright 2021 9rum
 *
 * llrbtree.h - generic left-leaning red-black tree definition
 *
 * A left-leaning red–black tree is a type of self-balancing binary search tree.
 * It is a variant of the red–black tree and guarantees the same asymptotic complexity for operations,
 * but is designed to be easier to implement.
 *
 * See https://www.cs.princeton.edu/~rs/talks/LLRB/LLRB.pdf for more details.
 */
#ifndef _INDEX_LLRBTREE_H
#define _INDEX_LLRBTREE_H

#include <index/stack.h>

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

/**
 * llrb_alloc - allocates a node
 */
static inline struct llrb_node *llrb_alloc(void) {
  struct llrb_node *node = malloc(sizeof(struct llrb_node));
  node->left             = NULL;
  node->right            = NULL;
  node->color            = false;
  return node;
}

static inline bool llrb_is_red(const struct llrb_node *restrict node) { return node != NULL && !node->color; }

static inline bool llrb_is_black(const struct llrb_node *restrict node) { return node == NULL || node->color; }

/**
 * llrb_rotate_left - rotates subtree rooted with @node counterclockwise
 *
 * @root:   root node of tree
 * @node:   root node of subtree
 * @parent: parent node of @node
 */
static inline struct llrb_node *llrb_rotate_left(struct llrb_node **restrict root, struct llrb_node *restrict node, struct llrb_node *restrict parent) {
  struct llrb_node *rchild = node->right;
  node->right              = rchild->left;
  rchild->left             = node;
  rchild->color            = node->color;
  node->color              = false;

  if (parent == NULL)            *root         = rchild; /* case of root */
  else if (parent->left == node) parent->left  = rchild;
  else                           parent->right = rchild;

  return rchild;
}

/**
 * llrb_rotate_right - rotates subtree rooted with @node clockwise
 *
 * @root:   root node of tree
 * @node:   root node of subtree
 * @parent: parent node of @node
 */
static inline struct llrb_node *llrb_rotate_right(struct llrb_node **restrict root, struct llrb_node *restrict node, struct llrb_node *restrict parent) {
  struct llrb_node *lchild = node->left;
  node->left               = lchild->right;
  lchild->right            = node;
  lchild->color            = node->color;
  node->color              = false;

  if (parent == NULL)            *root         = lchild; /* case of root */
  else if (parent->left == node) parent->left  = lchild;
  else                           parent->right = lchild;

  return lchild;
}

/**
 * llrb_flip - flips the colors of @node and its children
 *
 * @node: node to flip color
 */
static inline void llrb_flip(struct llrb_node *restrict node) {
  node->color        = !node->color;
  node->left->color  = !node->left->color;
  node->right->color = !node->right->color;
}

/**
 * llrb_move_red_left - carries a red link down the left spine of @node
 *
 * @root:   root node of tree
 * @node:   node to move a red link left
 * @parent: parent node of @node
 */
static inline struct llrb_node *llrb_move_red_left(struct llrb_node **restrict root, struct llrb_node *restrict node, struct llrb_node *restrict parent) {
  llrb_flip(node);
  if (llrb_is_red(node->right->left)) {
    node->right = llrb_rotate_right(root, node->right, node);
    node        = llrb_rotate_left(root, node, parent);
    llrb_flip(node);
  }
  return node;
}

/**
 * llrb_move_red_right - carries a red link down the right spine of @node
 *
 * @root:   root node of tree
 * @node:   node to move a red link right
 * @parent: parent node of @node
 */
static inline struct llrb_node *llrb_move_red_right(struct llrb_node **restrict root, struct llrb_node *restrict node, struct llrb_node *restrict parent) {
  llrb_flip(node);
  if (llrb_is_red(node->left->left)) {
    node = llrb_rotate_right(root, node, parent);
    llrb_flip(node);
  }
  return node;
}

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
extern inline void llrb_insert(struct llrb_node **restrict tree, const void *restrict key, void *restrict value, bool (*less)(const void *restrict, const void *restrict)) {
  register struct llrb_node *parent;
  register struct llrb_node *walk  = *tree;
           struct stack     *stack = NULL;

  while (walk != NULL) {
    if (!(less(key, walk->key) || less(walk->key, key))) { stack_free(stack); return; }
    stack_push(&stack, walk);
    walk = less(key, walk->key) ? walk->left : walk->right;
  }

  walk        = llrb_alloc();
  walk->key   = key;
  walk->value = value;

  if ((parent = stack_top(stack)) == NULL) *tree         = walk;
  else if (less(key, parent->key))         parent->left  = walk;
  else                                     parent->right = walk;

  while (!stack_empty(stack)) {
    walk   = stack_pop(&stack);
    parent = stack_top(stack);

    if (llrb_is_red(walk->right))                                 walk = llrb_rotate_left(tree, walk, parent);  /* case of right-leaning red */
    if (llrb_is_red(walk->left) && llrb_is_red(walk->left->left)) walk = llrb_rotate_right(tree, walk, parent); /* case of double reds */
    if (llrb_is_red(walk->left) && llrb_is_red(walk->right))      llrb_flip(walk);                              /* case of 4-node */
  }

  (*tree)->color = true;
}

/**
 * llrb_erase - erases @key from @tree
 *
 * @tree: tree to erase @key from
 * @key:  the key to erase
 * @less: operator defining the (partial) node order
 */
extern inline void llrb_erase(struct llrb_node **restrict tree, const void *restrict key, bool (*less)(const void *restrict, const void *restrict)) {
  register struct llrb_node *parent;
  register struct llrb_node *walk  = *tree;
           struct stack     *stack = NULL;

  while (walk != NULL) {
    if (less(key, walk->key)) {
      if (llrb_is_black(walk->left) && llrb_is_black(walk->left->left)) walk = llrb_move_red_left(tree, walk, stack_top(stack));
      stack_push(&stack, walk);
      walk = walk->left;
    } else {
      if (llrb_is_red(walk->left)) walk = llrb_rotate_right(tree, walk, stack_top(stack));

      if (!less(walk->key, key) && walk->right == NULL) {
        if ((parent = stack_top(stack)) == NULL) *tree         = NULL;                                          /* case of root */
        else if (parent->left == walk)           parent->left  = NULL;
        else                                     parent->right = NULL;
        free(walk);
        break;
      }

      if (llrb_is_black(walk->right) && llrb_is_black(walk->right->left)) walk = llrb_move_red_right(tree, walk, stack_top(stack));

      if (!less(walk->key, key)) {
        parent = walk;
        stack_push(&stack, walk);

        for (walk = walk->right; walk->left != NULL; walk = walk->left) {
          if (llrb_is_black(walk->left) && llrb_is_black(walk->left->left)) walk = llrb_move_red_left(tree, walk, stack_top(stack));
          stack_push(&stack, walk);
        }

        parent->key   = walk->key;
        parent->value = walk->value;

        if ((parent = stack_top(stack))->left == walk) parent->left  = NULL;
        else                                           parent->right = NULL;
        free(walk);
        break;
      } else {
        stack_push(&stack, walk);
        walk = walk->right;
      }
    }
  }

  while (!stack_empty(stack)) {
    walk   = stack_pop(&stack);
    parent = stack_top(stack);

    if (llrb_is_red(walk->right))                                 walk = llrb_rotate_left(tree, walk, parent);  /* case of right-leaning red */
    if (llrb_is_red(walk->left) && llrb_is_red(walk->left->left)) walk = llrb_rotate_right(tree, walk, parent); /* case of double reds */
    if (llrb_is_red(walk->left) && llrb_is_red(walk->right))      llrb_flip(walk);                              /* case of 4-node */
  }

  if (*tree != NULL) (*tree)->color = true;
}

/**
 * llrb_preorder - applies @func to each node of @tree preorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
extern inline void llrb_preorder(const struct llrb_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { func(tree->key, tree->value); llrb_preorder(tree->left, func); llrb_preorder(tree->right, func); } }

/**
 * llrb_inorder - applies @func to each node of @tree inorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
extern inline void llrb_inorder(const struct llrb_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { llrb_inorder(tree->left, func); func(tree->key, tree->value); llrb_inorder(tree->right, func); } }

/**
 * llrb_postorder - applies @func to each node of @tree postorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
extern inline void llrb_postorder(const struct llrb_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { llrb_postorder(tree->left, func); llrb_postorder(tree->right, func); func(tree->key, tree->value); } }

#endif /* _INDEX_LLRBTREE_H */
