/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2021-2022 9rum
 *
 * llrbtree.c - generic left-leaning red-black tree definition
 */
#include <index/llrbtree.h>
#include <index/stack.h>

/**
 * llrb_alloc - allocates a node with @key and @value
 *
 * @key:   the key of the node
 * @value: the value of the node
 */
static inline struct llrb_node *llrb_alloc(const void *restrict key, void *restrict value) {
  struct llrb_node *node = malloc(sizeof(struct llrb_node));
  node->key              = key;
  node->value            = value;
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

static inline void __llrb_clear(struct llrb_node *restrict tree) { if (tree != NULL) { __llrb_clear(tree->left); __llrb_clear(tree->right); free(tree); } }

static inline void __llrb_preorder(const struct llrb_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { func(tree->key, tree->value); __llrb_preorder(tree->left, func); __llrb_preorder(tree->right, func); } }

static inline void __llrb_inorder(const struct llrb_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { __llrb_inorder(tree->left, func); func(tree->key, tree->value); __llrb_inorder(tree->right, func); } }

static inline void __llrb_postorder(const struct llrb_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { __llrb_postorder(tree->left, func); __llrb_postorder(tree->right, func); func(tree->key, tree->value); } }

extern struct llrb_node *llrb_insert(struct llrb_root *restrict tree, const void *restrict key, void *restrict value) {
  register struct llrb_node *parent;
  register struct llrb_node *walk  = tree->root;
           struct stack     *stack = NULL;

  while (walk != NULL) {
    if (tree->less(key, walk->key))      { stack_push(&stack, walk); walk = walk->left; }
    else if (tree->less(walk->key, key)) { stack_push(&stack, walk); walk = walk->right; }
    else                                 { stack_clear(&stack); return NULL; }
  }

  struct llrb_node *node = llrb_alloc(key, value);

  if ((parent = stack_top(stack)) == NULL) tree->root    = node;
  else if (tree->less(key, parent->key))   parent->left  = node;
  else                                     parent->right = node;

  ++tree->size;

  while (!stack_empty(stack)) {
    walk   = stack_pop(&stack);
    parent = stack_top(stack);

    if (llrb_is_red(walk->right))                                 walk = llrb_rotate_left(&tree->root, walk, parent);  /* case of right-leaning red */
    if (llrb_is_red(walk->left) && llrb_is_red(walk->left->left)) walk = llrb_rotate_right(&tree->root, walk, parent); /* case of double reds */
    if (llrb_is_red(walk->left) && llrb_is_red(walk->right))      llrb_flip(walk);                                     /* case of 4-node */
  }

  tree->root->color = true;

  return node;
}

extern struct llrb_node *llrb_insert_or_assign(struct llrb_root *restrict tree, const void *restrict key, void *restrict value) {
  register struct llrb_node *parent;
  register struct llrb_node *walk  = tree->root;
           struct stack     *stack = NULL;

  while (walk != NULL) {
    if (tree->less(key, walk->key))      { stack_push(&stack, walk); walk = walk->left; }
    else if (tree->less(walk->key, key)) { stack_push(&stack, walk); walk = walk->right; }
    else                                 { stack_clear(&stack); walk->value = value; return walk; }
  }

  struct llrb_node *node = llrb_alloc(key, value);

  if ((parent = stack_top(stack)) == NULL) tree->root    = node;
  else if (tree->less(key, parent->key))   parent->left  = node;
  else                                     parent->right = node;

  ++tree->size;

  while (!stack_empty(stack)) {
    walk   = stack_pop(&stack);
    parent = stack_top(stack);

    if (llrb_is_red(walk->right))                                 walk = llrb_rotate_left(&tree->root, walk, parent);  /* case of right-leaning red */
    if (llrb_is_red(walk->left) && llrb_is_red(walk->left->left)) walk = llrb_rotate_right(&tree->root, walk, parent); /* case of double reds */
    if (llrb_is_red(walk->left) && llrb_is_red(walk->right))      llrb_flip(walk);                                     /* case of 4-node */
  }

  tree->root->color = true;

  return node;
}

extern void *llrb_erase(struct llrb_root *restrict tree, const void *restrict key) {
  register struct llrb_node *parent;
  register struct llrb_node *walk   = tree->root;
           struct stack     *stack  = NULL;
           void             *erased = NULL;

  while (walk != NULL) {
    if (tree->less(key, walk->key)) {
      if (llrb_is_black(walk->left) && llrb_is_black(walk->left->left)) walk = llrb_move_red_left(&tree->root, walk, stack_top(stack));
      stack_push(&stack, walk);
      walk = walk->left;
    } else {
      if (llrb_is_red(walk->left)) walk = llrb_rotate_right(&tree->root, walk, stack_top(stack));

      if (!tree->less(walk->key, key) && walk->right == NULL) {
        erased = walk->value;
        if ((parent = stack_top(stack)) == NULL) tree->root    = NULL;                                                 /* case of root */
        else if (parent->left == walk)           parent->left  = NULL;
        else                                     parent->right = NULL;
        free(walk);
        --tree->size;
        break;
      }

      if (llrb_is_black(walk->right) && llrb_is_black(walk->right->left)) walk = llrb_move_red_right(&tree->root, walk, stack_top(stack));

      if (!tree->less(walk->key, key)) {
        erased = walk->value;
        parent = walk;
        stack_push(&stack, walk);

        for (walk = walk->right; walk->left != NULL; walk = walk->left) {
          if (llrb_is_black(walk->left) && llrb_is_black(walk->left->left)) walk = llrb_move_red_left(&tree->root, walk, stack_top(stack));
          stack_push(&stack, walk);
        }

        parent->key   = walk->key;
        parent->value = walk->value;

        if ((parent = stack_top(stack))->left == walk) parent->left  = NULL;
        else                                           parent->right = NULL;
        free(walk);
        --tree->size;
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

    if (llrb_is_red(walk->right))                                 walk = llrb_rotate_left(&tree->root, walk, parent);  /* case of right-leaning red */
    if (llrb_is_red(walk->left) && llrb_is_red(walk->left->left)) walk = llrb_rotate_right(&tree->root, walk, parent); /* case of double reds */
    if (llrb_is_red(walk->left) && llrb_is_red(walk->right))      llrb_flip(walk);                                     /* case of 4-node */
  }

  if (tree->root != NULL) tree->root->color = true;

  return erased;
}

extern void llrb_clear(struct llrb_root *restrict tree) {
  __llrb_clear(tree->root);
  tree->root = NULL;
  tree->size = 0;
}

extern void llrb_preorder(const struct llrb_root tree, void (*func)(const void *restrict, void *restrict)) { __llrb_preorder(tree.root, func); }

extern void llrb_inorder(const struct llrb_root tree, void (*func)(const void *restrict, void *restrict)) { __llrb_inorder(tree.root, func); }

extern void llrb_postorder(const struct llrb_root tree, void (*func)(const void *restrict, void *restrict)) { __llrb_postorder(tree.root, func); }
