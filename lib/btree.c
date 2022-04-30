/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2020-2022 9rum
 *
 * btree.c - generic B-tree definition
 */
#include <index/btree.h>
#include <stdlib.h>
#include <string.h>

/**
 * btree_alloc - allocates a node
 *
 * @order:  the order of tree
 * @parent: the address of the parent node
 * @tree:   the address of the tree to which the node belongs
 * @index:  the index to the parent node
 */
static inline struct btree_node *btree_alloc(const size_t order, struct btree_node *restrict parent, struct btree_root *restrict tree, size_t index) {
  struct btree_node *node = malloc(sizeof(struct btree_node));
  node->keys              = malloc(__SIZEOF_POINTER__*(order-1));
  node->values            = malloc(__SIZEOF_POINTER__*(order-1));
  node->parent            = parent;
  node->children          = malloc(__SIZEOF_POINTER__*order);
  node->tree              = tree;
  node->index             = index;
  node->nmemb             = 0;
  return node;
}

/**
 * btree_free - deallocates @node
 *
 * @node: node to deallocate
 */
static inline void btree_free(struct btree_node *node) {
  free(node->keys);
  free(node->values);
  free(node->children);
  free(node);
}

/**
 * __bsearch - do a binary search for @key in @base, which consists of @nmemb elements, using @less to perform the comparisons
 *
 * @key:   the key to search for
 * @base:  where to search @key
 * @nmemb: number of elements in @base
 * @less:  operator defining the (partial) element order
 */
static inline size_t __bsearch(const void *restrict key, const void **restrict base, const size_t nmemb, bool (*less)(const void *restrict, const void *restrict)) {
  register size_t idx;
  register size_t lo = 0;
  register size_t hi = nmemb;

  while (lo < hi) {
    idx = (lo+hi)>>1;
    if (less(key, base[idx]))      hi = idx;
    else if (less(base[idx], key)) lo = idx+1;
    else                           return idx;
  }

  return lo;
}

/**
 * btree_lower_bound - finds logical lower bound of @index entry of @node
 *
 * @node:  node to find logical lower bound of
 * @index: index of entry to find logical lower bound of
 */
static inline struct btree_node *btree_lower_bound(struct btree_node *restrict node, size_t *restrict index) {
  if (node == NULL)
    return NULL;

  if (node->children[*index] != NULL) {
    for (node = node->children[*index]; node->children[node->nmemb] != NULL; node = node->children[node->nmemb]);
    *index = node->nmemb-1;
    return node;
  }

  if (0 < *index) {
    --*index;
    return node;
  }

  while (node->parent != NULL && node->index == 0)
    node = node->parent;

  *index = node->index == 0 ? 0 : node->index-1;
  return node->parent;
}

/**
 * btree_upper_bound - finds logical upper bound of @index entry of @node
 *
 * @node:  node to find logical upper bound of
 * @index: index of entry to find logical upper bound of
 */
static inline struct btree_node *btree_upper_bound(struct btree_node *restrict node, size_t *restrict index) {
  if (node == NULL)
    return NULL;

  if (node->children[*index+1] != NULL) {
    for (node = node->children[*index+1]; node->children[0] != NULL; node = node->children[0]);
    *index = 0;
    return node;
  }

  if (*index < node->nmemb-1) {
    ++*index;
    return node;
  }

  while (node->parent != NULL && node->index == node->parent->nmemb)
    node = node->parent;

  *index = node->index;
  return node->parent;
}

/**
 * btree_mk_iter - creates an iterator from @index entry of @node
 *
 * @node:  node to create an iterator from
 * @index: index of entry to create an iterator from
 */
static inline struct btree_iter btree_mk_iter(struct btree_node *node, size_t index) {
  struct btree_iter iter = {
    .pivot = node,
    .index = node == NULL ? 0 : index,
    .key   = node == NULL ? NULL : node->keys[index],
    .value = node == NULL ? NULL : node->values[index],
  };
  return iter;
}

/**
 * btree_mk_reverse_iter - creates a reverse iterator from @index entry of @node
 *
 * @node:  node to create a reverse iterator from
 * @index: index of entry to create a reverse iterator from
 */
static inline struct btree_reverse_iter btree_mk_reverse_iter(struct btree_node *node, size_t index) {
  struct btree_reverse_iter iter = {
    .pivot = node,
    .index = node == NULL ? 0 : index,
    .key   = node == NULL ? NULL : node->keys[index],
    .value = node == NULL ? NULL : node->values[index],
  };
  return iter;
}

/**
 * btree_destroy - erases all entries in tree
 *
 * @node: root node of tree
 */
static inline void btree_destroy(struct btree_node *node) {
  register size_t            idx;
  register struct btree_node *next;

  while (node != NULL) {
    for (idx = node->nmemb; 0 < idx; --idx)
      btree_destroy(node->children[idx]);
    next = node->children[0];
    btree_free(node);
    node = next;
  }
}

extern bool btree_contains(const struct btree_root tree, const void *key) {
  register size_t            idx;
  register struct btree_node *pivot = tree.root;

  while (pivot != NULL) {
    if ((idx = __bsearch(key, pivot->keys, pivot->nmemb, tree.less)) < pivot->nmemb && !(tree.less(key, pivot->keys[idx]) || tree.less(pivot->keys[idx], key)))
      return true;
    pivot = pivot->children[idx];
  }

  return false;
}

extern struct btree_iter btree_find(const struct btree_root tree, const void *key) {
  register size_t            idx;
  register struct btree_node *pivot = tree.root;

  while (pivot != NULL) {
    if ((idx = __bsearch(key, pivot->keys, pivot->nmemb, tree.less)) < pivot->nmemb && !(tree.less(key, pivot->keys[idx]) || tree.less(pivot->keys[idx], key)))
      break;
    pivot = pivot->children[idx];
  }

  return btree_mk_iter(pivot, idx);
}

extern struct btree_iter btree_insert(struct btree_root *restrict tree, const void *restrict key, void *restrict value) {
  register size_t            idx;
  register struct btree_node *parent  = NULL;
  register struct btree_node *sibling = NULL;
  register struct btree_node *pivot   = tree->root;

  while (pivot != NULL) {
    if ((idx = __bsearch(key, pivot->keys, pivot->nmemb, tree->less)) < pivot->nmemb && !(tree->less(key, pivot->keys[idx]) || tree->less(pivot->keys[idx], key)))
      return btree_mk_iter(pivot, idx);
    parent = pivot;
    pivot  = pivot->children[idx];
  }

  register struct btree_iter iter = btree_mk_iter(NULL, 0);

  ++tree->size;

  while (parent != NULL) {
    pivot = parent;

    if (pivot->nmemb < tree->order-1) {
      memmove(pivot->keys+idx+1, pivot->keys+idx, __SIZEOF_POINTER__*(pivot->nmemb-idx));
      memmove(pivot->values+idx+1, pivot->values+idx, __SIZEOF_POINTER__*(pivot->nmemb-idx));
      memmove(pivot->children+idx+2, pivot->children+idx+1, __SIZEOF_POINTER__*(pivot->nmemb++-idx));
      pivot->keys[idx]       = key;
      pivot->values[idx]     = value;
      pivot->children[idx+1] = sibling;
      iter                   = iter.pivot == NULL ? btree_mk_iter(pivot, idx) : iter;
      for (idx += 2; idx <= pivot->nmemb; ++idx)
        if (pivot->children[idx] != NULL)
          pivot->children[idx]->index = idx;
      return iter;
    }

    parent = btree_alloc(tree->order+1, NULL, NULL, 0);
    memcpy(parent->keys, pivot->keys, __SIZEOF_POINTER__*idx);
    memcpy(parent->keys+idx+1, pivot->keys+idx, __SIZEOF_POINTER__*(pivot->nmemb-idx));
    memcpy(parent->values, pivot->values, __SIZEOF_POINTER__*idx);
    memcpy(parent->values+idx+1, pivot->values+idx, __SIZEOF_POINTER__*(pivot->nmemb-idx));
    memcpy(parent->children, pivot->children, __SIZEOF_POINTER__*(idx+1));
    memcpy(parent->children+idx+2, pivot->children+idx+1, __SIZEOF_POINTER__*(pivot->nmemb-idx));
    parent->keys[idx]       = key;
    parent->values[idx]     = value;
    parent->children[idx+1] = sibling;

    sibling        = btree_alloc(tree->order, pivot->parent, tree, pivot->index+1);
    sibling->nmemb = (tree->order-1)>>1;
    pivot->nmemb   = tree->order>>1;
    memcpy(pivot->keys, parent->keys, __SIZEOF_POINTER__*pivot->nmemb);
    memcpy(sibling->keys, parent->keys+pivot->nmemb+1, __SIZEOF_POINTER__*sibling->nmemb);
    memcpy(pivot->values, parent->values, __SIZEOF_POINTER__*pivot->nmemb);
    memcpy(sibling->values, parent->values+pivot->nmemb+1, __SIZEOF_POINTER__*sibling->nmemb);
    memcpy(pivot->children, parent->children, __SIZEOF_POINTER__*(pivot->nmemb+1));
    memcpy(sibling->children, parent->children+pivot->nmemb+1, __SIZEOF_POINTER__*(sibling->nmemb+1));
    key   = parent->keys[pivot->nmemb];
    value = parent->values[pivot->nmemb];
    iter  = iter.pivot != NULL  ? iter
          : idx == pivot->nmemb ? iter
          : idx < pivot->nmemb  ? btree_mk_iter(pivot, idx)
                                : btree_mk_iter(sibling, idx-pivot->nmemb-1);
    for (idx += 2; idx <= pivot->nmemb; ++idx)
      if (pivot->children[idx] != NULL)
        pivot->children[idx]->index = idx;
    for (idx = 0; idx <= sibling->nmemb; ++idx)
      if (sibling->children[idx] != NULL) {
        sibling->children[idx]->parent = sibling;
        sibling->children[idx]->index  = idx;
      }
    btree_free(parent);

    idx    = pivot->index;
    parent = pivot->parent;
  }

  tree->root              = btree_alloc(tree->order, NULL, tree, 0);
  tree->root->keys[0]     = key;
  tree->root->values[0]   = value;
  tree->root->children[0] = pivot;
  tree->root->children[1] = sibling;
  tree->root->nmemb       = 1;

  if (pivot != NULL) {
    pivot->parent   = tree->root;
    sibling->parent = tree->root;
  }

  return iter.pivot == NULL ? btree_mk_iter(tree->root, 0) : iter;
}

extern void *btree_erase(struct btree_root *restrict tree, const void *restrict key) {
  register size_t            idx;
  register struct btree_node *parent;
  register struct btree_node *sibling;
  register struct btree_node *pivot = tree->root;

  while (pivot != NULL) {
    if ((idx = __bsearch(key, pivot->keys, pivot->nmemb, tree->less)) < pivot->nmemb && !(tree->less(key, pivot->keys[idx]) || tree->less(pivot->keys[idx], key)))
      break;
    pivot = pivot->children[idx];
  }

  if (pivot == NULL)
    return NULL;

  void *erased = pivot->values[idx];

  if (pivot->children[idx] != NULL) {
    parent = pivot;
    for (pivot = pivot->children[idx]; pivot->children[pivot->nmemb] != NULL; pivot = pivot->children[pivot->nmemb]);
    parent->keys[idx]   = pivot->keys[pivot->nmemb-1];
    parent->values[idx] = pivot->values[pivot->nmemb-1];
    idx                 = pivot->nmemb-1;
  }

  memmove(pivot->keys+idx, pivot->keys+idx+1, __SIZEOF_POINTER__*(--pivot->nmemb-idx));
  memmove(pivot->values+idx, pivot->values+idx+1, __SIZEOF_POINTER__*(pivot->nmemb-idx));

  --tree->size;

  idx    = pivot->index;
  parent = pivot->parent;

  while (parent != NULL) {
    if ((tree->order-1)>>1 <= pivot->nmemb)
      return erased;

    sibling = idx == 0                                                        ? parent->children[1]
            : idx == parent->nmemb                                            ? parent->children[idx-1]
            : parent->children[idx-1]->nmemb < parent->children[idx+1]->nmemb ? parent->children[idx+1]
                                                                              : parent->children[idx-1];
    if ((tree->order-1)>>1 < sibling->nmemb) { /* case of key redistribution */
      if (sibling->index < idx) {
        memmove(pivot->keys+1, pivot->keys, __SIZEOF_POINTER__*pivot->nmemb);
        memmove(pivot->values+1, pivot->values, __SIZEOF_POINTER__*pivot->nmemb);
        memmove(pivot->children+1, pivot->children, __SIZEOF_POINTER__*++pivot->nmemb);
        pivot->keys[0]        = parent->keys[idx-1];
        pivot->values[0]      = parent->values[idx-1];
        pivot->children[0]    = sibling->children[sibling->nmemb];
        parent->keys[idx-1]   = sibling->keys[--sibling->nmemb];
        parent->values[idx-1] = sibling->values[sibling->nmemb];
        if (pivot->children[0] != NULL)
          pivot->children[0]->parent = pivot;
        for (idx = 0; idx <= pivot->nmemb; ++idx)
          if (pivot->children[idx] != NULL)
            pivot->children[idx]->index = idx;
      } else {
        pivot->keys[pivot->nmemb]       = parent->keys[idx];
        pivot->values[pivot->nmemb]     = parent->values[idx];
        pivot->children[++pivot->nmemb] = sibling->children[0];
        parent->keys[idx]               = sibling->keys[0];
        parent->values[idx]             = sibling->values[0];
        memmove(sibling->children, sibling->children+1, __SIZEOF_POINTER__*sibling->nmemb);
        memmove(sibling->keys, sibling->keys+1, __SIZEOF_POINTER__*--sibling->nmemb);
        memmove(sibling->values, sibling->values+1, __SIZEOF_POINTER__*sibling->nmemb);
        for (idx = 0; idx <= sibling->nmemb; ++idx)
          if (sibling->children[idx] != NULL)
            sibling->children[idx]->index = idx;
        if (pivot->children[pivot->nmemb] != NULL) {
          pivot->children[pivot->nmemb]->parent = pivot;
          pivot->children[pivot->nmemb]->index  = pivot->nmemb;
        }
      }
      return erased;
    }

    if (sibling->index < idx) {                /* case of node merge */
      sibling->keys[sibling->nmemb]   = parent->keys[idx-1];
      sibling->values[sibling->nmemb] = parent->values[idx-1];
      memcpy(sibling->keys+(++sibling->nmemb), pivot->keys, __SIZEOF_POINTER__*pivot->nmemb);
      memcpy(sibling->values+sibling->nmemb, pivot->values, __SIZEOF_POINTER__*pivot->nmemb);
      memcpy(sibling->children+sibling->nmemb, pivot->children, __SIZEOF_POINTER__*(pivot->nmemb+1));
      memmove(parent->keys+idx-1, parent->keys+idx, __SIZEOF_POINTER__*(parent->nmemb-idx));
      memmove(parent->values+idx-1, parent->values+idx, __SIZEOF_POINTER__*(parent->nmemb-idx));
      memmove(parent->children+idx, parent->children+idx+1, __SIZEOF_POINTER__*(parent->nmemb---idx));
      sibling->nmemb += pivot->nmemb;
      for (idx = sibling->nmemb-pivot->nmemb; idx <= sibling->nmemb; ++idx)
        if (sibling->children[idx] != NULL) {
          sibling->children[idx]->parent = sibling;
          sibling->children[idx]->index  = idx;
        }
      for (idx = pivot->index; idx <= parent->nmemb; ++idx)
        parent->children[idx]->index = idx;
      btree_free(pivot);
    } else {
      pivot->keys[pivot->nmemb]   = parent->keys[idx];
      pivot->values[pivot->nmemb] = parent->values[idx];
      memcpy(pivot->keys+(++pivot->nmemb), sibling->keys, __SIZEOF_POINTER__*sibling->nmemb);
      memcpy(pivot->values+pivot->nmemb, sibling->values, __SIZEOF_POINTER__*sibling->nmemb);
      memcpy(pivot->children+pivot->nmemb, sibling->children, __SIZEOF_POINTER__*(sibling->nmemb+1));
      memmove(parent->keys+idx, parent->keys+idx+1, __SIZEOF_POINTER__*(--parent->nmemb-idx));
      memmove(parent->values+idx, parent->values+idx+1, __SIZEOF_POINTER__*(parent->nmemb-idx));
      memmove(parent->children+idx+1, parent->children+idx+2, __SIZEOF_POINTER__*(parent->nmemb-idx));
      pivot->nmemb += sibling->nmemb;
      for (idx = pivot->nmemb-sibling->nmemb; idx <= pivot->nmemb; ++idx)
        if (pivot->children[idx] != NULL) {
          pivot->children[idx]->parent = pivot;
          pivot->children[idx]->index  = idx;
        }
      for (idx = pivot->index+1; idx <= parent->nmemb; ++idx)
        parent->children[idx]->index = idx;
      btree_free(sibling);
    }
    pivot  = parent;
    idx    = pivot->index;
    parent = pivot->parent;
  }

  if (pivot->nmemb == 0) {
    tree->root = pivot->children[0];
    if (tree->root != NULL)
      tree->root->parent = NULL;
    btree_free(pivot);
  }

  return erased;
}

extern void btree_clear(struct btree_root *tree) {
  btree_destroy(tree->root);
  tree->root = NULL;
  tree->size = 0;
}

extern struct btree_iter btree_iter_init(const struct btree_root tree) {
  register struct btree_node *pivot = tree.root;

  if (pivot != NULL)
    while (pivot->children[0] != NULL)
      pivot = pivot->children[0];

  return btree_mk_iter(pivot, 0);
}

extern void btree_iter_prev(struct btree_iter *iter) {
  iter->pivot = btree_lower_bound(iter->pivot, &iter->index);
  iter->key   = iter->pivot == NULL ? NULL : iter->pivot->keys[iter->index];
  iter->value = iter->pivot == NULL ? NULL : iter->pivot->values[iter->index];
}

extern void btree_iter_next(struct btree_iter *iter) {
  iter->pivot = btree_upper_bound(iter->pivot, &iter->index);
  iter->key   = iter->pivot == NULL ? NULL : iter->pivot->keys[iter->index];
  iter->value = iter->pivot == NULL ? NULL : iter->pivot->values[iter->index];
}

extern struct btree_reverse_iter btree_reverse_iter_init(const struct btree_root tree) {
  register struct btree_node *pivot = tree.root;

  if (pivot != NULL)
    while (pivot->children[pivot->nmemb] != NULL)
      pivot = pivot->children[pivot->nmemb];

  return btree_mk_reverse_iter(pivot, pivot->nmemb-1);
}

extern void btree_reverse_iter_prev(struct btree_reverse_iter *iter) {
  iter->pivot = btree_upper_bound(iter->pivot, &iter->index);
  iter->key   = iter->pivot == NULL ? NULL : iter->pivot->keys[iter->index];
  iter->value = iter->pivot == NULL ? NULL : iter->pivot->values[iter->index];
}

extern void btree_reverse_iter_next(struct btree_reverse_iter *iter) {
  iter->pivot = btree_lower_bound(iter->pivot, &iter->index);
  iter->key   = iter->pivot == NULL ? NULL : iter->pivot->keys[iter->index];
  iter->value = iter->pivot == NULL ? NULL : iter->pivot->values[iter->index];
}
