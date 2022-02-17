1. Introduction

    | A left-leaning red–black tree is a type of self-balancing binary search tree.
    | It is a variant of the red–black tree and guarantees the same asymptotic complexity for operations, but is designed to be easier to implement.
    | See `Left-Leaning Red-Black Trees`_ for more details.

    .. _`Left-Leaning Red-Black Trees`: https://www.cs.princeton.edu/~rs/talks/LLRB/LLRB.pdf

2. Using the library from a C program

    To use the library from C code, include the following preprocessor directive in your source files:

    .. code-block::

      #include <index/llrbtree.h>

3. The C API

    ``struct llrb_node``

    | This data structure represents a node in left-leaning red-black tree.
    | To initialize an empty tree, declare a pointer of type ``struct llrb_node *`` to ``NULL``.

    .. code-block::

      struct llrb_node *tree = NULL;

    The below functions use the operator with 3 different calling conventions. The operator denotes:

    .. code-block::

      a < b  := less(a, b)
      a > b  := less(b, a)
      a == b := !less(a, b) && !less(b, a)

    NOTE:

    *less* must describe a transitive ordering:
        * if both ``less(a, b)`` and ``less(b, c)`` are ``true``, then ``less(a, c)`` must be ``true`` as well
        * if both ``less(a, b)`` and ``less(b, c)`` are ``false``, then ``less(a, c)`` must be ``false`` as well

    ``void *llrb_find(const struct llrb_node *tree, const void *key, bool (*less)(const void *, const void *))``

        | This function finds element from tree *tree* with specified key *key* using operator *less*.
        | It returns the value of element with matched key.
        | If *key* is not found in *tree*, it returns ``NULL``.

    ``struct llrb_node *llrb_insert(struct llrb_node **tree, const void *key, void *value, bool (*less)(const void *, const void *))``

        | This function inserts key *key* and value *value* into tree *tree* using operator *less*.
        | It returns the pointer to the inserted element.
        | If *key* already exists in *tree*, it returns ``NULL`` without insertion.

    ``struct llrb_node *llrb_insert_or_assign(struct llrb_node **tree, const void *key, void *value, bool (*less)(const void *, const void *))``

        | This function inserts key *key* and value *value* into tree *tree* using operator *less*.
        | Unlike ``llrb_insert``, it assigns *value* if *key* already exists in *tree*.
        | It returns the pointer to the inserted/updated element.

    ``void *llrb_erase(struct llrb_node **tree, const void *key, bool (*less)(const void *, const void *))``

        | This function erases element from tree *tree* with specified key *key* using operator *less*.
        | It returns the value of element with matched key.
        | If *key* does not exist in *tree*, it returns ``NULL`` without deletion.

    ``void llrb_clear(struct llrb_node **tree)``

        | This function clears tree *tree*.
        | If you inserted element using ``llrb_insert`` or ``llrb_insert_or_assign`` and did not erase the entire element, you must clear the tree using this function, or memory leak would occur.
        | After calling this function, *tree* becomes ``NULL``.

    ``void llrb_preorder(const struct llrb_node *tree, void (*func)(const void *, void *))``

        | This function applies function *func* to each element of tree *tree* preorderwise.

    ``void llrb_inorder(const struct llrb_node *tree, void (*func)(const void *, void *))``

        | This function applies function *func* to each element of tree *tree* inorderwise.

    ``void llrb_postorder(const struct llrb_node *tree, void (*func)(const void *, void *))``

        | This function applies function *func* to each element of tree *tree* postorderwise.
