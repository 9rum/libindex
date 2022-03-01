/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2020-2022 9rum
 *
 * avltree.c - generic AVL tree definition
 */
#include <index/avltree.h>
#include <index/stack.h>

/**
 * avl_alloc - allocates a node with @key and @value
 *
 * @key:   the key of the node
 * @value: the value of the node
 */
static inline struct avl_node *avl_alloc(const void *restrict key, void *restrict value) {
  struct avl_node *node = malloc(sizeof(struct avl_node));
  node->key             = key;
  node->value           = value;
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

static inline void __avl_clear(struct avl_node *restrict tree) { if (tree != NULL) { __avl_clear(tree->left); __avl_clear(tree->right); free(tree); } }

static inline void __avl_for_each(const struct avl_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { __avl_for_each(tree->left, func); func(tree->key, tree->value); __avl_for_each(tree->right, func); } }

static inline void __avl_rev_each(const struct avl_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { __avl_rev_each(tree->right, func); func(tree->key, tree->value); __avl_rev_each(tree->left, func); } }

extern struct avl_node *avl_insert(struct avl_root *restrict tree, const void *restrict key, void *restrict value) {
           struct avl_node *parent;
  register struct avl_node *walk       = tree->root;
           struct avl_node *unbalanced = NULL;
           struct stack    *stack      = NULL;

  while (walk != NULL) {
    if (tree->less(key, walk->key))      { stack_push(&stack, walk); walk = walk->left; }
    else if (tree->less(walk->key, key)) { stack_push(&stack, walk); walk = walk->right; }
    else                                 { stack_clear(&stack); return NULL; }
  }

  struct avl_node *node = avl_alloc(key, value);

  if ((parent = stack_top(stack)) == NULL) tree->root    = node;
  else if (tree->less(key, parent->key))   parent->left  = node;
  else                                     parent->right = node;

  ++tree->size;

  while (!stack_empty(stack)) {
    walk         = stack_pop(&stack);
    walk->height = 1 + max(avl_height(walk->left), avl_height(walk->right));
    if (1 + avl_height(walk->right) < avl_height(walk->left) || 1 + avl_height(walk->left) < avl_height(walk->right)) {
      unbalanced = walk;
      parent     = stack_top(stack);
      break;
    }
  }

  if (unbalanced != NULL) {
    if (1 + avl_height(unbalanced->right) < avl_height(unbalanced->left)) {
      if (avl_height(unbalanced->left->left) < avl_height(unbalanced->left->right)) {   /* case of Left Right */
        stack_push(&stack, unbalanced->left->right);
        stack_push(&stack, unbalanced->left);
        stack_push(&stack, unbalanced);
        avl_rotate_left(&tree->root, unbalanced->left, unbalanced);
        avl_rotate_right(&tree->root, unbalanced, parent);
      } else {                                                                          /* case of Left Left */
        stack_push(&stack, unbalanced->left);
        stack_push(&stack, unbalanced);
        avl_rotate_right(&tree->root, unbalanced, parent);
      }
    } else {
      if (avl_height(unbalanced->right->right) < avl_height(unbalanced->right->left)) { /* case of Right Left */
        stack_push(&stack, unbalanced->right->left);
        stack_push(&stack, unbalanced->right);
        stack_push(&stack, unbalanced);
        avl_rotate_right(&tree->root, unbalanced->right, unbalanced);
        avl_rotate_left(&tree->root, unbalanced, parent);
      } else {                                                                          /* case of Right Right */
        stack_push(&stack, unbalanced->right);
        stack_push(&stack, unbalanced);
        avl_rotate_left(&tree->root, unbalanced, parent);
      }
    }

    while (!stack_empty(stack)) {
      walk         = stack_pop(&stack);
      walk->height = 1 + max(avl_height(walk->left), avl_height(walk->right));
    }
  }

  return node;
}

extern struct avl_node *avl_insert_or_assign(struct avl_root *restrict tree, const void *restrict key, void *restrict value) {
           struct avl_node *parent;
  register struct avl_node *walk       = tree->root;
           struct avl_node *unbalanced = NULL;
           struct stack    *stack      = NULL;

  while (walk != NULL) {
    if (tree->less(key, walk->key))      { stack_push(&stack, walk); walk = walk->left; }
    else if (tree->less(walk->key, key)) { stack_push(&stack, walk); walk = walk->right; }
    else                                 { stack_clear(&stack); walk->value = value; return walk; }
  }

  struct avl_node *node = avl_alloc(key, value);

  if ((parent = stack_top(stack)) == NULL) tree->root    = node;
  else if (tree->less(key, parent->key))   parent->left  = node;
  else                                     parent->right = node;

  ++tree->size;

  while (!stack_empty(stack)) {
    walk         = stack_pop(&stack);
    walk->height = 1 + max(avl_height(walk->left), avl_height(walk->right));
    if (1 + avl_height(walk->right) < avl_height(walk->left) || 1 + avl_height(walk->left) < avl_height(walk->right)) {
      unbalanced = walk;
      parent     = stack_top(stack);
      break;
    }
  }

  if (unbalanced != NULL) {
    if (1 + avl_height(unbalanced->right) < avl_height(unbalanced->left)) {
      if (avl_height(unbalanced->left->left) < avl_height(unbalanced->left->right)) {   /* case of Left Right */
        stack_push(&stack, unbalanced->left->right);
        stack_push(&stack, unbalanced->left);
        stack_push(&stack, unbalanced);
        avl_rotate_left(&tree->root, unbalanced->left, unbalanced);
        avl_rotate_right(&tree->root, unbalanced, parent);
      } else {                                                                          /* case of Left Left */
        stack_push(&stack, unbalanced->left);
        stack_push(&stack, unbalanced);
        avl_rotate_right(&tree->root, unbalanced, parent);
      }
    } else {
      if (avl_height(unbalanced->right->right) < avl_height(unbalanced->right->left)) { /* case of Right Left */
        stack_push(&stack, unbalanced->right->left);
        stack_push(&stack, unbalanced->right);
        stack_push(&stack, unbalanced);
        avl_rotate_right(&tree->root, unbalanced->right, unbalanced);
        avl_rotate_left(&tree->root, unbalanced, parent);
      } else {                                                                          /* case of Right Right */
        stack_push(&stack, unbalanced->right);
        stack_push(&stack, unbalanced);
        avl_rotate_left(&tree->root, unbalanced, parent);
      }
    }

    while (!stack_empty(stack)) {
      walk         = stack_pop(&stack);
      walk->height = 1 + max(avl_height(walk->left), avl_height(walk->right));
    }
  }

  return node;
}

extern void *avl_erase(struct avl_root *restrict tree, const void *restrict key) {
           struct avl_node *parent;
  register struct avl_node *walk       = tree->root;
           struct avl_node *unbalanced = NULL;
           struct stack    *stack      = NULL;

  while (walk != NULL) {
    if (tree->less(key, walk->key))      { stack_push(&stack, walk); walk = walk->left; }
    else if (tree->less(walk->key, key)) { stack_push(&stack, walk); walk = walk->right; }
    else                                 break;
  }

  if (walk == NULL) { stack_clear(&stack); return NULL; }

  void *erased = walk->value;

  if (walk->left != NULL && walk->right != NULL) {                                      /* case of degree 2 */
    parent = walk;
    stack_push(&stack, walk);

    if (avl_height(walk->left) <= avl_height(walk->right)) for (walk = walk->right; walk->left != NULL; walk = walk->left)  stack_push(&stack, walk);
    else                                                   for (walk = walk->left; walk->right != NULL; walk = walk->right) stack_push(&stack, walk);

    parent->key   = walk->key;
    parent->value = walk->value;
  }

  if (walk->left == NULL && walk->right == NULL) {                                      /* case of degree 0 */
    if ((parent = stack_top(stack)) == NULL)   tree->root    = NULL;                    /* case of root */
    else if (parent->left == walk)             parent->left  = NULL;
    else                                       parent->right = NULL;
  } else {                                                                              /* case of degree 1 */
    if (walk->left != NULL) {
      if ((parent = stack_top(stack)) == NULL) tree->root    = walk->left;              /* case of root */
      else if (parent->left == walk)           parent->left  = walk->left;
      else                                     parent->right = walk->left;
    } else {
      if ((parent = stack_top(stack)) == NULL) tree->root    = walk->right;             /* case of root */
      else if (parent->left == walk)           parent->left  = walk->right;
      else                                     parent->right = walk->right;
    }
  }

  free(walk);
  --tree->size;

  while (!stack_empty(stack)) {
    walk         = stack_pop(&stack);
    walk->height = 1 + max(avl_height(walk->left), avl_height(walk->right));
    if (1 + avl_height(walk->right) < avl_height(walk->left) || 1 + avl_height(walk->left) < avl_height(walk->right)) {
      unbalanced = walk;
      parent     = stack_top(stack);
      break;
    }
  }

  if (unbalanced != NULL) {
    if (1 + avl_height(unbalanced->right) < avl_height(unbalanced->left)) {
      if (avl_height(unbalanced->left->left) < avl_height(unbalanced->left->right)) {   /* case of Left Right */
        stack_push(&stack, unbalanced->left->right);
        stack_push(&stack, unbalanced->left);
        stack_push(&stack, unbalanced);
        avl_rotate_left(&tree->root, unbalanced->left, unbalanced);
        avl_rotate_right(&tree->root, unbalanced, parent);
      } else {                                                                          /* case of Left Left */
        stack_push(&stack, unbalanced->left);
        stack_push(&stack, unbalanced);
        avl_rotate_right(&tree->root, unbalanced, parent);
      }
    } else {
      if (avl_height(unbalanced->right->right) < avl_height(unbalanced->right->left)) { /* case of Right Left */
        stack_push(&stack, unbalanced->right->left);
        stack_push(&stack, unbalanced->right);
        stack_push(&stack, unbalanced);
        avl_rotate_right(&tree->root, unbalanced->right, unbalanced);
        avl_rotate_left(&tree->root, unbalanced, parent);
      } else {                                                                          /* case of Right Right */
        stack_push(&stack, unbalanced->right);
        stack_push(&stack, unbalanced);
        avl_rotate_left(&tree->root, unbalanced, parent);
      }
    }

    while (!stack_empty(stack)) {
      walk         = stack_pop(&stack);
      walk->height = 1 + max(avl_height(walk->left), avl_height(walk->right));
    }
  }

  return erased;
}

extern void avl_clear(struct avl_root *restrict tree) {
  __avl_clear(tree->root);
  tree->root = NULL;
  tree->size = 0;
}

extern void avl_for_each(const struct avl_root tree, void (*func)(const void *restrict, void *restrict)) { __avl_for_each(tree.root, func); }

extern void avl_rev_each(const struct avl_root tree, void (*func)(const void *restrict, void *restrict)) { __avl_rev_each(tree.root, func); }
