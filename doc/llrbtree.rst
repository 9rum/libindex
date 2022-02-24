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

    ``struct llrb_node`` and ``struct llrb_root``

        | These structures represent a node and the root of a left-leaning red-black tree respectively.

    The below function uses the operator with 3 different calling conventions. The operator denotes:

    .. code-block::

      a < b  := less(a, b)
      a > b  := less(b, a)
      a == b := !less(a, b) && !less(b, a)

    NOTE:

    *less* must describe a transitive ordering:
        * if both ``less(a, b)`` and ``less(b, c)`` are ``true``, then ``less(a, c)`` must be ``true`` as well
        * if both ``less(a, b)`` and ``less(b, c)`` are ``false``, then ``less(a, c)`` must be ``false`` as well

    ``struct llrb_root llrb_init(bool (*less)(const void *, const void *))``

        | This function initializes an empty tree with operator *less*.

    ``void *llrb_find(const struct llrb_root tree, const void *key)``

        | This function finds element from tree *tree* with specified key *key*.
        | It returns the value of element with matched key.
        | If *key* is not found in *tree*, it returns ``NULL``.

    ``struct llrb_node *llrb_insert(struct llrb_root *tree, const void *key, void *value)``

        | This function inserts key *key* and value *value* into tree *tree*.
        | It returns the pointer to the inserted element.
        | If *key* already exists in *tree*, it returns ``NULL`` without insertion.

    ``struct llrb_node *llrb_insert_or_assign(struct llrb_root *tree, const void *key, void *value)``

        | This function inserts key *key* and value *value* into tree *tree*.
        | Unlike ``llrb_insert``, it assigns *value* if *key* already exists in *tree*.
        | It returns the pointer to the inserted/updated element.

    ``void *llrb_erase(struct llrb_root *tree, const void *key)``

        | This function erases element from tree *tree* with specified key *key*.
        | It returns the value of element with matched key.
        | If *key* does not exist in *tree*, it returns ``NULL`` without deletion.

    ``void llrb_clear(struct llrb_root *tree)``

        | This function clears tree *tree*.
        | If you inserted element using ``llrb_insert`` or ``llrb_insert_or_assign`` and did not erase the entire element, you must clear the tree using this function, or memory leak would occur.
        | After calling this function, ``llrb_size`` returns zero.

    ``void llrb_preorder(const struct llrb_root tree, void (*func)(const void *, void *))``

        | This function applies function *func* to each element of tree *tree* preorderwise.

    ``void llrb_inorder(const struct llrb_root tree, void (*func)(const void *, void *))``

        | This function applies function *func* to each element of tree *tree* inorderwise.

    ``void llrb_postorder(const struct llrb_root tree, void (*func)(const void *, void *))``

        | This function applies function *func* to each element of tree *tree* postorderwise.
