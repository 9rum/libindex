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

        | These structures represent a node and the root of an AVL tree respectively.

    ``struct avl_iter``

        | This structure represents an AVL tree iterator.

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

    ``size_t avl_size(const struct avl_root tree)``

        | This function returns the number of entries in tree *tree*.

    ``bool avl_empty(const struct avl_root tree)``

        | This function checks whether tree *tree* is empty.

    ``bool avl_contains(const struct avl_root tree, const void *key)``

        | This function checks if tree *tree* contains an entry with specified key *key*.

    ``struct avl_iter avl_find(const struct avl_root tree, const void *key)``

        | This function searches tree *tree* for an entry with specified key *key*.
        | It returns the iterator of the entry with the equivalent key.
        | If *key* is not present in *tree*, the iterator points to ``NULL``.

    ``struct avl_iter avl_insert(struct avl_root *tree, const void *key, void *value)``

        | This function inserts an entry with key *key* and value *value* into tree *tree*.
        | It returns the iterator of the inserted entry.
        | If *key* already exists in *tree*, the iterator points to the entry that prevented the insertion.

    ``struct avl_iter avl_replace(struct avl_root *tree, const void *key, void *value)``

        | This function inserts an entry with key *key* and value *value* into tree *tree*.
        | Unlike ``avl_insert``, it assigns *value* if *key* already exists in *tree*.
        | It returns the iterator of the inserted/assigned entry.

    ``void *avl_erase(struct avl_root *tree, const void *key)``

        | This function removes the entry from tree *tree* with specified key *key*.
        | It returns the value of the entry with the equivalent key.
        | If *key* is not present in *tree*, it returns ``NULL`` without removal.

    ``void avl_clear(struct avl_root *tree)``

        | This function erases all entries from tree *tree*.
        | If you inserted entries using ``avl_insert`` or ``avl_replace`` and did not erase all the entries, you must clear the tree using this function, or memory leak would occur.
        | After calling this function, ``avl_empty`` returns ``true``.

    ``struct avl_iter avl_iter_init(const struct avl_root tree)``

        | This function initializes an iterator of tree *tree*.

    ``void avl_iter_prev(struct avl_iter *iter)``

        | This function finds logical previous entry of iterator *iter*.

    ``void avl_iter_next(struct avl_iter *iter)``

        | This function finds logical next entry of iterator *iter*.
