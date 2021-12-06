/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright 2020 9rum
 *
 * bplustree.h - generic B+-tree definition
 *
 * A B+-tree is an m-ary tree with large number of children per node.
 * It can be viewed as a B-tree in which each node contains only keys (not key–value pairs),
 * and to which an additional level is added at the bottom with linked leaves.
 *
 * The primary value of a B+-tree is in storing data for efficient retrieval in a block-oriented storage context
 * — in particular, filesystems.
 * This is primarily because unlike binary search trees, B+-trees have very high fanout,
 * which reduces the number of I/O operations required to find an element in the tree.
 *
 * The ReiserFS, NSS, XFS, JFS, ReFS, and BFS filesystems all use this type of tree for metadata indexing;
 * BFS also uses B+-trees for storing directories.
 * NTFS uses B+-trees for directory and security-related metadata indexing.
 * EXT4 uses extent trees for file extent indexing.
 * APFS uses B+-trees to store mappings from filesystem object IDs to their locations on disk,
 * and to store filesystem records (including directories), though these trees' leaf nodes lack sibling pointers.
 *
 * Relational database management systems such as IBM DB2, Informix, Microsoft SQL Server, Oracle 8, Sybase ASE,
 * and SQLite support this type of tree for table indices.
 * Key–value database management systems such as CouchDB and Tokyo Cabinet support this type of tree for data access.
 *
 * There is no single paper introducing the B+-tree concept.
 * Instead, the notion of maintaining all data in leaf nodes is repeatedly brought up as an interesting variant.
 * An early survey of B-trees also covering B+-trees is Douglas Comer.
 * Comer notes that the B+-tree was used in IBM's VSAM data access software and he refers to an IBM published article from 1973.
 *
 * See http://carlosproal.com/ir/papers/p121-comer.pdf for more details.
 */
#ifndef _BPLUSTREE_H
#define _BPLUSTREE_H

#include <string.h>
#include <stack.h>

/**
 * struct btree_node - an internal node in B+-tree
 *
 * @keys:     the ordered set of keys of the node
 * @children: the ordered set of children of the node
 * @nmemb:    the number of the keys of the node
 * @type:     the type of the node
 */
struct btree_node {
  const void   **keys;
        void   **children;
        size_t nmemb;
        bool   type;
} __attribute__((aligned(__SIZEOF_POINTER__)));

/**
 * btree_alloc - allocates an internal node
 *
 * @order: order of B+-tree
 */
static inline struct btree_node *btree_alloc(const size_t order) {
  struct btree_node *node = malloc(sizeof(struct btree_node));
  node->keys              = malloc(__SIZEOF_POINTER__*(order-1));
  node->children          = malloc(__SIZEOF_POINTER__*order);
  node->nmemb             = 0;
  node->type              = false;
  return node;
}

/**
 * btree_free - deallocates @node
 *
 * @node: node to deallocate
 */
static inline void btree_free(struct btree_node *restrict node) {
  free(node->keys);
  free(node->children);
  free(node);
}

/**
 * struct list_node - an external node in B+-tree
 *
 * @keys:   the ordered set of keys of the node
 * @values: the ordered set of values of the node
 * @next:   the pointer to the next node
 * @nmemb:  the number of the keys of the node
 */
struct list_node {
  const void             **keys;
        void             **values;
        struct list_node *next;
        size_t           nmemb;
} __attribute__((aligned(__SIZEOF_POINTER__)));

/**
 * list_alloc - allocates an external node
 *
 * @order: order of B+-tree
 */
static inline struct list_node *list_alloc(const size_t order) {
  struct list_node *node = malloc(sizeof(struct list_node));
  node->keys             = malloc(__SIZEOF_POINTER__*order);
  node->values           = malloc(__SIZEOF_POINTER__*order);
  node->next             = NULL;
  node->nmemb            = 0;
  return node;
}

/**
 * list_free - deallocates @node
 *
 * @node: node to deallocate
 */
static inline void list_free(struct list_node *restrict node) {
  free(node->keys);
  free(node->values);
  free(node);
}

/*
 * The below functions use the operator with 3 different
 * calling conventions. The operator denotes:
 *
 *    a < b  := less(a, b)
 *    a > b  := less(b, a)
 *    a == b := !less(a, b) && !less(b, a)
 *
 * NOTE:
 *
 * less must describe a transitive ordering:
 *  - if both less(a, b) and less(b, c) are true, then less(a, c) must be true as well
 *  - if both less(a, b) and less(b, c) are false, then less(a, c) must be false as well
 */

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
    else                              return idx;
  }

  return lo;
}

/**
 * bplus_insert - inserts @key and @value into @tree and @list
 *
 * @tree:  tree to insert @key and @value into
 * @list:  list to insert @key and @value into
 * @order: order of B+-tree
 * @key:   the key to insert
 * @value: the value to insert
 * @less:  operator defining the (partial) node order
 */
extern inline void bplus_insert(struct btree_node **restrict tree, struct list_node **restrict list, const size_t order, const void *restrict key, void *restrict value, bool (*less)(const void *restrict, const void *restrict)) {
  register size_t            idx;
  register struct btree_node *tmp;
  register struct btree_node *sibling;
  register struct btree_node *walk  = *tree;
           struct list_node  *node  = *list;
           struct stack      *stack = NULL;

  while (walk != NULL) {
    idx = __bsearch(key, walk->keys, walk->nmemb, less);
    stack_push(&stack, walk);
    stack_push(&stack, (void *)idx);
    if (walk->type) node = walk->children[idx], walk = NULL;
    else            walk = walk->children[idx];
  }

  if (node == NULL) {
    *list              = list_alloc(order);
    (*list)->keys[0]   = key;
    (*list)->values[0] = value;
    (*list)->nmemb     = 1;
    return;
  }

  if ((idx = __bsearch(key, node->keys, node->nmemb, less)) < node->nmemb &&
      !(less(key, node->keys[idx]) || less(node->keys[idx], key))) { stack_free(stack); return; }

  if (node->nmemb < order) {
    memcpy(&node->keys[idx+1], &node->keys[idx], __SIZEOF_POINTER__*(node->nmemb-idx));
    memcpy(&node->values[idx+1], &node->values[idx], __SIZEOF_POINTER__*(node->nmemb++-idx));
    node->keys[idx]   = key;
    node->values[idx] = value;
    stack_free(stack);
    return;
  }

  struct list_node *temp = list_alloc(order+1);
  memcpy(temp->keys, node->keys, __SIZEOF_POINTER__*idx);
  memcpy(&temp->keys[idx+1], &node->keys[idx], __SIZEOF_POINTER__*(order-idx));
  memcpy(temp->values, node->values, __SIZEOF_POINTER__*idx);
  memcpy(&temp->values[idx+1], &node->values[idx], __SIZEOF_POINTER__*(order-idx));
  temp->keys[idx]   = key;
  temp->values[idx] = value;

  struct list_node *sib = list_alloc(order);
  sib->nmemb            = (order+1)>>1;
  node->nmemb           = (order>>1)+1;
  memcpy(node->keys, temp->keys, __SIZEOF_POINTER__*node->nmemb);
  memcpy(sib->keys, &temp->keys[node->nmemb], __SIZEOF_POINTER__*sib->nmemb);
  memcpy(node->values, temp->values, __SIZEOF_POINTER__*node->nmemb);
  memcpy(sib->values, &temp->values[node->nmemb], __SIZEOF_POINTER__*sib->nmemb);
  sib->next  = node->next;
  node->next = sib;
  key        = node->keys[node->nmemb-1];
  list_free(temp);

  if (stack_empty(stack)) {
    *tree                = btree_alloc(order);
    (*tree)->keys[0]     = key;
    (*tree)->children[0] = node;
    (*tree)->children[1] = sib;
    (*tree)->nmemb       = 1;
    (*tree)->type        = true;
    return;
  }

  idx  = (size_t)stack_pop(&stack);
  walk = stack_pop(&stack);

  if (walk->nmemb < order-1) {
    memcpy(&walk->keys[idx+1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
    memcpy(&walk->children[idx+2], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb++-idx));
    walk->keys[idx]       = key;
    walk->children[idx+1] = sib;
    stack_free(stack);
    return;
  }

  tmp = btree_alloc(order+1);
  memcpy(tmp->keys, walk->keys, __SIZEOF_POINTER__*idx);
  memcpy(&tmp->keys[idx+1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
  memcpy(tmp->children, walk->children, __SIZEOF_POINTER__*(idx+1));
  memcpy(&tmp->children[idx+2], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb-idx));
  tmp->keys[idx]       = key;
  tmp->children[idx+1] = sib;

  sibling        = btree_alloc(order);
  sibling->type  = true;
  sibling->nmemb = (order-1)>>1;
  walk->nmemb    = order>>1;
  memcpy(walk->keys, tmp->keys, __SIZEOF_POINTER__*walk->nmemb);
  memcpy(sibling->keys, &tmp->keys[walk->nmemb+1], __SIZEOF_POINTER__*sibling->nmemb);
  memcpy(walk->children, tmp->children, __SIZEOF_POINTER__*(walk->nmemb+1));
  memcpy(sibling->children, &tmp->children[walk->nmemb+1], __SIZEOF_POINTER__*(sibling->nmemb+1));
  key = tmp->keys[walk->nmemb];
  btree_free(tmp);

  while (!stack_empty(stack)) {
    idx  = (size_t)stack_pop(&stack);
    walk = stack_pop(&stack);

    if (walk->nmemb < order-1) {
      memcpy(&walk->keys[idx+1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
      memcpy(&walk->children[idx+2], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb++-idx));
      walk->keys[idx]       = key;
      walk->children[idx+1] = sibling;
      stack_free(stack);
      return;
    }

    tmp = btree_alloc(order+1);
    memcpy(tmp->keys, walk->keys, __SIZEOF_POINTER__*idx);
    memcpy(&tmp->keys[idx+1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
    memcpy(tmp->children, walk->children, __SIZEOF_POINTER__*(idx+1));
    memcpy(&tmp->children[idx+2], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb-idx));
    tmp->keys[idx]       = key;
    tmp->children[idx+1] = sibling;

    sibling        = btree_alloc(order);
    sibling->nmemb = (order-1)>>1;
    walk->nmemb    = order>>1;
    memcpy(walk->keys, tmp->keys, __SIZEOF_POINTER__*walk->nmemb);
    memcpy(sibling->keys, &tmp->keys[walk->nmemb+1], __SIZEOF_POINTER__*sibling->nmemb);
    memcpy(walk->children, tmp->children, __SIZEOF_POINTER__*(walk->nmemb+1));
    memcpy(sibling->children, &tmp->children[walk->nmemb+1], __SIZEOF_POINTER__*(sibling->nmemb+1));
    key = tmp->keys[walk->nmemb];
    btree_free(tmp);
  }

  *tree                = btree_alloc(order);
  (*tree)->keys[0]     = key;
  (*tree)->children[0] = walk;
  (*tree)->children[1] = sibling;
  (*tree)->nmemb       = 1;
}

/**
 * bplus_erase - erases @key from @tree and @list
 *
 * @tree:  tree to erase @key from
 * @list:  list to erase @key from
 * @order: order of B+-tree
 * @key:   the key to erase
 * @less:  operator defining the (partial) node order
 */
extern inline void bplus_erase(struct btree_node **restrict tree, struct list_node **restrict list, const size_t order, const void *restrict key, bool (*less)(const void *restrict, const void *restrict)) {
  register size_t            idx;
  register struct btree_node *parent;
  register struct btree_node *sibling;
  register struct btree_node *walk  = *tree;
           struct list_node  *node  = *list;
           struct stack      *stack = NULL;

  while (walk != NULL) {
    idx = __bsearch(key, walk->keys, walk->nmemb, less);
    stack_push(&stack, walk);
    stack_push(&stack, (void *)idx);
    if (walk->type) node = walk->children[idx], walk = NULL;
    else            walk = walk->children[idx];
  }

  if (node == NULL) return;

  if ((idx = __bsearch(key, node->keys, node->nmemb, less)) < node->nmemb &&
      (less(key, node->keys[idx]) || less(node->keys[idx], key)) || idx == node->nmemb) { stack_free(stack); return; }

  memcpy(&node->keys[idx], &node->keys[idx+1], __SIZEOF_POINTER__*(--node->nmemb-idx));
  memcpy(&node->values[idx], &node->values[idx+1], __SIZEOF_POINTER__*(node->nmemb-idx));

  if ((order+1)>>1 <= node->nmemb) { stack_free(stack); return; }

  if (stack_empty(stack)) { if (node->nmemb == 0) { *list = NULL; list_free(node); } return; }

  idx                   = (size_t)stack_pop(&stack);
  walk                  = stack_pop(&stack);
  struct list_node *sib = idx == 0                                                                                                ? walk->children[1]
                        : idx == walk->nmemb                                                                                      ? walk->children[idx-1]
                        : ((struct list_node *)walk->children[idx-1])->nmemb < ((struct list_node *)walk->children[idx+1])->nmemb ? walk->children[idx+1]
                                                                                                                                  : walk->children[idx-1];
  if ((order+1)>>1 < sib->nmemb) {            /* case of key redistribution */
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
    stack_free(stack);
    return;
  }

  if (sib == walk->children[idx-1]) {         /* case of external node merge */
    memcpy(&sib->keys[sib->nmemb], node->keys, __SIZEOF_POINTER__*node->nmemb);
    memcpy(&sib->values[sib->nmemb], node->values, __SIZEOF_POINTER__*node->nmemb);
    memcpy(&walk->keys[idx-1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
    memcpy(&walk->children[idx], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb---idx));
    sib->next  = node->next;
    sib->nmemb += node->nmemb;
    list_free(node);
  } else {
    memcpy(&node->keys[node->nmemb], sib->keys, __SIZEOF_POINTER__*sib->nmemb);
    memcpy(&node->values[node->nmemb], sib->values, __SIZEOF_POINTER__*sib->nmemb);
    memcpy(&walk->keys[idx], &walk->keys[idx+1], __SIZEOF_POINTER__*(--walk->nmemb-idx));
    memcpy(&walk->children[idx+1], &walk->children[idx+2], __SIZEOF_POINTER__*(walk->nmemb-idx));
    node->next  = sib->next;
    node->nmemb += sib->nmemb;
    list_free(sib);
  }

  while (!stack_empty(stack)) {
    if ((order-1)>>1 <= walk->nmemb) { stack_free(stack); return; }

    idx     = (size_t)stack_pop(&stack);
    parent  = stack_pop(&stack);
    sibling = idx == 0                                                                                                      ? parent->children[1]
            : idx == parent->nmemb                                                                                          ? parent->children[idx-1]
            : ((struct btree_node *)parent->children[idx-1])->nmemb < ((struct btree_node *)parent->children[idx+1])->nmemb ? parent->children[idx+1]
                                                                                                                            : parent->children[idx-1];
    if ((order-1)>>1 < sibling->nmemb) {      /* case of key redistribution */
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
      stack_free(stack);
      return;
    }

    if (sibling == parent->children[idx-1]) { /* case of internal node merge */
      sibling->keys[sibling->nmemb] = parent->keys[idx-1];
      memcpy(&sibling->keys[++sibling->nmemb], walk->keys, __SIZEOF_POINTER__*walk->nmemb);
      memcpy(&sibling->children[sibling->nmemb], walk->children, __SIZEOF_POINTER__*(walk->nmemb+1));
      memcpy(&parent->keys[idx-1], &parent->keys[idx], __SIZEOF_POINTER__*(parent->nmemb-idx));
      memcpy(&parent->children[idx], &parent->children[idx+1], __SIZEOF_POINTER__*(parent->nmemb---idx));
      sibling->nmemb += walk->nmemb;
      btree_free(walk);
    } else {
      walk->keys[walk->nmemb] = parent->keys[idx];
      memcpy(&walk->keys[++walk->nmemb], sibling->keys, __SIZEOF_POINTER__*sibling->nmemb);
      memcpy(&walk->children[walk->nmemb], sibling->children, __SIZEOF_POINTER__*(sibling->nmemb+1));
      memcpy(&parent->keys[idx], &parent->keys[idx+1], __SIZEOF_POINTER__*(--parent->nmemb-idx));
      memcpy(&parent->children[idx+1], &parent->children[idx+2], __SIZEOF_POINTER__*(parent->nmemb-idx));
      walk->nmemb += sibling->nmemb;
      btree_free(sibling);
    }
    walk = parent;
  }

  if (walk->nmemb == 0) { *tree = walk->type ? NULL : walk->children[0]; btree_free(walk); }
}

/**
 * bplus_for_each - applies @func to each node of @list sequentialwise
 *
 * @list: list to apply @func to each node of
 * @func: function to apply to each node of @list
 */
extern inline void bplus_for_each(const struct list_node *restrict list, void (*func)(const void *restrict, void *restrict)) {
  for (const struct list_node *node = list; node != NULL; node = node->next)
    for (size_t idx = 0; idx < node->nmemb; ++idx)
      func(node->keys[idx], node->values[idx]);
}

#endif /* _BPLUSTREE_H */
