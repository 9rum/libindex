/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2020-2022 9rum
 *
 * avltree.c - generic AVL tree definition
 */
#include <index/avltree.h>
#include <stdlib.h>

/**
 * avl_alloc - allocates a node with @key and @value
 *
 * @key:    the key of the node
 * @value:  the value of the node
 * @parent: the address of the parent node
 * @tree:   the address of the tree to which the node belongs
 */
static inline struct avl_node *avl_alloc(const void *restrict key, void *restrict value, struct avl_node *restrict parent, struct avl_root *restrict tree) {
  struct avl_node *node = malloc(sizeof(struct avl_node));
  node->key             = key;
  node->value           = value;
  node->parent          = parent;
  node->left            = NULL;
  node->right           = NULL;
  node->tree            = tree;
  node->height          = 1;
  return node;
}

static inline size_t max(const size_t lhs, const size_t rhs) { return lhs < rhs ? rhs : lhs; }

/**
 * avl_height - returns the height of subtree rooted with @node
 *
 * @node: root node of subtree to get the height
 */
static inline size_t avl_height(const struct avl_node *node) { return node == NULL ? 0 : node->height; }

/**
 * avl_rotate_left - rotates subtree rooted with @node counterclockwise
 *
 * @node: root node of subtree
 */
static inline void avl_rotate_left(struct avl_node *node) {
  struct avl_node *rchild = node->right;
  node->right             = rchild->left;
  rchild->left            = node;

  if (node->parent == NULL)            node->tree->root    = rchild; /* case of root */
  else if (node->parent->left == node) node->parent->left  = rchild;
  else                                 node->parent->right = rchild;

  rchild->parent = node->parent;
  node->parent   = rchild;

  if (node->right != NULL)
    node->right->parent = node;
}

/**
 * avl_rotate_right - rotates subtree rooted with @node clockwise
 *
 * @node: root node of subtree
 */
static inline void avl_rotate_right(struct avl_node *node) {
  struct avl_node *lchild = node->left;
  node->left              = lchild->right;
  lchild->right           = node;

  if (node->parent == NULL)            node->tree->root    = lchild; /* case of root */
  else if (node->parent->left == node) node->parent->left  = lchild;
  else                                 node->parent->right = lchild;

  lchild->parent = node->parent;
  node->parent   = lchild;

  if (node->left != NULL)
    node->left->parent = node;
}

/**
 * avl_rebalance - rebalances subtree rooted with @node
 *
 * @node: root node of subtree
 */
static inline void avl_rebalance(struct avl_node *node) {
  if (1 + avl_height(node->right) < avl_height(node->left)) {
    if (avl_height(node->left->left) < avl_height(node->left->right)) {   /* case of Left Right */
      avl_rotate_left(node->left);
      avl_rotate_right(node);
      node->parent->left->height = 1 + max(avl_height(node->parent->left->left), avl_height(node->parent->left->right));
    } else {                                                              /* case of Left Left */
      avl_rotate_right(node);
    }
  } else {
    if (avl_height(node->right->right) < avl_height(node->right->left)) { /* case of Right Left */
      avl_rotate_right(node->right);
      avl_rotate_left(node);
      node->parent->right->height = 1 + max(avl_height(node->parent->right->left), avl_height(node->parent->right->right));
    } else {                                                              /* case of Right Right */
      avl_rotate_left(node);
    }
  }

  for (; node != NULL; node = node->parent)
    node->height = 1 + max(avl_height(node->left), avl_height(node->right));
}

/**
 * avl_lower_bound - finds logical lower bound of @node
 *
 * @node: node to find logical lower bound
 */
static inline struct avl_node *avl_lower_bound(struct avl_node *node) {
  if (node == NULL)
    return NULL;

  if (node->left != NULL) {
    for (node = node->left; node->right != NULL; node = node->right);
    return node;
  }

  for (; node->parent != NULL && node->parent->left == node; node = node->parent);
  return node->parent;
}

/**
 * avl_upper_bound - finds logical upper bound of @node
 *
 * @node: node to find logical upper bound
 */
static inline struct avl_node *avl_upper_bound(struct avl_node *node) {
  if (node == NULL)
    return NULL;

  if (node->right != NULL) {
    for (node = node->right; node->left != NULL; node = node->left);
    return node;
  }

  for (; node->parent != NULL && node->parent->right == node; node = node->parent);
  return node->parent;
}

/**
 * avl_mk_iter - creates an iterator from @node
 *
 * @node: node to create an iterator from
 */
static inline struct avl_iter avl_mk_iter(struct avl_node *node) {
  struct avl_iter iter = {
    .node  = node,
    .key   = node == NULL ? NULL : node->key,
    .value = node == NULL ? NULL : node->value,
  };
  return iter;
}

/**
 * avl_mk_reverse_iter - creates a reverse iterator from @node
 *
 * @node: node to create a reverse iterator from
 */
static inline struct avl_reverse_iter avl_mk_reverse_iter(struct avl_node *node) {
  struct avl_reverse_iter iter = {
    .node  = node,
    .key   = node == NULL ? NULL : node->key,
    .value = node == NULL ? NULL : node->value,
  };
  return iter;
}

/**
 * avl_destroy - erases all entries in tree
 *
 * @node: root node of tree
 */
static inline void avl_destroy(struct avl_node *node) {
  register struct avl_node *next;

  while (node != NULL) {
    avl_destroy(node->right);
    next = node->left;
    free(node);
    node = next;
  }
}

extern struct avl_iter avl_find(const struct avl_root tree, const void *key) {
  register struct avl_node *walk = tree.root;

  while (walk != NULL) {
    if (tree.less(key, walk->key))      walk = walk->left;
    else if (tree.less(walk->key, key)) walk = walk->right;
    else                                return avl_mk_iter(walk);
  }

  return avl_mk_iter(NULL);
}

extern struct avl_iter avl_insert(struct avl_root *restrict tree, const void *restrict key, void *restrict value) {
  register struct avl_node *parent = NULL;
  register struct avl_node *walk   = tree->root;

  while (walk != NULL) {
    if (tree->less(key, walk->key)) {
      parent = walk;
      walk   = walk->left;
    } else if (tree->less(walk->key, key)) {
      parent = walk;
      walk   = walk->right;
    } else {
      return avl_mk_iter(walk);
    }
  }

  struct avl_node *node = avl_alloc(key, value, parent, tree);

  if (parent == NULL)                    tree->root    = node;
  else if (tree->less(key, parent->key)) parent->left  = node;
  else                                   parent->right = node;

  ++tree->size;

  for (walk = parent; walk != NULL; walk = walk->parent) {
    walk->height = 1 + max(avl_height(walk->left), avl_height(walk->right));
    if (1 + avl_height(walk->right) < avl_height(walk->left) || 1 + avl_height(walk->left) < avl_height(walk->right))
      break;
  }

  if (walk != NULL)
    avl_rebalance(walk);

  return avl_mk_iter(node);
}

extern struct avl_iter avl_replace(struct avl_root *restrict tree, const void *restrict key, void *restrict value) {
  register struct avl_node *parent = NULL;
  register struct avl_node *walk   = tree->root;

  while (walk != NULL) {
    if (tree->less(key, walk->key)) {
      parent = walk;
      walk   = walk->left;
    } else if (tree->less(walk->key, key)) {
      parent = walk;
      walk   = walk->right;
    } else {
      walk->value = value;
      return avl_mk_iter(walk);
    }
  }

  struct avl_node *node = avl_alloc(key, value, parent, tree);

  if (parent == NULL)                    tree->root    = node;
  else if (tree->less(key, parent->key)) parent->left  = node;
  else                                   parent->right = node;

  ++tree->size;

  for (walk = parent; walk != NULL; walk = walk->parent) {
    walk->height = 1 + max(avl_height(walk->left), avl_height(walk->right));
    if (1 + avl_height(walk->right) < avl_height(walk->left) || 1 + avl_height(walk->left) < avl_height(walk->right))
      break;
  }

  if (walk != NULL)
    avl_rebalance(walk);

  return avl_mk_iter(node);
}

extern void *avl_erase(struct avl_root *restrict tree, const void *restrict key) {
  register struct avl_node *parent = NULL;
  register struct avl_node *walk   = tree->root;

  while (walk != NULL) {
    if (tree->less(key, walk->key)) {
      parent = walk;
      walk   = walk->left;
    } else if (tree->less(walk->key, key)) {
      parent = walk;
      walk   = walk->right;
    } else {
      break;
    }
  }

  if (walk == NULL)
    return NULL;

  void *erased = walk->value;

  if (walk->left != NULL && walk->right != NULL) {                /* case of degree 2 */
    parent = walk;

    if (avl_height(walk->left) < avl_height(walk->right))
      for (walk = walk->right; walk->left != NULL; walk = walk->left);
    else
      for (walk = walk->left; walk->right != NULL; walk = walk->right);

    parent->key   = walk->key;
    parent->value = walk->value;
    parent        = walk->parent;
  }

  if (walk->left == NULL && walk->right == NULL) {                /* case of degree 0 */
    if (parent == NULL)            tree->root    = NULL;          /* case of root */
    else if (parent->left == walk) parent->left  = NULL;
    else                           parent->right = NULL;
  } else {                                                        /* case of degree 1 */
    if (walk->left != NULL) {
      if (parent == NULL)            tree->root    = walk->left;  /* case of root */
      else if (parent->left == walk) parent->left  = walk->left;
      else                           parent->right = walk->left;
    } else {
      if (parent == NULL)            tree->root    = walk->right; /* case of root */
      else if (parent->left == walk) parent->left  = walk->right;
      else                           parent->right = walk->right;
    }
  }

  free(walk);
  --tree->size;

  for (walk = parent; walk != NULL; walk = walk->parent) {
    walk->height = 1 + max(avl_height(walk->left), avl_height(walk->right));
    if (1 + avl_height(walk->right) < avl_height(walk->left) || 1 + avl_height(walk->left) < avl_height(walk->right))
      break;
  }

  if (walk != NULL)
    avl_rebalance(walk);

  return erased;
}

extern void avl_clear(struct avl_root *tree) {
  avl_destroy(tree->root);
  tree->root = NULL;
  tree->size = 0;
}

extern struct avl_iter avl_iter_init(const struct avl_root tree) {
  register struct avl_node *walk = tree.root;

  if (walk != NULL)
    while (walk->left != NULL)
      walk = walk->left;

  return avl_mk_iter(walk);
}

extern void avl_iter_prev(struct avl_iter *iter) {
  iter->node  = avl_lower_bound(iter->node);
  iter->key   = iter->node == NULL ? NULL : iter->node->key;
  iter->value = iter->node == NULL ? NULL : iter->node->value;
}

extern void avl_iter_next(struct avl_iter *iter) {
  iter->node  = avl_upper_bound(iter->node);
  iter->key   = iter->node == NULL ? NULL : iter->node->key;
  iter->value = iter->node == NULL ? NULL : iter->node->value;
}

extern struct avl_reverse_iter avl_reverse_iter_init(const struct avl_root tree) {
  register struct avl_node *walk = tree.root;

  if (walk != NULL)
    while (walk->right != NULL)
      walk = walk->right;

  return avl_mk_reverse_iter(walk);
}

extern void avl_reverse_iter_prev(struct avl_reverse_iter *iter) {
  iter->node  = avl_upper_bound(iter->node);
  iter->key   = iter->node == NULL ? NULL : iter->node->key;
  iter->value = iter->node == NULL ? NULL : iter->node->value;
}

extern void avl_reverse_iter_next(struct avl_reverse_iter *iter) {
  iter->node  = avl_lower_bound(iter->node);
  iter->key   = iter->node == NULL ? NULL : iter->node->key;
  iter->value = iter->node == NULL ? NULL : iter->node->value;
}
