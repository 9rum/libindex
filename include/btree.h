/*
 * Copyright 2020 9rum
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * File Processing, 2020
 *
 * btree.h - generic B-tree definition
 *
 * A B-tree is a self-balancing tree data structure that maintains sorted data and allows searches,
 * sequential access, insertions, and deletions in logarithmic time.
 * The B-tree generalizes the binary search tree, allowing for nodes with more than two children.
 * Unlike other self-balancing binary search trees, the B-tree is well suited for storage systems
 * that read and write relatively large blocks of data, such as disks.
 * It is commonly used in databases and file systems.
 *
 * B-trees were invented by Rudolf Bayer and Edward M. McCreight while working at Boeing Research Labs,
 * for the purpose of efficiently managing index pages for large random access files.
 * The basic assumption was that indexes would be so voluminous
 * that only small chunks of the tree could fit in main memory.
 * Bayer and McCreight's paper, Organization and maintenance of large ordered indices,
 * was first circulated in July 1970 and later published in Acta Informatica.
 */
#ifndef _BTREE_H
#define _BTREE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stack.h>

/**
 * struct btree_node - a node in B-tree
 *
 * @keys:     the ordered set of keys of the node
 * @values:   the ordered set of values of the node
 * @children: the ordered set of children of the node
 * @nmemb:    the number of the keys in the node
 *
 * According to Knuth's definition, a B-tree of order m is a tree which satisfies the following properties:
 *
 *  1. Every node has at most m children
 *  2. Every non-leaf node (except root) has at least ⌈m/2⌉ children
 *  3. The root has at least two children if it is not a leaf node
 *  4. A non-leaf node with k children contains k−1 keys
 *  5. All leaves appear in the same level and carry information
 *
 * See https://infolab.usc.edu/csci585/Spring2010/den_ar/indexing.pdf
 */
struct btree_node {
  const void              **keys;
        void              **values;
        struct btree_node **children;
        size_t            nmemb;
} __attribute__((aligned(__SIZEOF_POINTER__)));

/**
 * btree_alloc - allocates a node
 *
 * @order: order of B-tree
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
static inline void btree_free(struct btree_node *node) {
  free(node->keys);
  free(node->values);
  free(node->children);
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
 * binsearch - do a binary search for @key in @base, which consists of @nmemb elements, using @less to perform the comparisons
 *
 * @key:   the key to search
 * @base:  where to search @key
 * @nmemb: number of elements in @base
 * @less:  operator defining the (partial) element order
 */
static inline size_t binsearch(const void *restrict key, const void **restrict base, const size_t nmemb, bool (*less)(const void *, const void *)) {
  register size_t idx;
  register size_t lo = 0;
  register size_t hi = nmemb;

  while (lo < hi) {
    idx = (lo + hi) / 2;
    if      (less(key, base[idx])) hi = idx;
    else if (less(base[idx], key)) lo = idx+1;
    else                              return idx;
  }

  return lo;
}

/**
 * btree_insert - inserts @key and @value into @tree
 *
 * @tree:  tree to insert @key and @value into
 * @order: order of B-tree
 * @key:   the key to insert
 * @value: the value to insert
 * @less:  operator defining the (partial) node order
 */
extern inline void btree_insert(struct btree_node **restrict tree, const size_t order, const void *restrict key, void *restrict value, bool (*less)(const void *, const void *)) {
  register size_t            idx;
  register struct btree_node *tmp;
  register struct btree_node *walk    = *tree;
  register struct btree_node *sibling = NULL;
           struct stack      *stack   = NULL;

  while (walk != NULL) {
    if  ((idx = binsearch(key, walk->keys, walk->nmemb, less)) < walk->nmemb &&
        !(less(key, walk->keys[idx]) || less(walk->keys[idx], key))) { destroy(&stack); return; }
    push(&stack, walk);
    push(&stack, (void *)idx);
    walk = walk->children[idx];
  }

  while (!empty(stack)) {
    idx  = (size_t)pop(&stack);
    walk = pop(&stack);

    if (walk->nmemb < order-1) {
      memcpy(&walk->keys[idx+1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
      memcpy(&walk->values[idx+1], &walk->values[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
      memcpy(&walk->children[idx+2], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb++-idx));
      walk->keys[idx]       = key;
      walk->values[idx]     = value;
      walk->children[idx+1] = sibling;
      destroy(&stack);
      return;
    }

    tmp = btree_alloc(order+1);
    memcpy(tmp->keys, walk->keys, __SIZEOF_POINTER__*idx);
    memcpy(&tmp->keys[idx+1], &walk->keys[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
    memcpy(tmp->values, walk->values, __SIZEOF_POINTER__*idx);
    memcpy(&tmp->values[idx+1], &walk->values[idx], __SIZEOF_POINTER__*(walk->nmemb-idx));
    memcpy(tmp->children, walk->children, __SIZEOF_POINTER__*(idx+1));
    memcpy(&tmp->children[idx+2], &walk->children[idx+1], __SIZEOF_POINTER__*(walk->nmemb-idx));
    tmp->keys[idx]       = key;
    tmp->values[idx]     = value;
    tmp->children[idx+1] = sibling;

    sibling        = btree_alloc(order);
    sibling->nmemb = (order-1)>>1;
    walk->nmemb    = order>>1;
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

  *tree                = btree_alloc(order);
  (*tree)->keys[0]     = key;
  (*tree)->values[0]   = value;
  (*tree)->children[0] = walk;
  (*tree)->children[1] = sibling;
  (*tree)->nmemb       = 1;
}

/**
 * btree_erase - erases @key from @tree
 *
 * @tree:  tree to erase @key from
 * @order: order of B-tree
 * @key:   the key to erase
 * @less:  operator defining the (partial) node order
 */
extern inline void btree_erase(struct btree_node **restrict tree, const size_t order, const void *restrict key, bool (*less)(const void *, const void *)) {
  register size_t            idx;
  register struct btree_node *parent;
  register struct btree_node *sibling;
  register struct btree_node *walk  = *tree;
           struct stack      *stack = NULL;

  while (walk != NULL) {
    if  ((idx = binsearch(key, walk->keys, walk->nmemb, less)) < walk->nmemb &&
        !(less(key, walk->keys[idx]) || less(walk->keys[idx], key))) break;
    push(&stack, walk);
    push(&stack, (void *)idx);
    walk = walk->children[idx];
  }

  if (walk == NULL) { destroy(&stack); return; }

  if (walk->children[idx] != NULL) {
    parent = walk;
    push(&stack, walk);
    push(&stack, (void *)idx);
    for (walk = walk->children[idx]; walk->children[walk->nmemb] != NULL; walk = walk->children[walk->nmemb]) {
      push(&stack, walk);
      push(&stack, (void *)walk->nmemb);
    }
    parent->keys[idx]   = walk->keys[walk->nmemb-1];
    parent->values[idx] = walk->values[walk->nmemb-1];
    idx                 = walk->nmemb-1;
  }

  memcpy(&walk->keys[idx], &walk->keys[idx+1], __SIZEOF_POINTER__*(--walk->nmemb-idx));
  memcpy(&walk->values[idx], &walk->values[idx+1], __SIZEOF_POINTER__*(walk->nmemb-idx));

  while (!empty(stack)) {
    if  ((order-1)>>1 <= walk->nmemb) { destroy(&stack); return; }

    idx     = (size_t)pop(&stack);
    parent  = pop(&stack);
    sibling = idx == 0                                                        ? parent->children[1]
            : idx == parent->nmemb                                            ? parent->children[idx-1]
            : parent->children[idx-1]->nmemb < parent->children[idx+1]->nmemb ? parent->children[idx+1]
                                                                              : parent->children[idx-1];
    if   ((order-1)>>1 < sibling->nmemb) {    /* case of key redistribution */
      if (sibling == parent->children[idx-1]) {
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
      destroy(&stack);
      return;
    }

    if (sibling == parent->children[idx-1]) { /* case of node merge */
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

  if (walk->nmemb == 0) { *tree = walk->children[0]; btree_free(walk); }
}

/**
 * btree_preorder - applies @func to each node of @tree preorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
extern inline void btree_preorder(const struct btree_node *restrict tree, void (*func)(const void *restrict, void *restrict)) {
  if    (tree != NULL) {
    for (size_t idx = 0; idx < tree->nmemb; ++idx) {
      func(tree->keys[idx], tree->values[idx]);
      btree_preorder(tree->children[idx], func);
    }
    btree_preorder(tree->children[tree->nmemb], func);
  }
}

/**
 * btree_inorder - applies @func to each node of @tree inorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
extern inline void btree_inorder(const struct btree_node *restrict tree, void (*func)(const void *restrict, void *restrict)) {
  if    (tree != NULL) {
    for (size_t idx = 0; idx < tree->nmemb; ++idx) {
      btree_inorder(tree->children[idx], func);
      func(tree->keys[idx], tree->values[idx]);
    }
    btree_inorder(tree->children[tree->nmemb], func);
  }
}

/**
 * btree_postorder - applies @func to each node of @tree postorderwise
 *
 * @tree: tree to apply @func to each node of
 * @func: function to apply to each node of @tree
 */
extern inline void btree_postorder(const struct btree_node *restrict tree, void (*func)(const void *restrict, void *restrict)) {
  if    (tree != NULL) {
    btree_postorder(tree->children[0], func);
    for (size_t idx = 0; idx < tree->nmemb; ++idx) {
      btree_postorder(tree->children[idx+1], func);
      func(tree->keys[idx], tree->values[idx]);
    }
  }
}

#ifdef __cplusplus
}
#endif

#endif /* _BTREE_H */
