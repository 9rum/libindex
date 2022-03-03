/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2020-2022 9rum
 *
 * btree.c - generic B-tree definition
 */
#include <index/btree.h>
#include <index/stack.h>
#include <string.h>

/**
 * btree_alloc - allocates a node
 *
 * @order: the order of tree
 */
static inline struct btree_node *btree_alloc(const size_t order) {
  struct btree_node *node = malloc(sizeof(struct btree_node));
  node->keys              = malloc(__SIZEOF_POINTER__*(order-1));
  node->values            = malloc(__SIZEOF_POINTER__*(order-1));
  node->children          = malloc(__SIZEOF_POINTER__*order);
  node->nmemb             = 0;
  return node;
}

/**
 * btree_free - deallocates @node
 *
 * @node: node to deallocate
 */
static inline void btree_free(struct btree_node *restrict node) {
  free(node->keys);
  free(node->values);
  free(node->children);
  free(node);
}

/**
 * __bsearch - do a binary search for @key in @base, which consists of @nmemb elements, using @less to perform the comparisons
 *
 * @key:   the key to search
 * @base:  where to search @key
 * @nmemb: number of elements in @base
 * @less:  operator defining the (partial) element order
 */
static inline size_t __bsearch(const void *restrict key, const void **restrict base, const size_t nmemb, bool (*less)(const void *restrict, const void *restrict)) {
  register size_t idx;
  register size_t lo = 0;
  register size_t hi = nmemb;

  while (lo < hi) {
    idx = (lo + hi) / 2;
    if (less(key, base[idx]))      hi = idx;
    else if (less(base[idx], key)) lo = idx+1;
    else                           return idx;
  }

  return lo;
}

static inline void __btree_clear(struct btree_node *restrict tree) {
  if (tree != NULL) {
    for (register size_t idx = 0; idx <= tree->nmemb; ++idx)
      __btree_clear(tree->children[idx]);
    btree_free(tree);
  }
}

static inline void __btree_for_each(const struct btree_node *restrict tree, void (*func)(const void *restrict, void *restrict)) {
  if (tree != NULL) {
    for (register size_t idx = 0; idx < tree->nmemb; ++idx) {
      __btree_for_each(tree->children[idx], func);
      func(tree->keys[idx], tree->values[idx]);
    }
    __btree_for_each(tree->children[tree->nmemb], func);
  }
}

static inline void __btree_rev_each(const struct btree_node *restrict tree, void (*func)(const void *restrict, void *restrict)) {
  if (tree != NULL) {
    __btree_rev_each(tree->children[tree->nmemb], func);
    for (register size_t idx = tree->nmemb-1; 0 <= idx; --idx) {
      func(tree->keys[idx], tree->values[idx]);
      __btree_rev_each(tree->children[idx], func);
    }
  }
}

extern void *btree_find(const struct btree_root tree, const void *restrict key) {
  register       size_t            idx;
  register const struct btree_node *walk = tree.root;

  while (walk != NULL) {
    if ((idx = __bsearch(key, walk->keys, walk->nmemb, tree.less)) < walk->nmemb &&
        !(tree.less(key, walk->keys[idx]) || tree.less(walk->keys[idx], key))) return walk->values[idx];
    walk = walk->children[idx];
  }

  return NULL;
}

extern bool btree_contains(const struct btree_root tree, const void *restrict key) {
  register       size_t            idx;
  register const struct btree_node *walk = tree.root;

  while (walk != NULL) {
    if ((idx = __bsearch(key, walk->keys, walk->nmemb, tree.less)) < walk->nmemb &&
        !(tree.less(key, walk->keys[idx]) || tree.less(walk->keys[idx], key))) return true;
    walk = walk->children[idx];
  }

  return false;
}

extern struct btree_node *btree_insert(struct btree_root *restrict tree, const void *restrict key, void *restrict value) {
  register size_t            idx;
  register struct btree_node *tmp;
  register struct btree_node *walk    = tree->root;
  register struct btree_node *sibling = NULL;
           struct stack      *stack   = NULL;

  while (walk != NULL) {
    if ((idx = __bsearch(key, walk->keys, walk->nmemb, tree->less)) < walk->nmemb &&
        !(tree->less(key, walk->keys[idx]) || tree->less(walk->keys[idx], key))) { stack_clear(&stack); return NULL; }
    stack_push(&stack, walk);
    stack_push(&stack, (void *)idx);
    walk = walk->children[idx];
  }

  register struct btree_node *node = NULL;

  ++tree->size;

  while (!stack_empty(stack)) {
    idx  = (size_t)stack_pop(&stack);
    walk = stack_pop(&stack);

    if (walk->nmemb < tree->order-1) {
      memcpy(&walk->keys[idx+1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
      memcpy(&walk->values[idx+1], &walk->values[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
      memcpy(&walk->children[idx+2], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb++-idx));
      walk->keys[idx]       = key;
      walk->values[idx]     = value;
      walk->children[idx+1] = sibling;
      stack_clear(&stack);
      return node == NULL ? walk : node;
    }

    tmp = btree_alloc(tree->order+1);
    memcpy(tmp->keys, walk->keys, __SIZEOF_POINTER__*idx);
    memcpy(&tmp->keys[idx+1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
    memcpy(tmp->values, walk->values, __SIZEOF_POINTER__*idx);
    memcpy(&tmp->values[idx+1], &walk->values[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
    memcpy(tmp->children, walk->children, __SIZEOF_POINTER__*(idx+1));
    memcpy(&tmp->children[idx+2], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb-idx));
    tmp->keys[idx]       = key;
    tmp->values[idx]     = value;
    tmp->children[idx+1] = sibling;

    sibling        = btree_alloc(tree->order);
    sibling->nmemb = (tree->order-1)>>1;
    walk->nmemb    = tree->order>>1;
    node           = node != NULL       ? node
                   : idx == walk->nmemb ? node
                   : idx < walk->nmemb  ? walk
                                        : sibling;
    memcpy(walk->keys, tmp->keys, __SIZEOF_POINTER__*walk->nmemb);
    memcpy(sibling->keys, &tmp->keys[walk->nmemb+1], __SIZEOF_POINTER__*sibling->nmemb);
    memcpy(walk->values, tmp->values, __SIZEOF_POINTER__*walk->nmemb);
    memcpy(sibling->values, &tmp->values[walk->nmemb+1], __SIZEOF_POINTER__*sibling->nmemb);
    memcpy(walk->children, tmp->children, __SIZEOF_POINTER__*(walk->nmemb+1));
    memcpy(sibling->children, &tmp->children[walk->nmemb+1], __SIZEOF_POINTER__*(sibling->nmemb+1));
    key   = tmp->keys[walk->nmemb];
    value = tmp->values[walk->nmemb];
    btree_free(tmp);
  }

  tmp              = btree_alloc(tree->order);
  tmp->keys[0]     = key;
  tmp->values[0]   = value;
  tmp->children[0] = walk;
  tmp->children[1] = sibling;
  tmp->nmemb       = 1;
  tree->root       = tmp;

  return node == NULL ? tmp : node;
}

extern struct btree_node *btree_insert_or_assign(struct btree_root *restrict tree, const void *restrict key, void *restrict value) {
  register size_t            idx;
  register struct btree_node *tmp;
  register struct btree_node *walk    = tree->root;
  register struct btree_node *sibling = NULL;
           struct stack      *stack   = NULL;

  while (walk != NULL) {
    if ((idx = __bsearch(key, walk->keys, walk->nmemb, tree->less)) < walk->nmemb &&
        !(tree->less(key, walk->keys[idx]) || tree->less(walk->keys[idx], key))) { stack_clear(&stack); walk->values[idx] = value; return walk; }
    stack_push(&stack, walk);
    stack_push(&stack, (void *)idx);
    walk = walk->children[idx];
  }

  register struct btree_node *node = NULL;

  ++tree->size;

  while (!stack_empty(stack)) {
    idx  = (size_t)stack_pop(&stack);
    walk = stack_pop(&stack);

    if (walk->nmemb < tree->order-1) {
      memcpy(&walk->keys[idx+1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
      memcpy(&walk->values[idx+1], &walk->values[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
      memcpy(&walk->children[idx+2], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb++-idx));
      walk->keys[idx]       = key;
      walk->values[idx]     = value;
      walk->children[idx+1] = sibling;
      stack_clear(&stack);
      return node == NULL ? walk : node;
    }

    tmp = btree_alloc(tree->order+1);
    memcpy(tmp->keys, walk->keys, __SIZEOF_POINTER__*idx);
    memcpy(&tmp->keys[idx+1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
    memcpy(tmp->values, walk->values, __SIZEOF_POINTER__*idx);
    memcpy(&tmp->values[idx+1], &walk->values[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
    memcpy(tmp->children, walk->children, __SIZEOF_POINTER__*(idx+1));
    memcpy(&tmp->children[idx+2], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb-idx));
    tmp->keys[idx]       = key;
    tmp->values[idx]     = value;
    tmp->children[idx+1] = sibling;

    sibling        = btree_alloc(tree->order);
    sibling->nmemb = (tree->order-1)>>1;
    walk->nmemb    = tree->order>>1;
    node           = node != NULL       ? node
                   : idx == walk->nmemb ? node
                   : idx < walk->nmemb  ? walk
                                        : sibling;
    memcpy(walk->keys, tmp->keys, __SIZEOF_POINTER__*walk->nmemb);
    memcpy(sibling->keys, &tmp->keys[walk->nmemb+1], __SIZEOF_POINTER__*sibling->nmemb);
    memcpy(walk->values, tmp->values, __SIZEOF_POINTER__*walk->nmemb);
    memcpy(sibling->values, &tmp->values[walk->nmemb+1], __SIZEOF_POINTER__*sibling->nmemb);
    memcpy(walk->children, tmp->children, __SIZEOF_POINTER__*(walk->nmemb+1));
    memcpy(sibling->children, &tmp->children[walk->nmemb+1], __SIZEOF_POINTER__*(sibling->nmemb+1));
    key   = tmp->keys[walk->nmemb];
    value = tmp->values[walk->nmemb];
    btree_free(tmp);
  }

  tmp              = btree_alloc(tree->order);
  tmp->keys[0]     = key;
  tmp->values[0]   = value;
  tmp->children[0] = walk;
  tmp->children[1] = sibling;
  tmp->nmemb       = 1;
  tree->root       = tmp;

  return node == NULL ? tmp : node;
}

extern void *btree_erase(struct btree_root *restrict tree, const void *restrict key) {
  register size_t            idx;
  register struct btree_node *parent;
  register struct btree_node *sibling;
  register struct btree_node *walk  = tree->root;
           struct stack      *stack = NULL;

  while (walk != NULL) {
    if ((idx = __bsearch(key, walk->keys, walk->nmemb, tree->less)) < walk->nmemb &&
        !(tree->less(key, walk->keys[idx]) || tree->less(walk->keys[idx], key))) break;
    stack_push(&stack, walk);
    stack_push(&stack, (void *)idx);
    walk = walk->children[idx];
  }

  if (walk == NULL) { stack_clear(&stack); return NULL; }

  void *erased = walk->values[idx];

  if (walk->children[idx] != NULL) {
    parent = walk;
    stack_push(&stack, walk);
    stack_push(&stack, (void *)idx);
    for (walk = walk->children[idx]; walk->children[walk->nmemb] != NULL; walk = walk->children[walk->nmemb]) {
      stack_push(&stack, walk);
      stack_push(&stack, (void *)walk->nmemb);
    }
    parent->keys[idx]   = walk->keys[walk->nmemb-1];
    parent->values[idx] = walk->values[walk->nmemb-1];
    idx                 = walk->nmemb-1;
  }

  memcpy(&walk->keys[idx], &walk->keys[idx+1], __SIZEOF_POINTER__*(--walk->nmemb-idx));
  memcpy(&walk->values[idx], &walk->values[idx+1], __SIZEOF_POINTER__*(walk->nmemb-idx));
  --tree->size;

  while (!stack_empty(stack)) {
    if ((tree->order-1)>>1 <= walk->nmemb) { stack_clear(&stack); return erased; }

    idx     = (size_t)stack_pop(&stack);
    parent  = stack_pop(&stack);
    sibling = idx == 0                                                        ? parent->children[1]
            : idx == parent->nmemb                                            ? parent->children[idx-1]
            : parent->children[idx-1]->nmemb < parent->children[idx+1]->nmemb ? parent->children[idx+1]
                                                                              : parent->children[idx-1];
    if ((tree->order-1)>>1 < sibling->nmemb) {           /* case of key redistribution */
      if (0 < idx && sibling == parent->children[idx-1]) {
        memcpy(&walk->keys[1], walk->keys, __SIZEOF_POINTER__*walk->nmemb);
        memcpy(&walk->values[1], walk->values, __SIZEOF_POINTER__*walk->nmemb);
        memcpy(&walk->children[1], walk->children, __SIZEOF_POINTER__*++walk->nmemb);
        walk->keys[0]         = parent->keys[idx-1];
        walk->values[0]       = parent->values[idx-1];
        walk->children[0]     = sibling->children[sibling->nmemb];
        parent->keys[idx-1]   = sibling->keys[--sibling->nmemb];
        parent->values[idx-1] = sibling->values[sibling->nmemb];
      } else {
        walk->keys[walk->nmemb]       = parent->keys[idx];
        walk->values[walk->nmemb]     = parent->values[idx];
        walk->children[++walk->nmemb] = sibling->children[0];
        parent->keys[idx]             = sibling->keys[0];
        parent->values[idx]           = sibling->values[0];
        memcpy(sibling->children, &sibling->children[1], __SIZEOF_POINTER__*sibling->nmemb);
        memcpy(sibling->keys, &sibling->keys[1], __SIZEOF_POINTER__*--sibling->nmemb);
        memcpy(sibling->values, &sibling->values[1], __SIZEOF_POINTER__*sibling->nmemb);
      }
      stack_clear(&stack);
      return erased;
    }

    if (0 < idx && sibling == parent->children[idx-1]) { /* case of node merge */
      sibling->keys[sibling->nmemb]   = parent->keys[idx-1];
      sibling->values[sibling->nmemb] = parent->values[idx-1];
      memcpy(&sibling->keys[++sibling->nmemb], walk->keys, __SIZEOF_POINTER__*walk->nmemb);
      memcpy(&sibling->values[sibling->nmemb], walk->values, __SIZEOF_POINTER__*walk->nmemb);
      memcpy(&sibling->children[sibling->nmemb], walk->children, __SIZEOF_POINTER__*(walk->nmemb+1));
      memcpy(&parent->keys[idx-1], &parent->keys[idx], __SIZEOF_POINTER__*(parent->nmemb-idx));
      memcpy(&parent->values[idx-1], &parent->values[idx], __SIZEOF_POINTER__*(parent->nmemb-idx));
      memcpy(&parent->children[idx], &parent->children[idx+1], __SIZEOF_POINTER__*(parent->nmemb---idx));
      sibling->nmemb += walk->nmemb;
      btree_free(walk);
    } else {
      walk->keys[walk->nmemb]   = parent->keys[idx];
      walk->values[walk->nmemb] = parent->values[idx];
      memcpy(&walk->keys[++walk->nmemb], sibling->keys, __SIZEOF_POINTER__*sibling->nmemb);
      memcpy(&walk->values[walk->nmemb], sibling->values, __SIZEOF_POINTER__*sibling->nmemb);
      memcpy(&walk->children[walk->nmemb], sibling->children, __SIZEOF_POINTER__*(sibling->nmemb+1));
      memcpy(&parent->keys[idx], &parent->keys[idx+1], __SIZEOF_POINTER__*(--parent->nmemb-idx));
      memcpy(&parent->values[idx], &parent->values[idx+1], __SIZEOF_POINTER__*(parent->nmemb-idx));
      memcpy(&parent->children[idx+1], &parent->children[idx+2], __SIZEOF_POINTER__*(parent->nmemb-idx));
      walk->nmemb += sibling->nmemb;
      btree_free(sibling);
    }
    walk = parent;
  }

  if (walk->nmemb == 0) { tree->root = walk->children[0]; btree_free(walk); }

  return erased;
}

extern void btree_clear(struct btree_root *restrict tree) {
  __btree_clear(tree->root);
  tree->root = NULL;
  tree->size = 0;
}

extern void btree_for_each(const struct btree_root tree, void (*func)(const void *restrict, void *restrict)) { __btree_for_each(tree.root, func); }

extern void btree_rev_each(const struct btree_root tree, void (*func)(const void *restrict, void *restrict)) { __btree_rev_each(tree.root, func); }
