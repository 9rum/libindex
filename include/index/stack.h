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
 * See https://dl.gi.de/bitstream/handle/20.500.12116/4381/lni-t-7.pdf for more details.
 */
#ifndef _INDEX_STACK_H
#define _INDEX_STACK_H

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
 * stack_empty - checks whether @stack is empty
 *
 * @stack: stack to check
 */
extern inline bool stack_empty(const struct stack *restrict stack) { return stack == NULL; }

/**
 * stack_top - accesses the top element of @stack
 *
 * @stack: stack to access the top element
 */
extern inline void *stack_top(const struct stack *restrict stack) { return stack_empty(stack) ? NULL : stack->value; }

/**
 * stack_push - inserts @value at the top of @stack
 *
 * @stack: stack to insert @value
 * @value: the value of the element to insert
 */
extern inline void stack_push(struct stack **restrict stack, void *restrict value) {
  struct stack *top = malloc(sizeof(struct stack));
  top->value        = value;
  top->next         = *stack;
  *stack            = top;
}

/**
 * stack_pop - removes the top element from @stack
 *
 * @stack: stack to remove the top element from
 */
extern inline void *stack_pop(struct stack **restrict stack) {
  if (stack_empty(*stack)) return NULL;
  struct stack *top        = *stack;
  void         *value      = top->value;
  *stack                   = top->next;
  free(top);
  return value;
}

/**
 * stack_free - deallocates @stack
 *
 * @stack: stack to deallocate
 */
extern inline void stack_free(struct stack *restrict stack) {
  register struct stack *top;

  while (!stack_empty(stack)) {
    top   = stack;
    stack = top->next;
    free(top);
  }
}

#endif /* _INDEX_STACK_H */
