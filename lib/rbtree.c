/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2021-2022 9rum
 *
 * rbtree.c - generic red-black tree definition
 */
#include <index/rbtree.h>
#include <index/stack.h>

/**
 * rb_alloc - allocates a node with @key and @value
 *
 * @key:   the key of the node
 * @value: the value of the node
 */
static inline struct rb_node *rb_alloc(const void *restrict key, void *restrict value) {
  struct rb_node *node = malloc(sizeof(struct rb_node));
  node->key            = key;
  node->value          = value;
  node->left           = NULL;
  node->right          = NULL;
  node->color          = false;
  return node;
}

/**
 * rb_rotate_left - rotates subtree rooted with @node counterclockwise
 *
 * @root:   root node of tree
 * @node:   root node of subtree
 * @parent: parent node of @node
 */
static inline void rb_rotate_left(struct rb_node **restrict root, struct rb_node *restrict node, struct rb_node *restrict parent) {
  struct rb_node *rchild = node->right;
  node->right            = rchild->left;
  rchild->left           = node;

  if (parent == NULL)            *root         = rchild; /* case of root */
  else if (parent->left == node) parent->left  = rchild;
  else                           parent->right = rchild;
}

/**
 * rb_rotate_right - rotates subtree rooted with @node clockwise
 *
 * @root:   root node of tree
 * @node:   root node of subtree
 * @parent: parent node of @node
 */
static inline void rb_rotate_right(struct rb_node **restrict root, struct rb_node *restrict node, struct rb_node *restrict parent) {
  struct rb_node *lchild = node->left;
  node->left             = lchild->right;
  lchild->right          = node;

  if (parent == NULL)            *root         = lchild; /* case of root */
  else if (parent->left == node) parent->left  = lchild;
  else                           parent->right = lchild;
}

static inline void __rb_clear(struct rb_node *restrict tree) { if (tree != NULL) { __rb_clear(tree->left); __rb_clear(tree->right); free(tree); } }

static inline void __rb_preorder(const struct rb_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { func(tree->key, tree->value); __rb_preorder(tree->left, func); __rb_preorder(tree->right, func); } }

static inline void __rb_inorder(const struct rb_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { __rb_inorder(tree->left, func); func(tree->key, tree->value); __rb_inorder(tree->right, func); } }

static inline void __rb_postorder(const struct rb_node *restrict tree, void (*func)(const void *restrict, void *restrict)) { if (tree != NULL) { __rb_postorder(tree->left, func); __rb_postorder(tree->right, func); func(tree->key, tree->value); } }

extern struct rb_node *rb_insert(struct rb_root *restrict tree, const void *restrict key, void *restrict value) {
  register struct rb_node *parent;
  register struct rb_node *gparent;
  register struct rb_node *uncle;
  register struct rb_node *walk  = tree->root;
           struct stack   *stack = NULL;

  while (walk != NULL) {
    if (tree->less(key, walk->key))      { stack_push(&stack, walk); walk = walk->left; }
    else if (tree->less(walk->key, key)) { stack_push(&stack, walk); walk = walk->right; }
    else                                 { stack_clear(&stack); return NULL; }
  }

  struct rb_node *node = rb_alloc(key, value);
  walk                 = node;

  if ((parent = stack_top(stack)) == NULL) tree->root    = node;
  else if (tree->less(key, parent->key))   parent->left  = node;
  else                                     parent->right = node;

  ++tree->size;

  while (!stack_empty(stack)) {
    if ((parent = stack_pop(&stack))->color) { stack_clear(&stack); return node; }

    gparent = stack_pop(&stack);
    uncle   = gparent->right == parent ? gparent->left : gparent->right;

    if (uncle == NULL || uncle->color) { /* case of rearranging */
      if (gparent->left == parent) {
        if (parent->left == walk) {      /* case of Left Left */
          parent->color  = true;
          gparent->color = false;
          rb_rotate_right(&tree->root, gparent, stack_top(stack));
        } else {                         /* case of Left Right */
          walk->color    = true;
          gparent->color = false;
          rb_rotate_left(&tree->root, parent, gparent);
          rb_rotate_right(&tree->root, gparent, stack_top(stack));
        }
      } else {
        if (parent->left == walk) {      /* case of Right Left */
          walk->color    = true;
          gparent->color = false;
          rb_rotate_right(&tree->root, parent, gparent);
          rb_rotate_left(&tree->root, gparent, stack_top(stack));
        } else {                         /* case of Right Right */
          parent->color  = true;
          gparent->color = false;
          rb_rotate_left(&tree->root, gparent, stack_top(stack));
        }
      }

      stack_clear(&stack);
      return node;
    }

    parent->color  = true;               /* case of recoloring */
    uncle->color   = true;
    gparent->color = false;
    walk           = gparent;
  }

  tree->root->color = true;

  return node;
}

extern struct rb_node *rb_insert_or_assign(struct rb_root *restrict tree, const void *restrict key, void *restrict value) {
  register struct rb_node *parent;
  register struct rb_node *gparent;
  register struct rb_node *uncle;
  register struct rb_node *walk  = tree->root;
           struct stack   *stack = NULL;

  while (walk != NULL) {
    if (tree->less(key, walk->key))      { stack_push(&stack, walk); walk = walk->left; }
    else if (tree->less(walk->key, key)) { stack_push(&stack, walk); walk = walk->right; }
    else                                 { stack_clear(&stack); walk->value = value; return walk; }
  }

  struct rb_node *node = rb_alloc(key, value);
  walk                 = node;

  if ((parent = stack_top(stack)) == NULL) tree->root    = node;
  else if (tree->less(key, parent->key))   parent->left  = node;
  else                                     parent->right = node;

  while (!stack_empty(stack)) {
    if ((parent = stack_pop(&stack))->color) { stack_clear(&stack); return node; }

    gparent = stack_pop(&stack);
    uncle   = gparent->right == parent ? gparent->left : gparent->right;

    if (uncle == NULL || uncle->color) { /* case of rearranging */
      if (gparent->left == parent) {
        if (parent->left == walk) {      /* case of Left Left */
          parent->color  = true;
          gparent->color = false;
          rb_rotate_right(&tree->root, gparent, stack_top(stack));
        } else {                         /* case of Left Right */
          walk->color    = true;
          gparent->color = false;
          rb_rotate_left(&tree->root, parent, gparent);
          rb_rotate_right(&tree->root, gparent, stack_top(stack));
        }
      } else {
        if (parent->left == walk) {      /* case of Right Left */
          walk->color    = true;
          gparent->color = false;
          rb_rotate_right(&tree->root, parent, gparent);
          rb_rotate_left(&tree->root, gparent, stack_top(stack));
        } else {                         /* case of Right Right */
          parent->color  = true;
          gparent->color = false;
          rb_rotate_left(&tree->root, gparent, stack_top(stack));
        }
      }

      stack_clear(&stack);
      return node;
    }

    parent->color  = true;               /* case of recoloring */
    uncle->color   = true;
    gparent->color = false;
    walk           = gparent;
  }

  tree->root->color = true;

  return node;
}

extern void *rb_erase(struct rb_root *restrict tree, const void *restrict key) {
  register struct rb_node *parent;
  register struct rb_node *sibling;
  register struct rb_node *walk  = tree->root;
           struct stack   *stack = NULL;

  while (walk != NULL) {
    if (tree->less(key, walk->key))      { stack_push(&stack, walk); walk = walk->left; }
    else if (tree->less(walk->key, key)) { stack_push(&stack, walk); walk = walk->right; }
    else                                 break;
  }

  if (walk == NULL) { stack_clear(&stack); return NULL; }

  void *erased = walk->value;

  if (walk->left != NULL && walk->right != NULL) {                          /* case of degree 2 */
    parent = walk;
    stack_push(&stack, walk);

    for (walk = walk->left; walk->right != NULL; walk = walk->right) stack_push(&stack, walk);

    parent->key   = walk->key;
    parent->value = walk->value;
  }

  if (walk->left == NULL && walk->right == NULL) {                          /* case of degree 0 */
    if ((parent = stack_top(stack)) == NULL)   tree->root    = NULL;        /* case of root */
    else if (parent->left == walk)             parent->left  = NULL;
    else                                       parent->right = NULL;
  } else {                                                                  /* case of degree 1 */
    if (walk->left != NULL) {
      if ((parent = stack_top(stack)) == NULL) tree->root    = walk->left;  /* case of root */
      else if (parent->left == walk)           parent->left  = walk->left;
      else                                     parent->right = walk->left;
    } else {
      if ((parent = stack_top(stack)) == NULL) tree->root    = walk->right; /* case of root */
      else if (parent->left == walk)           parent->left  = walk->right;
      else                                     parent->right = walk->right;
    }
  }

  --tree->size;
  if (!walk->color) { free(walk); stack_clear(&stack); return erased; }

  parent = walk;
  walk   = parent->right == NULL ? parent->left : parent->right;
  free(parent);

  if (walk != NULL && !walk->color) { walk->color = true; stack_clear(&stack); return erased; }

  while (!stack_empty(stack)) {
    parent  = stack_pop(&stack);
    sibling = parent->right == walk ? parent->left : parent->right;

    if (!sibling->color) {                                                  /* case of rearranging */
      sibling->color = true;
      parent->color  = false;
      parent->left == walk ? rb_rotate_left(&tree->root, parent, stack_top(stack)) : rb_rotate_right(&tree->root, parent, stack_top(stack));
      stack_push(&stack, sibling);
      sibling = parent->right == walk ? parent->left : parent->right;
    }

    if (sibling->left != NULL && !sibling->left->color ||
        sibling->right != NULL && !sibling->right->color) {                 /* case of rearranging */
      if (parent->left == sibling) {
        if (sibling->right != NULL && !sibling->right->color) {             /* case of Left Right */
          sibling->right->color = true;
          sibling->color        = false;
          rb_rotate_left(&tree->root, sibling, parent);
          sibling = parent->left;
        }
        sibling->left->color = true;                                        /* case of Left Left */
        sibling->color       = parent->color;
        parent->color        = true;
        rb_rotate_right(&tree->root, parent, stack_top(stack));
      } else {
        if (sibling->left != NULL && !sibling->left->color) {               /* case of Right Left */
          sibling->left->color = true;
          sibling->color       = false;
          rb_rotate_right(&tree->root, sibling, parent);
          sibling = parent->right;
        }
        sibling->right->color = true;                                       /* case of Right Right */
        sibling->color        = parent->color;
        parent->color         = true;
        rb_rotate_left(&tree->root, parent, stack_top(stack));
      }

      stack_clear(&stack);
      return erased;
    }

    sibling->color = false;                                                 /* case of recoloring */
    if (!parent->color) { parent->color = true; stack_clear(&stack); return erased; }
    walk = parent;
  }

  return erased;
}

extern void rb_clear(struct rb_root *restrict tree) {
  __rb_clear(tree->root);
  tree->root = NULL;
  tree->size = 0;
}

extern void rb_preorder(const struct rb_root tree, void (*func)(const void *restrict, void *restrict)) { __rb_preorder(tree.root, func); }

extern void rb_inorder(const struct rb_root tree, void (*func)(const void *restrict, void *restrict)) { __rb_inorder(tree.root, func); }

extern void rb_postorder(const struct rb_root tree, void (*func)(const void *restrict, void *restrict)) { __rb_postorder(tree.root, func); }
