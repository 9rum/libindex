/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright 2020 - 2022 9rum
 *
 * avltree.c - generic AVL tree definition
 */
#include <index/avltree.h>

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

static inline size_t max(const size_t a, const size_t b) { return a < b ? b : a; }

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

extern void avl_insert(struct avl_node **restrict tree, const void *restrict key, void *restrict value, bool (*less)(const void *restrict, const void *restrict)) {
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
    walk->height = 1 + max(avl_height(walk->left), avl_height(walk->right));
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
    walk->height = 1 + max(avl_height(walk->left), avl_height(walk->right));
  }
}

extern void avl_erase(struct avl_node **restrict tree, const void *restrict key, bool (*less)(const void *restrict, const void *restrict)) {
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
    walk->height = 1 + max(avl_height(walk->left), avl_height(walk->right));
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
    walk->height = 1 + max(avl_height(walk->left), avl_height(walk->right));
  }
}
