/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright 2020 - 2022 9rum
 *
 * stack.c - generic stack definition
 */
#include <index/stack.h>

extern inline bool stack_empty(const struct stack *restrict stack) { return stack == NULL; }

extern inline void *stack_top(const struct stack *restrict stack) { return stack_empty(stack) ? NULL : stack->value; }

extern inline void stack_push(struct stack **restrict stack, void *restrict value) {
  struct stack *top = malloc(sizeof(struct stack));
  top->value        = value;
  top->next         = *stack;
  *stack            = top;
}

extern inline void *stack_pop(struct stack **restrict stack) {
  if (stack_empty(*stack)) return NULL;
  struct stack *top        = *stack;
  void         *value      = top->value;
  *stack                   = top->next;
  free(top);
  return value;
}

extern inline void stack_free(struct stack *restrict stack) {
  register struct stack *top;

  while (!stack_empty(stack)) {
    top   = stack;
    stack = top->next;
    free(top);
  }
}
