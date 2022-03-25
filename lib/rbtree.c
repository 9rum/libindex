/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2021-2022 9rum
 *
 * rbtree.c - generic red-black tree definition
 */
#include <index/rbtree.h>
#include <stdlib.h>

/**
 * rb_alloc - allocates a node with @key and @value
 *
 * @key:    the key of the node
 * @value:  the value of the node
 * @parent: the address of the parent node
 * @tree:   the address of the tree to which the node belongs
 */
static inline struct rb_node *rb_alloc(const void *restrict key, void *restrict value, struct rb_node *restrict parent, struct rb_root *restrict tree) {
  struct rb_node *node = malloc(sizeof(struct rb_node));
  node->key            = key;
  node->value          = value;
  node->parent         = parent;
  node->left           = NULL;
  node->right          = NULL;
  node->tree           = tree;
  node->black          = false;
  return node;
}

/**
 * rb_rotate_left - rotates subtree rooted with @node counterclockwise
 *
 * @node: root node of subtree
 */
static inline void rb_rotate_left(struct rb_node *node) {
  struct rb_node *rchild = node->right;
  node->right            = rchild->left;
  rchild->left           = node;

  if (node->parent == NULL)            node->tree->root    = rchild; /* case of root */
  else if (node->parent->left == node) node->parent->left  = rchild;
  else                                 node->parent->right = rchild;

  rchild->parent = node->parent;
  node->parent   = rchild;

  if (node->right != NULL)
    node->right->parent = node;
}

/**
 * rb_rotate_right - rotates subtree rooted with @node clockwise
 *
 * @node: root node of subtree
 */
static inline void rb_rotate_right(struct rb_node *node) {
  struct rb_node *lchild = node->left;
  node->left             = lchild->right;
  lchild->right          = node;

  if (node->parent == NULL)            node->tree->root    = lchild; /* case of root */
  else if (node->parent->left == node) node->parent->left  = lchild;
  else                                 node->parent->right = lchild;

  lchild->parent = node->parent;
  node->parent   = lchild;

  if (node->left != NULL)
    node->left->parent = node;
}

/**
 * rb_lower_bound - finds logical lower bound of @node
 *
 * @node: node to find logical lower bound of
 */
static inline struct rb_node *rb_lower_bound(struct rb_node *node) {
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
 * rb_upper_bound - finds logical upper bound of @node
 *
 * @node: node to find logical upper bound of
 */
static inline struct rb_node *rb_upper_bound(struct rb_node *node) {
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
 * rb_mk_iter - creates an iterator from @node
 *
 * @node: node to create an iterator from
 */
static inline struct rb_iter rb_mk_iter(struct rb_node *node) {
  struct rb_iter iter = {
    .pivot = node,
    .key   = node == NULL ? NULL : node->key,
    .value = node == NULL ? NULL : node->value,
  };
  return iter;
}

/**
 * rb_mk_reverse_iter - creates a reverse iterator from @node
 *
 * @node: node to create a reverse iterator from
 */
static inline struct rb_reverse_iter rb_mk_reverse_iter(struct rb_node *node) {
  struct rb_reverse_iter iter = {
    .pivot = node,
    .key   = node == NULL ? NULL : node->key,
    .value = node == NULL ? NULL : node->value,
  };
  return iter;
}

/**
 * rb_destroy - erases all entries in tree
 *
 * @node: root node of tree
 */
static inline void rb_destroy(struct rb_node *node) {
  register struct rb_node *next;

  while (node != NULL) {
    rb_destroy(node->right);
    next = node->left;
    free(node);
    node = next;
  }
}

extern struct rb_iter rb_find(const struct rb_root tree, const void *key) {
  register struct rb_node *pivot = tree.root;

  while (pivot != NULL) {
    if (tree.less(key, pivot->key))      pivot = pivot->left;
    else if (tree.less(pivot->key, key)) pivot = pivot->right;
    else                                 break;
  }

  return rb_mk_iter(pivot);
}

extern struct rb_iter rb_insert(struct rb_root *restrict tree, const void *restrict key, void *restrict value) {
  register struct rb_node *gparent;
  register struct rb_node *uncle;
  register struct rb_node *parent = NULL;
  register struct rb_node *pivot  = tree->root;

  while (pivot != NULL) {
    if (tree->less(key, pivot->key)) {
      parent = pivot;
      pivot  = pivot->left;
    } else if (tree->less(pivot->key, key)) {
      parent = pivot;
      pivot  = pivot->right;
    } else {
      return rb_mk_iter(pivot);
    }
  }

  struct rb_node *node = rb_alloc(key, value, parent, tree);

  if (parent == NULL)                    tree->root    = node;
  else if (tree->less(key, parent->key)) parent->left  = node;
  else                                   parent->right = node;

  ++tree->size;

  for (pivot = node; pivot->parent != NULL; parent = pivot->parent) {
    if (parent->black)
      return rb_mk_iter(node);

    gparent = parent->parent;
    uncle   = gparent->right == parent ? gparent->left : gparent->right;

    if (uncle == NULL || uncle->black) { /* case of rearranging */
      if (gparent->left == parent) {
        if (parent->left == pivot) {     /* case of Left Left */
          parent->black  = true;
          gparent->black = false;
          rb_rotate_right(gparent);
        } else {                         /* case of Left Right */
          pivot->black   = true;
          gparent->black = false;
          rb_rotate_left(parent);
          rb_rotate_right(gparent);
        }
      } else {
        if (parent->right == pivot) {    /* case of Right Right */
          parent->black  = true;
          gparent->black = false;
          rb_rotate_left(gparent);
        } else {                         /* case of Right Left */
          pivot->black   = true;
          gparent->black = false;
          rb_rotate_right(parent);
          rb_rotate_left(gparent);
        }
      }

      return rb_mk_iter(node);
    }

    parent->black  = true;               /* case of recoloring */
    uncle->black   = true;
    gparent->black = false;
    pivot          = gparent;
  }

  tree->root->black = true;

  return rb_mk_iter(node);
}

extern struct rb_iter rb_replace(struct rb_root *restrict tree, const void *restrict key, void *restrict value) {
  register struct rb_node *gparent;
  register struct rb_node *uncle;
  register struct rb_node *parent = NULL;
  register struct rb_node *pivot  = tree->root;

  while (pivot != NULL) {
    if (tree->less(key, pivot->key)) {
      parent = pivot;
      pivot  = pivot->left;
    } else if (tree->less(pivot->key, key)) {
      parent = pivot;
      pivot  = pivot->right;
    } else {
      pivot->value = value;
      return rb_mk_iter(pivot);
    }
  }

  struct rb_node *node = rb_alloc(key, value, parent, tree);

  if (parent == NULL)                    tree->root    = node;
  else if (tree->less(key, parent->key)) parent->left  = node;
  else                                   parent->right = node;

  ++tree->size;

  for (pivot = node; pivot->parent != NULL; parent = pivot->parent) {
    if (parent->black)
      return rb_mk_iter(node);

    gparent = parent->parent;
    uncle   = gparent->right == parent ? gparent->left : gparent->right;

    if (uncle == NULL || uncle->black) { /* case of rearranging */
      if (gparent->left == parent) {
        if (parent->left == pivot) {     /* case of Left Left */
          parent->black  = true;
          gparent->black = false;
          rb_rotate_right(gparent);
        } else {                         /* case of Left Right */
          pivot->black   = true;
          gparent->black = false;
          rb_rotate_left(parent);
          rb_rotate_right(gparent);
        }
      } else {
        if (parent->right == pivot) {    /* case of Right Right */
          parent->black  = true;
          gparent->black = false;
          rb_rotate_left(gparent);
        } else {                         /* case of Right Left */
          pivot->black   = true;
          gparent->black = false;
          rb_rotate_right(parent);
          rb_rotate_left(gparent);
        }
      }

      return rb_mk_iter(node);
    }

    parent->black  = true;               /* case of recoloring */
    uncle->black   = true;
    gparent->black = false;
    pivot          = gparent;
  }

  tree->root->black = true;

  return rb_mk_iter(node);
}

extern void *rb_erase(struct rb_root *restrict tree, const void *restrict key) {
  register struct rb_node *sibling;
  register struct rb_node *parent = NULL;
  register struct rb_node *pivot  = tree->root;

  while (pivot != NULL) {
    if (tree->less(key, pivot->key)) {
      parent = pivot;
      pivot  = pivot->left;
    } else if (tree->less(pivot->key, key)) {
      parent = pivot;
      pivot  = pivot->right;
    } else {
      break;
    }
  }

  if (pivot == NULL)
    return NULL;

  void *erased = pivot->value;

  if (pivot->left != NULL && pivot->right != NULL) {                /* case of degree 2 */
    parent = pivot;

    for (pivot = pivot->left; pivot->right != NULL; pivot = pivot->right);

    parent->key   = pivot->key;
    parent->value = pivot->value;
    parent        = pivot->parent;
  }

  if (pivot->left == NULL && pivot->right == NULL) {                /* case of degree 0 */
    if (parent == NULL)             tree->root    = NULL;           /* case of root */
    else if (parent->left == pivot) parent->left  = NULL;
    else                            parent->right = NULL;
  } else {                                                          /* case of degree 1 */
    if (pivot->left != NULL) {
      if (parent == NULL)             tree->root    = pivot->left;  /* case of root */
      else if (parent->left == pivot) parent->left  = pivot->left;
      else                            parent->right = pivot->left;
      pivot->left->parent = parent;
    } else {
      if (parent == NULL)             tree->root    = pivot->right; /* case of root */
      else if (parent->left == pivot) parent->left  = pivot->right;
      else                            parent->right = pivot->right;
      pivot->right->parent = parent;
    }
  }

  --tree->size;

  if (!pivot->black) {
    free(pivot);
    return erased;
  }

  sibling = pivot;
  pivot   = sibling->right == NULL ? sibling->left : sibling->right;
  free(sibling);

  if (pivot != NULL && !pivot->black) {
    pivot->black = true;
    return erased;
  }

  for (; parent != NULL; parent = pivot->parent) {
    sibling = parent->right == pivot ? parent->left : parent->right;

    if (!sibling->black) {                                          /* case of rearranging */
      sibling->black = true;
      parent->black  = false;
      parent->left == pivot ? rb_rotate_left(parent) : rb_rotate_right(parent);
      sibling = parent->right == pivot ? parent->left : parent->right;
    }

    if (sibling->left != NULL && !sibling->left->black || sibling->right != NULL && !sibling->right->black) {
      if (parent->left == sibling) {
        if (sibling->right != NULL && !sibling->right->black) {     /* case of Left Right */
          sibling->right->black = true;
          sibling->black        = false;
          rb_rotate_left(sibling);
          sibling = parent->left;
        }
        sibling->left->black = true;                                /* case of Left Left */
        sibling->black       = parent->black;
        parent->black        = true;
        rb_rotate_right(parent);
      } else {
        if (sibling->left != NULL && !sibling->left->black) {       /* case of Right Left */
          sibling->left->black = true;
          sibling->black       = false;
          rb_rotate_right(sibling);
          sibling = parent->right;
        }
        sibling->right->black = true;                               /* case of Right Right */
        sibling->black        = parent->black;
        parent->black         = true;
        rb_rotate_left(parent);
      }

      return erased;
    }

    sibling->black = false;                                         /* case of recoloring */

    if (!parent->black) {
      parent->black = true;
      return erased;
    }

    pivot = parent;
  }

  return erased;
}

extern void rb_clear(struct rb_root *tree) {
  rb_destroy(tree->root);
  tree->root = NULL;
  tree->size = 0;
}

extern struct rb_iter rb_iter_init(const struct rb_root tree) {
  register struct rb_node *pivot = tree.root;

  if (pivot != NULL)
    while (pivot->left != NULL)
      pivot = pivot->left;

  return rb_mk_iter(pivot);
}

extern void rb_iter_prev(struct rb_iter *iter) {
  iter->pivot = rb_lower_bound(iter->pivot);
  iter->key   = iter->pivot == NULL ? NULL : iter->pivot->key;
  iter->value = iter->pivot == NULL ? NULL : iter->pivot->value;
}

extern void rb_iter_next(struct rb_iter *iter) {
  iter->pivot = rb_upper_bound(iter->pivot);
  iter->key   = iter->pivot == NULL ? NULL : iter->pivot->key;
  iter->value = iter->pivot == NULL ? NULL : iter->pivot->value;
}

extern struct rb_reverse_iter rb_reverse_iter_init(const struct rb_root tree) {
  register struct rb_node *pivot = tree.root;

  if (pivot != NULL)
    while (pivot->right != NULL)
      pivot = pivot->right;

  return rb_mk_reverse_iter(pivot);
}

extern void rb_reverse_iter_prev(struct rb_reverse_iter *iter) {
  iter->pivot = rb_upper_bound(iter->pivot);
  iter->key   = iter->pivot == NULL ? NULL : iter->pivot->key;
  iter->value = iter->pivot == NULL ? NULL : iter->pivot->value;
}

extern void rb_reverse_iter_next(struct rb_reverse_iter *iter) {
  iter->pivot = rb_lower_bound(iter->pivot);
  iter->key   = iter->pivot == NULL ? NULL : iter->pivot->key;
  iter->value = iter->pivot == NULL ? NULL : iter->pivot->value;
}
