/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright (C) 2020-2022 9rum
 *
 * bplustree.c - generic B+-tree definition
 */
#include <index/bplustree.h>
#include <index/stack.h>
#include <string.h>

/**
 * bplus_internal_alloc - allocates an internal node
 *
 * @order: the order of tree
 */
static inline struct bplus_internal_node *bplus_internal_alloc(const size_t order) {
  struct bplus_internal_node *node = malloc(sizeof(struct bplus_internal_node));
  node->keys                       = malloc(__SIZEOF_POINTER__*(order-1));
  node->children                   = malloc(__SIZEOF_POINTER__*order);
  node->nmemb                      = 0;
  node->type                       = false;
  return node;
}

/**
 * bplus_internal_free - deallocates @node
 *
 * @node: node to deallocate
 */
static inline void bplus_internal_free(struct bplus_internal_node *restrict node) {
  free(node->keys);
  free(node->children);
  free(node);
}

/**
 * bplus_internal_clear - clears @tree
 *
 * @tree: tree to clear
 */
static inline void bplus_internal_clear(struct bplus_internal_node *restrict tree) {
  if (tree != NULL) {
    if (!tree->type)
      for (size_t idx = 0; idx <= tree->nmemb; ++idx)
        bplus_internal_clear(tree->children[idx]);
    bplus_internal_free(tree);
  }
}

/**
 * bplus_external_alloc - allocates an external node
 *
 * @order: the order of tree
 */
static inline struct bplus_external_node *bplus_external_alloc(const size_t order) {
  struct bplus_external_node *node = malloc(sizeof(struct bplus_external_node));
  node->keys                       = malloc(__SIZEOF_POINTER__*order);
  node->values                     = malloc(__SIZEOF_POINTER__*order);
  node->next                       = NULL;
  node->nmemb                      = 0;
  return node;
}

/**
 * bplus_external_free - deallocates @node
 *
 * @node: node to deallocate
 */
static inline void bplus_external_free(struct bplus_external_node *restrict node) {
  free(node->keys);
  free(node->values);
  free(node);
}

/**
 * bplus_external_clear - clears @list
 *
 * @list: list to clear
 */
static inline void bplus_external_clear(struct bplus_external_node *restrict list) {
  register struct bplus_external_node *node;

  while (list != NULL) {
    node = list;
    list = list->next;
    free(node);
  }
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

extern void *bplus_find(const struct bplus_root tree, const void *restrict key) {
  register       size_t                     idx;
  register const struct bplus_internal_node *walk = tree.root;
           const struct bplus_external_node *node = tree.head;

  while (walk != NULL) {
    idx = __bsearch(key, walk->keys, walk->nmemb, tree.less);
    if (walk->type) node = walk->children[idx], walk = NULL;
    else            walk = walk->children[idx];
  }

  if (node == NULL) return NULL;

  if ((idx = __bsearch(key, node->keys, node->nmemb, tree.less)) < node->nmemb &&
      !(tree.less(key, node->keys[idx]) || tree.less(node->keys[idx], key))) return node->values[idx];

  return NULL;
}

extern struct bplus_external_node *bplus_insert(struct bplus_root *restrict tree, const void *restrict key, void *restrict value) {
  register size_t                     idx;
  register struct bplus_internal_node *tmp;
  register struct bplus_internal_node *sibling;
  register struct bplus_internal_node *walk  = tree->root;
           struct bplus_external_node *node  = tree->head;
           struct stack               *stack = NULL;

  while (walk != NULL) {
    idx = __bsearch(key, walk->keys, walk->nmemb, tree->less);
    stack_push(&stack, walk);
    stack_push(&stack, (void *)idx);
    if (walk->type) node = walk->children[idx], walk = NULL;
    else            walk = walk->children[idx];
  }

  if (node == NULL) {
    node            = bplus_external_alloc(tree->order);
    node->keys[0]   = key;
    node->values[0] = value;
    node->nmemb     = 1;
    tree->head      = node;
    tree->size      = 1;

    return node;
  }

  if ((idx = __bsearch(key, node->keys, node->nmemb, tree->less)) < node->nmemb &&
      !(tree->less(key, node->keys[idx]) || tree->less(node->keys[idx], key))) { stack_clear(&stack); return NULL; }

  ++tree->size;

  if (node->nmemb < tree->order) {
    memcpy(&node->keys[idx+1], &node->keys[idx], __SIZEOF_POINTER__*(node->nmemb-idx));
    memcpy(&node->values[idx+1], &node->values[idx], __SIZEOF_POINTER__*(node->nmemb++-idx));
    node->keys[idx]   = key;
    node->values[idx] = value;
    stack_clear(&stack);
    return node;
  }

  struct bplus_external_node *temp = bplus_external_alloc(tree->order+1);
  memcpy(temp->keys, node->keys, __SIZEOF_POINTER__*idx);
  memcpy(&temp->keys[idx+1], &node->keys[idx], __SIZEOF_POINTER__*(tree->order-idx));
  memcpy(temp->values, node->values, __SIZEOF_POINTER__*idx);
  memcpy(&temp->values[idx+1], &node->values[idx], __SIZEOF_POINTER__*(tree->order-idx));
  temp->keys[idx]   = key;
  temp->values[idx] = value;

  struct bplus_external_node *sib = bplus_external_alloc(tree->order);
  sib->nmemb                      = (tree->order+1)>>1;
  node->nmemb                     = (tree->order>>1)+1;
  memcpy(node->keys, temp->keys, __SIZEOF_POINTER__*node->nmemb);
  memcpy(sib->keys, &temp->keys[node->nmemb], __SIZEOF_POINTER__*sib->nmemb);
  memcpy(node->values, temp->values, __SIZEOF_POINTER__*node->nmemb);
  memcpy(sib->values, &temp->values[node->nmemb], __SIZEOF_POINTER__*sib->nmemb);
  sib->next  = node->next;
  node->next = sib;
  key        = node->keys[node->nmemb-1];
  bplus_external_free(temp);

  if (stack_empty(stack)) {
    tmp              = bplus_internal_alloc(tree->order);
    tmp->keys[0]     = key;
    tmp->children[0] = node;
    tmp->children[1] = sib;
    tmp->nmemb       = 1;
    tmp->type        = true;
    tree->root       = tmp;

    return idx < node->nmemb ? node : sib;
  }

  node = idx < node->nmemb ? node : sib;
  idx  = (size_t)stack_pop(&stack);
  walk = stack_pop(&stack);

  if (walk->nmemb < tree->order-1) {
    memcpy(&walk->keys[idx+1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
    memcpy(&walk->children[idx+2], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb++-idx));
    walk->keys[idx]       = key;
    walk->children[idx+1] = sib;
    stack_clear(&stack);
    return node;
  }

  tmp = bplus_internal_alloc(tree->order+1);
  memcpy(tmp->keys, walk->keys, __SIZEOF_POINTER__*idx);
  memcpy(&tmp->keys[idx+1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
  memcpy(tmp->children, walk->children, __SIZEOF_POINTER__*(idx+1));
  memcpy(&tmp->children[idx+2], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb-idx));
  tmp->keys[idx]       = key;
  tmp->children[idx+1] = sib;

  sibling        = bplus_internal_alloc(tree->order);
  sibling->type  = true;
  sibling->nmemb = (tree->order-1)>>1;
  walk->nmemb    = tree->order>>1;
  memcpy(walk->keys, tmp->keys, __SIZEOF_POINTER__*walk->nmemb);
  memcpy(sibling->keys, &tmp->keys[walk->nmemb+1], __SIZEOF_POINTER__*sibling->nmemb);
  memcpy(walk->children, tmp->children, __SIZEOF_POINTER__*(walk->nmemb+1));
  memcpy(sibling->children, &tmp->children[walk->nmemb+1], __SIZEOF_POINTER__*(sibling->nmemb+1));
  key = tmp->keys[walk->nmemb];
  bplus_internal_free(tmp);

  while (!stack_empty(stack)) {
    idx  = (size_t)stack_pop(&stack);
    walk = stack_pop(&stack);

    if (walk->nmemb < tree->order-1) {
      memcpy(&walk->keys[idx+1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
      memcpy(&walk->children[idx+2], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb++-idx));
      walk->keys[idx]       = key;
      walk->children[idx+1] = sibling;
      stack_clear(&stack);
      return node;
    }

    tmp = bplus_internal_alloc(tree->order+1);
    memcpy(tmp->keys, walk->keys, __SIZEOF_POINTER__*idx);
    memcpy(&tmp->keys[idx+1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
    memcpy(tmp->children, walk->children, __SIZEOF_POINTER__*(idx+1));
    memcpy(&tmp->children[idx+2], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb-idx));
    tmp->keys[idx]       = key;
    tmp->children[idx+1] = sibling;

    sibling        = bplus_internal_alloc(tree->order);
    sibling->nmemb = (tree->order-1)>>1;
    walk->nmemb    = tree->order>>1;
    memcpy(walk->keys, tmp->keys, __SIZEOF_POINTER__*walk->nmemb);
    memcpy(sibling->keys, &tmp->keys[walk->nmemb+1], __SIZEOF_POINTER__*sibling->nmemb);
    memcpy(walk->children, tmp->children, __SIZEOF_POINTER__*(walk->nmemb+1));
    memcpy(sibling->children, &tmp->children[walk->nmemb+1], __SIZEOF_POINTER__*(sibling->nmemb+1));
    key = tmp->keys[walk->nmemb];
    bplus_internal_free(tmp);
  }

  tmp              = bplus_internal_alloc(tree->order);
  tmp->keys[0]     = key;
  tmp->children[0] = walk;
  tmp->children[1] = sibling;
  tmp->nmemb       = 1;
  tree->root       = tmp;

  return node;
}

extern struct bplus_external_node *bplus_insert_or_assign(struct bplus_root *restrict tree, const void *restrict key, void *restrict value) {
  register size_t                     idx;
  register struct bplus_internal_node *tmp;
  register struct bplus_internal_node *sibling;
  register struct bplus_internal_node *walk  = tree->root;
           struct bplus_external_node *node  = tree->head;
           struct stack               *stack = NULL;

  while (walk != NULL) {
    idx = __bsearch(key, walk->keys, walk->nmemb, tree->less);
    stack_push(&stack, walk);
    stack_push(&stack, (void *)idx);
    if (walk->type) node = walk->children[idx], walk = NULL;
    else            walk = walk->children[idx];
  }

  if (node == NULL) {
    node            = bplus_external_alloc(tree->order);
    node->keys[0]   = key;
    node->values[0] = value;
    node->nmemb     = 1;
    tree->head      = node;
    tree->size      = 1;

    return node;
  }

  if ((idx = __bsearch(key, node->keys, node->nmemb, tree->less)) < node->nmemb &&
      !(tree->less(key, node->keys[idx]) || tree->less(node->keys[idx], key))) { stack_clear(&stack); node->values[idx] = value; return node; }

  ++tree->size;

  if (node->nmemb < tree->order) {
    memcpy(&node->keys[idx+1], &node->keys[idx], __SIZEOF_POINTER__*(node->nmemb-idx));
    memcpy(&node->values[idx+1], &node->values[idx], __SIZEOF_POINTER__*(node->nmemb++-idx));
    node->keys[idx]   = key;
    node->values[idx] = value;
    stack_clear(&stack);
    return node;
  }

  struct bplus_external_node *temp = bplus_external_alloc(tree->order+1);
  memcpy(temp->keys, node->keys, __SIZEOF_POINTER__*idx);
  memcpy(&temp->keys[idx+1], &node->keys[idx], __SIZEOF_POINTER__*(tree->order-idx));
  memcpy(temp->values, node->values, __SIZEOF_POINTER__*idx);
  memcpy(&temp->values[idx+1], &node->values[idx], __SIZEOF_POINTER__*(tree->order-idx));
  temp->keys[idx]   = key;
  temp->values[idx] = value;

  struct bplus_external_node *sib = bplus_external_alloc(tree->order);
  sib->nmemb                      = (tree->order+1)>>1;
  node->nmemb                     = (tree->order>>1)+1;
  memcpy(node->keys, temp->keys, __SIZEOF_POINTER__*node->nmemb);
  memcpy(sib->keys, &temp->keys[node->nmemb], __SIZEOF_POINTER__*sib->nmemb);
  memcpy(node->values, temp->values, __SIZEOF_POINTER__*node->nmemb);
  memcpy(sib->values, &temp->values[node->nmemb], __SIZEOF_POINTER__*sib->nmemb);
  sib->next  = node->next;
  node->next = sib;
  key        = node->keys[node->nmemb-1];
  bplus_external_free(temp);

  if (stack_empty(stack)) {
    tmp              = bplus_internal_alloc(tree->order);
    tmp->keys[0]     = key;
    tmp->children[0] = node;
    tmp->children[1] = sib;
    tmp->nmemb       = 1;
    tmp->type        = true;
    tree->root       = tmp;

    return idx < node->nmemb ? node : sib;
  }

  node = idx < node->nmemb ? node : sib;
  idx  = (size_t)stack_pop(&stack);
  walk = stack_pop(&stack);

  if (walk->nmemb < tree->order-1) {
    memcpy(&walk->keys[idx+1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
    memcpy(&walk->children[idx+2], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb++-idx));
    walk->keys[idx]       = key;
    walk->children[idx+1] = sib;
    stack_clear(&stack);
    return node;
  }

  tmp = bplus_internal_alloc(tree->order+1);
  memcpy(tmp->keys, walk->keys, __SIZEOF_POINTER__*idx);
  memcpy(&tmp->keys[idx+1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
  memcpy(tmp->children, walk->children, __SIZEOF_POINTER__*(idx+1));
  memcpy(&tmp->children[idx+2], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb-idx));
  tmp->keys[idx]       = key;
  tmp->children[idx+1] = sib;

  sibling        = bplus_internal_alloc(tree->order);
  sibling->type  = true;
  sibling->nmemb = (tree->order-1)>>1;
  walk->nmemb    = tree->order>>1;
  memcpy(walk->keys, tmp->keys, __SIZEOF_POINTER__*walk->nmemb);
  memcpy(sibling->keys, &tmp->keys[walk->nmemb+1], __SIZEOF_POINTER__*sibling->nmemb);
  memcpy(walk->children, tmp->children, __SIZEOF_POINTER__*(walk->nmemb+1));
  memcpy(sibling->children, &tmp->children[walk->nmemb+1], __SIZEOF_POINTER__*(sibling->nmemb+1));
  key = tmp->keys[walk->nmemb];
  bplus_internal_free(tmp);

  while (!stack_empty(stack)) {
    idx  = (size_t)stack_pop(&stack);
    walk = stack_pop(&stack);

    if (walk->nmemb < tree->order-1) {
      memcpy(&walk->keys[idx+1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
      memcpy(&walk->children[idx+2], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb++-idx));
      walk->keys[idx]       = key;
      walk->children[idx+1] = sibling;
      stack_clear(&stack);
      return node;
    }

    tmp = bplus_internal_alloc(tree->order+1);
    memcpy(tmp->keys, walk->keys, __SIZEOF_POINTER__*idx);
    memcpy(&tmp->keys[idx+1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
    memcpy(tmp->children, walk->children, __SIZEOF_POINTER__*(idx+1));
    memcpy(&tmp->children[idx+2], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb-idx));
    tmp->keys[idx]       = key;
    tmp->children[idx+1] = sibling;

    sibling        = bplus_internal_alloc(tree->order);
    sibling->nmemb = (tree->order-1)>>1;
    walk->nmemb    = tree->order>>1;
    memcpy(walk->keys, tmp->keys, __SIZEOF_POINTER__*walk->nmemb);
    memcpy(sibling->keys, &tmp->keys[walk->nmemb+1], __SIZEOF_POINTER__*sibling->nmemb);
    memcpy(walk->children, tmp->children, __SIZEOF_POINTER__*(walk->nmemb+1));
    memcpy(sibling->children, &tmp->children[walk->nmemb+1], __SIZEOF_POINTER__*(sibling->nmemb+1));
    key = tmp->keys[walk->nmemb];
    bplus_internal_free(tmp);
  }

  tmp              = bplus_internal_alloc(tree->order);
  tmp->keys[0]     = key;
  tmp->children[0] = walk;
  tmp->children[1] = sibling;
  tmp->nmemb       = 1;
  tree->root       = tmp;

  return node;
}

extern void *bplus_erase(struct bplus_root *restrict tree, const void *restrict key) {
  register size_t                     idx;
  register struct bplus_internal_node *parent;
  register struct bplus_internal_node *sibling;
  register struct bplus_internal_node *walk  = tree->root;
           struct bplus_external_node *node  = tree->head;
           struct stack               *stack = NULL;

  while (walk != NULL) {
    idx = __bsearch(key, walk->keys, walk->nmemb, tree->less);
    stack_push(&stack, walk);
    stack_push(&stack, (void *)idx);
    if (walk->type) node = walk->children[idx], walk = NULL;
    else            walk = walk->children[idx];
  }

  if (node == NULL) return NULL;

  if ((idx = __bsearch(key, node->keys, node->nmemb, tree->less)) < node->nmemb &&
      (tree->less(key, node->keys[idx]) || tree->less(node->keys[idx], key)) || idx == node->nmemb) { stack_clear(&stack); return NULL; }

  void *erased = node->values[idx];

  memcpy(&node->keys[idx], &node->keys[idx+1], __SIZEOF_POINTER__*(--node->nmemb-idx));
  memcpy(&node->values[idx], &node->values[idx+1], __SIZEOF_POINTER__*(node->nmemb-idx));
  --tree->size;

  if ((tree->order+1)>>1 <= node->nmemb) { stack_clear(&stack); return erased; }

  if (stack_empty(stack)) { if (node->nmemb == 0) { tree->head = NULL; bplus_external_free(node); } return erased; }

  idx                             = (size_t)stack_pop(&stack);
  walk                            = stack_pop(&stack);
  struct bplus_external_node *sib = idx == 0                                                                                                                    ? walk->children[1]
                                  : idx == walk->nmemb                                                                                                          ? walk->children[idx-1]
                                  : ((struct bplus_external_node *)walk->children[idx-1])->nmemb < ((struct bplus_external_node *)walk->children[idx+1])->nmemb ? walk->children[idx+1]
                                                                                                                                                                : walk->children[idx-1];
  if ((tree->order+1)>>1 < sib->nmemb) {       /* case of key redistribution */
    if (sib == walk->children[idx-1]) {
      memcpy(&node->keys[1], node->keys, __SIZEOF_POINTER__*node->nmemb);
      memcpy(&node->values[1], node->values, __SIZEOF_POINTER__*node->nmemb++);
      node->keys[0]     = sib->keys[--sib->nmemb];
      node->values[0]   = sib->values[sib->nmemb];
      walk->keys[idx-1] = sib->keys[sib->nmemb-1];
    } else {
      walk->keys[idx]             = sib->keys[0];
      node->keys[node->nmemb]     = sib->keys[0];
      node->values[node->nmemb++] = sib->values[0];
      memcpy(sib->keys, &sib->keys[1], __SIZEOF_POINTER__*--sib->nmemb);
      memcpy(sib->values, &sib->values[1], __SIZEOF_POINTER__*sib->nmemb);
    }
    stack_clear(&stack);
    return erased;
  }

  if (sib == walk->children[idx-1]) {          /* case of external node merge */
    memcpy(&sib->keys[sib->nmemb], node->keys, __SIZEOF_POINTER__*node->nmemb);
    memcpy(&sib->values[sib->nmemb], node->values, __SIZEOF_POINTER__*node->nmemb);
    memcpy(&walk->keys[idx-1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
    memcpy(&walk->children[idx], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb---idx));
    sib->next   = node->next;
    sib->nmemb += node->nmemb;
    bplus_external_free(node);
  } else {
    memcpy(&node->keys[node->nmemb], sib->keys, __SIZEOF_POINTER__*sib->nmemb);
    memcpy(&node->values[node->nmemb], sib->values, __SIZEOF_POINTER__*sib->nmemb);
    memcpy(&walk->keys[idx], &walk->keys[idx+1], __SIZEOF_POINTER__*(--walk->nmemb-idx));
    memcpy(&walk->children[idx+1], &walk->children[idx+2], __SIZEOF_POINTER__*(walk->nmemb-idx));
    node->next   = sib->next;
    node->nmemb += sib->nmemb;
    bplus_external_free(sib);
  }

  while (!stack_empty(stack)) {
    if ((tree->order-1)>>1 <= walk->nmemb) { stack_clear(&stack); return erased; }

    idx     = (size_t)stack_pop(&stack);
    parent  = stack_pop(&stack);
    sibling = idx == 0                                                                                                                        ? parent->children[1]
            : idx == parent->nmemb                                                                                                            ? parent->children[idx-1]
            : ((struct bplus_internal_node *)parent->children[idx-1])->nmemb < ((struct bplus_internal_node *)parent->children[idx+1])->nmemb ? parent->children[idx+1]
                                                                                                                                              : parent->children[idx-1];
    if ((tree->order-1)>>1 < sibling->nmemb) { /* case of key redistribution */
      if (sibling == parent->children[idx-1]) {
        memcpy(&walk->keys[1], walk->keys, __SIZEOF_POINTER__*walk->nmemb);
        memcpy(&walk->children[1], walk->children, __SIZEOF_POINTER__*++walk->nmemb);
        walk->keys[0]       = parent->keys[idx-1];
        walk->children[0]   = sibling->children[sibling->nmemb];
        parent->keys[idx-1] = sibling->keys[--sibling->nmemb];
      } else {
        walk->keys[walk->nmemb]       = parent->keys[idx];
        walk->children[++walk->nmemb] = sibling->children[0];
        parent->keys[idx]             = sibling->keys[0];
        memcpy(sibling->children, &sibling->children[1], __SIZEOF_POINTER__*sibling->nmemb);
        memcpy(sibling->keys, &sibling->keys[1], __SIZEOF_POINTER__*--sibling->nmemb);
      }
      stack_clear(&stack);
      return erased;
    }

    if (sibling == parent->children[idx-1]) {  /* case of internal node merge */
      sibling->keys[sibling->nmemb] = parent->keys[idx-1];
      memcpy(&sibling->keys[++sibling->nmemb], walk->keys, __SIZEOF_POINTER__*walk->nmemb);
      memcpy(&sibling->children[sibling->nmemb], walk->children, __SIZEOF_POINTER__*(walk->nmemb+1));
      memcpy(&parent->keys[idx-1], &parent->keys[idx], __SIZEOF_POINTER__*(parent->nmemb-idx));
      memcpy(&parent->children[idx], &parent->children[idx+1], __SIZEOF_POINTER__*(parent->nmemb---idx));
      sibling->nmemb += walk->nmemb;
      bplus_internal_free(walk);
    } else {
      walk->keys[walk->nmemb] = parent->keys[idx];
      memcpy(&walk->keys[++walk->nmemb], sibling->keys, __SIZEOF_POINTER__*sibling->nmemb);
      memcpy(&walk->children[walk->nmemb], sibling->children, __SIZEOF_POINTER__*(sibling->nmemb+1));
      memcpy(&parent->keys[idx], &parent->keys[idx+1], __SIZEOF_POINTER__*(--parent->nmemb-idx));
      memcpy(&parent->children[idx+1], &parent->children[idx+2], __SIZEOF_POINTER__*(parent->nmemb-idx));
      walk->nmemb += sibling->nmemb;
      bplus_internal_free(sibling);
    }
    walk = parent;
  }

  if (walk->nmemb == 0) { tree->root = walk->type ? NULL : walk->children[0]; bplus_internal_free(walk); }

  return erased;
}

extern void bplus_clear(struct bplus_root *restrict tree) {
  bplus_internal_clear(tree->root);
  bplus_external_clear(tree->head);
  tree->root = NULL;
  tree->head = NULL;
  tree->size = 0;
}
