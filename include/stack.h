/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright 2020 9rum
 *
 * stack.h - generic stack definition
 *
 * The stack gives the programmer the functionality of a stack
 * - specifically, a LIFO (last-in, first-out) data structure.
 *
 * The stack pushes and pops the element from the top of the stack.
 *
 * See https://dl.gi.de/bitstream/handle/20.500.12116/4381/lni-t-7.pdf
 */
#ifndef _STACK_H
#define _STACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>

/**
 * struct stack - generic stack
 *
 * @value: the value of the element
 * @next:  the pointer to the next element
 */
struct stack {
  void         *value;
  struct stack *next;
} __attribute__((aligned(__SIZEOF_POINTER__)));

/**
 * empty - checks whether @stack is empty
 *
 * @stack: stack to check
 */
extern inline bool empty(const struct stack *restrict stack) { return stack == NULL; }

/**
 * top - accesses the top element
 *
 * @stack: stack to access the top element
 */
extern inline void *top(const struct stack *restrict stack) { return empty(stack) ? NULL : stack->value; }

/**
 * push - inserts element at the top
 *
 * @stack: stack to insert element
 * @value: the value of the element to push
 */
extern inline void push(struct stack **restrict stack, void *restrict value) {
  struct stack *top = malloc(sizeof(struct stack));
  top->value        = value;
  top->next         = *stack;
  *stack            = top;
}

/**
 * pop - removes the top element
 *
 * @stack: stack to remove the top element
 */
extern inline void *pop(struct stack **restrict stack) {
  if (empty(*stack))  return NULL;
  struct stack *top   = *stack;
  void         *value = top->value;
  *stack              = top->next;
  free(top);
  return value;
}

/**
 * destroy - empties @stack
 *
 * @stack: stack to empty
 */
extern inline void destroy(struct stack *restrict stack) {
  register struct stack *top;

  while (!empty(stack)) {
    top   = stack;
    stack = top->next;
    free(top);
  }
}

#ifdef __cplusplus
}
#endif

#endif /* _STACK_H */
