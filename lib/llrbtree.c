/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2021-2022 9rum
 *
 * llrbtree.c - generic left-leaning red-black tree definition
 */
#include <index/llrbtree.h>
#include <stdlib.h>

/**
 * llrb_alloc - allocates a node with @key and @value
 *
 * @key:    the key of the node
 * @value:  the value of the node
 * @parent: the address of the parent node
 * @tree:   the address of the tree to which the node belongs
 */
static inline struct llrb_node *llrb_alloc(const void *restrict key, void *restrict value, struct llrb_node *restrict parent, struct llrb_root *restrict tree) {
  struct llrb_node *node = malloc(sizeof(struct llrb_node));
  node->key              = key;
  node->value            = value;
  node->parent           = parent;
  node->left             = NULL;
  node->right            = NULL;
  node->tree             = tree;
  node->black            = false;
  return node;
}

/**
 * llrb_rotate_left - rotates subtree rooted with @node counterclockwise
 *
 * @node: root node of subtree
 */
static inline struct llrb_node *llrb_rotate_left(struct llrb_node *node) {
  struct llrb_node *rchild = node->right;
  node->right              = rchild->left;
  rchild->left             = node;
  rchild->black            = node->black;
  node->black              = false;

  if (node->parent == NULL)            node->tree->root    = rchild; /* case of root */
  else if (node->parent->left == node) node->parent->left  = rchild;
  else                                 node->parent->right = rchild;

  rchild->parent = node->parent;
  node->parent   = rchild;

  if (node->right != NULL)
    node->right->parent = node;

  return rchild;
}

/**
 * llrb_rotate_right - rotates subtree rooted with @node clockwise
 *
 * @node: root node of subtree
 */
static inline struct llrb_node *llrb_rotate_right(struct llrb_node *node) {
  struct llrb_node *lchild = node->left;
  node->left               = lchild->right;
  lchild->right            = node;
  lchild->black            = node->black;
  node->black              = false;

  if (node->parent == NULL)            node->tree->root    = lchild; /* case of root */
  else if (node->parent->left == node) node->parent->left  = lchild;
  else                                 node->parent->right = lchild;

  lchild->parent = node->parent;
  node->parent   = lchild;

  if (node->left != NULL)
    node->left->parent = node;

  return lchild;
}

/**
 * llrb_flip - flips the colors of @node and its children
 *
 * @node: node to flip color
 */
static inline void llrb_flip(struct llrb_node *node) {
  node->black        = !node->black;
  node->left->black  = !node->left->black;
  node->right->black = !node->right->black;
}

/**
 * llrb_rebalance - rebalances tree from @node
 *
 * @node: node to initiate rebalancing
 */
static inline void llrb_rebalance(struct llrb_node *node) {
  for (; node != NULL; node = node->parent) {
    if (node->right != NULL && !node->right->black)                                                       /* case of right-leaning red */
      node = llrb_rotate_left(node);
    if (node->left != NULL && !node->left->black && node->left->left != NULL && !node->left->left->black) /* case of double reds */
      node = llrb_rotate_right(node);
    if (node->left != NULL && !node->left->black && node->right != NULL && !node->right->black)           /* case of 4-node */
      llrb_flip(node);
  }
}

/**
 * llrb_move_left - carries a red link down the left spine of @node
 *
 * @node: node to move a red link left
 */
static inline struct llrb_node *llrb_move_left(struct llrb_node *node) {
  llrb_flip(node);

  if (node->right->left != NULL && !node->right->left->black) {
    node->right = llrb_rotate_right(node->right);
    node        = llrb_rotate_left(node);
    llrb_flip(node);
  }

  return node;
}

/**
 * llrb_move_right - carries a red link down the right spine of @node
 *
 * @node: node to move a red link right
 */
static inline struct llrb_node *llrb_move_right(struct llrb_node *node) {
  llrb_flip(node);

  if (node->left->left != NULL && !node->left->left->black) {
    node = llrb_rotate_right(node);
    llrb_flip(node);
  }

  return node;
}

/**
 * llrb_lower_bound - finds logical lower bound of @node
 *
 * @node: node to find logical lower bound of
 */
static inline struct llrb_node *llrb_lower_bound(struct llrb_node *node) {
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
 * llrb_upper_bound - finds logical upper bound of @node
 *
 * @node: node to find logical upper bound of
 */
static inline struct llrb_node *llrb_upper_bound(struct llrb_node *node) {
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
 * llrb_mk_iter - creates an iterator from @node
 *
 * @node: node to create an iterator from
 */
static inline struct llrb_iter llrb_mk_iter(struct llrb_node *node) {
  struct llrb_iter iter = {
    .pivot = node,
    .key   = node == NULL ? NULL : node->key,
    .value = node == NULL ? NULL : node->value,
  };
  return iter;
}

/**
 * llrb_mk_reverse_iter - creates a reverse iterator from @node
 *
 * @node: node to create a reverse iterator from
 */
static inline struct llrb_reverse_iter llrb_mk_reverse_iter(struct llrb_node *node) {
  struct llrb_reverse_iter iter = {
    .pivot = node,
    .key   = node == NULL ? NULL : node->key,
    .value = node == NULL ? NULL : node->value,
  };
  return iter;
}

/**
 * llrb_destroy - erases all entries in tree
 *
 * @node: root node of tree
 */
static inline void llrb_destroy(struct llrb_node *node) {
  register struct llrb_node *next;

  while (node != NULL) {
    llrb_destroy(node->right);
    next = node->left;
    free(node);
    node = next;
  }
}

extern struct llrb_iter llrb_find(const struct llrb_root tree, const void *key) {
  register struct llrb_node *pivot = tree.root;

  while (pivot != NULL) {
    if (tree.less(key, pivot->key))      pivot = pivot->left;
    else if (tree.less(pivot->key, key)) pivot = pivot->right;
    else                                 break;
  }

  return llrb_mk_iter(pivot);
}

extern struct llrb_iter llrb_insert(struct llrb_root *restrict tree, const void *restrict key, void *restrict value) {
  register struct llrb_node *parent = NULL;
  register struct llrb_node *pivot  = tree->root;

  while (pivot != NULL) {
    if (tree->less(key, pivot->key)) {
      parent = pivot;
      pivot  = pivot->left;
    } else if (tree->less(pivot->key, key)) {
      parent = pivot;
      pivot  = pivot->right;
    } else {
      return llrb_mk_iter(pivot);
    }
  }

  struct llrb_node *node = llrb_alloc(key, value, parent, tree);

  if (parent == NULL)                    tree->root    = node;
  else if (tree->less(key, parent->key)) parent->left  = node;
  else                                   parent->right = node;

  ++tree->size;

  llrb_rebalance(parent);

  tree->root->black = true;

  return llrb_mk_iter(node);
}

extern void *llrb_erase(struct llrb_root *restrict tree, const void *restrict key) {
  register struct llrb_node *parent = NULL;
  register struct llrb_node *pivot  = tree->root;
           void             *erased = NULL;

  while (pivot != NULL) {
    if (tree->less(key, pivot->key)) {
      if ((pivot->left == NULL || pivot->left->black) && (pivot->left->left == NULL || pivot->left->left->black))
        pivot = llrb_move_left(pivot);

      parent = pivot;
      pivot  = pivot->left;
    } else {
      if (pivot->left != NULL && !pivot->left->black)
        pivot = llrb_rotate_right(pivot);

      if (!tree->less(pivot->key, key) && pivot->right == NULL) {
        erased = pivot->value;

        if (parent == NULL)             tree->root    = NULL; /* case of root */
        else if (parent->left == pivot) parent->left  = NULL;
        else                            parent->right = NULL;

        --tree->size;

        free(pivot);
        break;
      }

      if ((pivot->right == NULL || pivot->right->black) && (pivot->right->left == NULL || pivot->right->left->black))
        pivot = llrb_move_right(pivot);

      if (!tree->less(pivot->key, key)) {
        erased = pivot->value;
        parent = pivot;

        for (pivot = pivot->right; pivot->left != NULL; pivot = pivot->left)
          if ((pivot->left == NULL || pivot->left->black) && (pivot->left->left == NULL || pivot->left->left->black))
            pivot = llrb_move_left(pivot);

        parent->key   = pivot->key;
        parent->value = pivot->value;
        parent        = pivot->parent;

        if (parent->left == pivot) parent->left  = NULL;
        else                       parent->right = NULL;

        --tree->size;

        free(pivot);
        break;
      } else {
        parent = pivot;
        pivot  = pivot->right;
      }
    }
  }

  llrb_rebalance(parent);

  if (tree->root != NULL)
    tree->root->black = true;

  return erased;
}

extern void llrb_clear(struct llrb_root *tree) {
  llrb_destroy(tree->root);
  tree->root = NULL;
  tree->size = 0;
}

extern struct llrb_iter llrb_iter_init(const struct llrb_root tree) {
  register struct llrb_node *pivot = tree.root;

  if (pivot != NULL)
    while (pivot->left != NULL)
      pivot = pivot->left;

  return llrb_mk_iter(pivot);
}

extern void llrb_iter_prev(struct llrb_iter *iter) {
  iter->pivot = llrb_lower_bound(iter->pivot);
  iter->key   = iter->pivot == NULL ? NULL : iter->pivot->key;
  iter->value = iter->pivot == NULL ? NULL : iter->pivot->value;
}

extern void llrb_iter_next(struct llrb_iter *iter) {
  iter->pivot = llrb_upper_bound(iter->pivot);
  iter->key   = iter->pivot == NULL ? NULL : iter->pivot->key;
  iter->value = iter->pivot == NULL ? NULL : iter->pivot->value;
}

extern struct llrb_reverse_iter llrb_reverse_iter_init(const struct llrb_root tree) {
  register struct llrb_node *pivot = tree.root;

  if (pivot != NULL)
    while (pivot->right != NULL)
      pivot = pivot->right;

  return llrb_mk_reverse_iter(pivot);
}

extern void llrb_reverse_iter_prev(struct llrb_reverse_iter *iter) {
  iter->pivot = llrb_upper_bound(iter->pivot);
  iter->key   = iter->pivot == NULL ? NULL : iter->pivot->key;
  iter->value = iter->pivot == NULL ? NULL : iter->pivot->value;
}

extern void llrb_reverse_iter_next(struct llrb_reverse_iter *iter) {
  iter->pivot = llrb_lower_bound(iter->pivot);
  iter->key   = iter->pivot == NULL ? NULL : iter->pivot->key;
  iter->value = iter->pivot == NULL ? NULL : iter->pivot->value;
}
