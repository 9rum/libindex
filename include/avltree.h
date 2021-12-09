/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright 2020 9rum
 *
 * avltree.h - generic AVL tree definition
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
#ifndef _AVLTREE_H
#define _AVLTREE_H

#include <stack.h>

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

/**
 * avl_alloc - allocates a node
 */
static inline struct avl_node *avl_alloc(void) {
  struct avl_node *node = malloc(sizeof(struct avl_node));
  node->left            = NULL;
  node->right           = NULL;
  node->height          = 1;
  return node;
}

static inline size_t __max(const size_t a, const size_t b) { return a < b ? b : a; }

/**
 * avl_height - returns the height of @tree
 *
 * @tree: tree to get the height
 */
static inline size_t avl_height(const struct avl_node *restrict tree) { return tree == NULL ? 0 : tree->height; }

/**
 * avl_rotate_left - rotates subtree rooted with @node counterclockwise
 *
 * @root:   root node of tree
 * @node:   root node of subtree
 * @parent: parent node of @node
 */
static inline void avl_rotate_left(struct avl_node **restrict root, struct avl_node *restrict node, struct avl_node *restrict parent) {
  struct avl_node *rchild = node->right;
  node->right             = rchild->left;
  rchild->left            = node;

  if (parent == NULL)            *root         = rchild; /* case of root */
  else if (parent->left == node) parent->left  = rchild;
  else                           parent->right = rchild;
}

/**
 * avl_rotate_right - rotates subtree rooted with @node clockwise
 *
 * @root:   root node of tree
 * @node:   root node of subtree
 * @parent: parent node of @node
 */
static inline void avl_rotate_right(struct avl_node **restrict root, struct avl_node *restrict node, struct avl_node *restrict parent) {
  struct avl_node *lchild = node->left;
  node->left              = lchild->right;
  lchild->right           = node;

  if (parent == NULL)            *root         = lchild; /* case of root */
  else if (parent->left == node) parent->left  = lchild;
  else                           parent->right = lchild;
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
 * avl_insert - inserts @key and @value into @tree
 *
 * @tree:  tree to insert @key and @value into
 * @key:   the key to insert
 * @value: the value to insert
 * @less:  operator defining the (partial) node order
 */
extern inline void avl_insert(struct avl_node **restrict tree, const void *restrict key, void *restrict value, bool (*less)(const void *restrict, const void *restrict)) {
           struct avl_node *parent;
  register struct avl_node *walk  = *tree;
           struct avl_node *x     = NULL;
           struct stack    *stack = NULL;

  while (walk != NULL) {
    if (!(less(key, walk->key) || less(walk->key, key))) { stack_free(stack); return; }
    stack_push(&stack, walk);
    walk = less(key, walk->key) ? walk->left : walk->right;
  }

  walk        = avl_alloc();
  walk->key   = key;
  walk->value = value;

  if ((parent = stack_top(stack)) == NULL) *tree         = walk;
  else if (less(key, parent->key))         parent->left  = walk;
  else                                     parent->right = walk;

  while (!stack_empty(stack)) {
    walk         = stack_pop(&stack);
    walk->height = 1 + __max(avl_height(walk->left), avl_height(walk->right));
    if (1 + avl_height(walk->right) < avl_height(walk->left) || 1 + avl_height(walk->left) < avl_height(walk->right)) { x = walk; parent = stack_top(stack); break; }
  }

  if (x == NULL) return;

  if (1 + avl_height(x->right) < avl_height(x->left)) {
    if (avl_height(x->left->left) < avl_height(x->left->right)) {   /* case of Left Right */
      stack_push(&stack, x->left->right);
      stack_push(&stack, x->left);
      stack_push(&stack, x);
      avl_rotate_left(tree, x->left, x);
      avl_rotate_right(tree, x, parent);
    } else {                                                        /* case of Left Left */
      stack_push(&stack, x->left);
      stack_push(&stack, x);
      avl_rotate_right(tree, x, parent);
    }
  } else {
    if (avl_height(x->right->right) < avl_height(x->right->left)) { /* case of Right Left */
      stack_push(&stack, x->right->left);
      stack_push(&stack, x->right);
      stack_push(&stack, x);
      avl_rotate_right(tree, x->right, x);
      avl_rotate_left(tree, x, parent);
    } else {                                                        /* case of Right Right */
      stack_push(&stack, x->right);
      stack_push(&stack, x);
      avl_rotate_left(tree, x, parent);
    }
  }

  while (!stack_empty(stack)) {
    walk         = stack_pop(&stack);
    walk->height = 1 + __max(avl_height(walk->left), avl_height(walk->right));
  }
}

/**
 * avl_erase - erases @key from @tree
 *
 * @tree: tree to erase @key from
 * @key:  the key to erase
 * @less: operator defining the (partial) node order
 */
extern inline void avl_erase(struct avl_node **restrict tree, const void *restrict key, bool (*less)(const void *restrict, const void *restrict)) {
           struct avl_node *parent;
  register struct avl_node *walk  = *tree;
           struct avl_node *x     = NULL;
           struct stack    *stack = NULL;

  while (walk != NULL && (less(key, walk->key) || less(walk->key, key))) {
    stack_push(&stack, walk);
    walk = less(key, walk->key) ? walk->left : walk->right;
  }

  if (walk == NULL) { stack_free(stack); return; }

  if (walk->left != NULL && walk->right != NULL) {                          /* case of degree 2 */
    parent = walk;
    stack_push(&stack, walk);

    if (avl_height(walk->left) <= avl_height(walk->right)) for (walk = walk->right; walk->left != NULL; walk = walk->left)  stack_push(&stack, walk);
    else                                                   for (walk = walk->left; walk->right != NULL; walk = walk->right) stack_push(&stack, walk);

    parent->key   = walk->key;
    parent->value = walk->value;
  }

  if (walk->left == NULL && walk->right == NULL) {                          /* case of degree 0 */
    if ((parent = stack_top(stack)) == NULL)   *tree         = NULL;        /* case of root */
    else if (parent->left == walk)             parent->left  = NULL;
    else                                       parent->right = NULL;
  } else {                                                                  /* case of degree 1 */
    if (walk->left != NULL) {
      if ((parent = stack_top(stack)) == NULL) *tree         = walk->left;  /* case of root */
      else if (parent->left == walk)           parent->left  = walk->left;
      else                                     parent->right = walk->left;
    } else {
      if ((parent = stack_top(stack)) == NULL) *tree         = walk->right; /* case of root */
      else if (parent->left == walk)           parent->left  = walk->right;
      else                                     parent->right = walk->right;
    }
  }

  free(walk);

  while (!stack_empty(stack)) {
    walk         = stack_pop(&stack);
    walk->height = 1 + __max(avl_height(walk->left), avl_height(walk->right));
    if (1 + avl_height(walk->right) < avl_height(walk->left) || 1 + avl_height(walk->left) < avl_height(walk->right)) { x = walk; parent = stack_top(stack); break; }
  }

  if (x == NULL) return;

  if (1 + avl_height(x->right) < avl_height(x->left)) {
    if (avl_height(x->left->left) < avl_height(x->left->right)) {           /* case of Left Right */
      stack_push(&stack, x->left->right);
      stack_push(&stack, x->left);
      stack_push(&stack, x);
      avl_rotate_left(tree, x->left, x);
      avl_rotate_right(tree, x, parent);
    } else {                                                                /* case of Left Left */
      stack_push(&stack, x->left);
      stack_push(&stack, x);
      avl_rotate_right(tree, x, parent);
    }
  } else {
    if (avl_height(x->right->right) < avl_height(x->right->left)) {         /* case of Right Left */
      stack_push(&stack, x->right->left);
      stack_push(&stack, x->right);
      stack_push(&stack, x);
      avl_rotate_right(tree, x->right, x);
      avl_rotate_left(tree, x, parent);
    } else {                                                                /* case of Right Right */
      stack_push(&stack, x->right);
      stack_push(&stack, x);
      avl_rotate_left(tree, x, parent);
    }
  }

  while (!stack_empty(stack)) {
    walk         = stack_pop(&stack);
    walk->height = 1 + __max(avl_height(walk->left), avl_height(walk->right));
  }
}

/**
 * avl_preorder - applies @func to each node of @tree preorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
extern inline void avl_preorder(const struct avl_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { func(tree->key, tree->value); avl_preorder(tree->left, func); avl_preorder(tree->right, func); } }

/**
 * avl_inorder - applies @func to each node of @tree inorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
extern inline void avl_inorder(const struct avl_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { avl_inorder(tree->left, func); func(tree->key, tree->value); avl_inorder(tree->right, func); } }

/**
 * avl_postorder - applies @func to each node of @tree postorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
extern inline void avl_postorder(const struct avl_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { avl_postorder(tree->left, func); avl_postorder(tree->right, func); func(tree->key, tree->value); } }

#endif /* _AVLTREE_H */
