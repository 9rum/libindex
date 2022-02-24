1. Introduction

    | The AVL tree (named after inventors Adelson-Velsky and Landis) is a self-balancing binary search tree.
    | In an AVL tree, the heights of the two child subtrees of any node differ by at most one;
    | if at any time they differ by more than one, rebalancing is done to restore this property.
    | Lookup, insertion, and deletion all take logarithmic time in both the average and worst cases, where n is the number of nodes in the tree prior to the operation.
    | See `An algorithm for the organization of information`_ for more details.

    .. _`An algorithm for the organization of information`: https://zhjwpku.com/assets/pdf/AED2-10-avl-paper.pdf

2. Using the library from a C program

    To use the library from C code, include the following preprocessor directive in your source files:

    .. code-block::

      #include <index/avltree.h>

3. The C API

    ``struct avl_node`` and ``struct avl_root``

    | These data structures represent a node and the metadata of AVL tree respectively.

    The below function uses the operator with 3 different calling conventions. The operator denotes:

    .. code-block::

      a < b  := less(a, b)
      a > b  := less(b, a)
      a == b := !less(a, b) && !less(b, a)

    NOTE:

    *less* must describe a transitive ordering:
        * if both ``less(a, b)`` and ``less(b, c)`` are ``true``, then ``less(a, c)`` must be ``true`` as well
        * if both ``less(a, b)`` and ``less(b, c)`` are ``false``, then ``less(a, c)`` must be ``false`` as well

    ``struct avl_root avl_init(bool (*less)(const void *, const void *))``

    | This function initializes an empty tree with operator *less*.

    ``void *avl_find(const struct avl_root tree, const void *key)``

    | This function finds element from tree *tree* with specified key *key*.
    | It returns the value of element with matched key.
    | If *key* is not found in *tree*, it returns ``NULL``.

    ``struct avl_node *avl_insert(struct avl_root *tree, const void *key, void *value)``

    | This function inserts key *key* and value *value* into tree *tree*.
    | It returns the pointer to the inserted element.
    | If *key* already exists in *tree*, it returns ``NULL`` without insertion.

    ``struct avl_node *avl_insert_or_assign(struct avl_root *tree, const void *key, void *value)``

    | This function inserts key *key* and value *value* into tree *tree*.
    | Unlike ``avl_insert``, it assigns *value* if *key* already exists in *tree*.
    | It returns the pointer to the inserted/updated element.

    ``void *avl_erase(struct avl_root *tree, const void *key)``

    | This function erases element from tree *tree* with specified key *key*.
    | It returns the value of element with matched key.
    | If *key* does not exist in *tree*, it returns ``NULL`` without deletion.

    ``void avl_clear(struct avl_root *tree)``

    | This function clears tree *tree*.
    | If you inserted element using ``avl_insert`` or ``avl_insert_or_assign`` and did not erase the entire element, you must clear the tree using this function, or memory leak would occur.
    | After calling this function, ``avl_size`` returns zero.

    ``void avl_preorder(const struct avl_root tree, void (*func)(const void *, void *))``

    | This function applies function *func* to each element of tree *tree* preorderwise.

    ``void avl_inorder(const struct avl_root tree, void (*func)(const void *, void *))``

    | This function applies function *func* to each element of tree *tree* inorderwise.

    ``void avl_postorder(const struct avl_root tree, void (*func)(const void *, void *))``

    | This function applies function *func* to each element of tree *tree* postorderwise.
