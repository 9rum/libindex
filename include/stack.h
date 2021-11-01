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
 * stack.h - generic stack definition
 *
 * The stack gives the programmer the functionality of a stack
 * - specifically, a LIFO (last-in, first-out) data structure.
 *
 * The stack pushes and pops the element from the top of the stack.
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
 *
 * See https://en.cppreference.com/w/cpp/container/stack
 */
struct stack {
  const void         *value;
  const struct stack *next;
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
extern inline void push(struct stack **restrict stack, const void *restrict value) {
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
extern inline void destroy(struct stack **restrict stack) {
  register struct stack *top;

  while (!empty(*stack)) {
    top    = *stack;
    *stack = top->next;
    free(top);
  }
}

#ifdef __cplusplus
}
#endif

#endif /* _STACK_H */
