1. Introduction

    | A left-leaning red–black tree is a type of self-balancing binary search tree.
    | It is a variant of the red–black tree and guarantees the same asymptotic complexity for operations, but is designed to be easier to implement.
    | See `Left-Leaning Red-Black Trees`_ for more details.

    .. _`Left-Leaning Red-Black Trees`: https://sedgewick.io/wp-content/themes/sedgewick/papers/2008LLRB.pdf

2. Using the library from a C program

    To use the library from C code, include the following preprocessor directive in your source files:

    .. code-block::

      #include <index/llrbtree.h>

3. The C API

    ``struct llrb_node`` and ``struct llrb_root``

        | These structures represent a node and the root of a left-leaning red-black tree respectively.

    ``struct llrb_iter`` and ``struct llrb_reverse_iter``

        | These structures represent an iterator and a reverse iterator of a left-leaning red-black tree respectively.

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

    ``size_t llrb_size(const struct llrb_root tree)``

        | This function returns the number of entries in tree *tree*.

    ``bool llrb_empty(const struct llrb_root tree)``

        | This function checks whether tree *tree* is empty.

    ``bool llrb_contains(const struct llrb_root tree, const void *key)``

        | This function checks if tree *tree* contains an entry with specified key *key*.

    ``struct llrb_iter llrb_find(const struct llrb_root tree, const void *key)``

        | This function searches tree *tree* for an entry with specified key *key*.
        | It returns the iterator of the entry with the equivalent key.
        | If *key* is not present in *tree*, the iterator points to ``NULL``.

    ``struct llrb_iter llrb_insert(struct llrb_root *tree, const void *key, void *value)``

        | This function inserts an entry with key *key* and value *value* into tree *tree*.
        | It returns the iterator of the inserted entry.
        | If *key* already exists in *tree*, the iterator points to the entry that prevented the insertion.

    ``struct llrb_iter llrb_replace(struct llrb_root *tree, const void *key, void *value)``

        | This function inserts an entry with key *key* and value *value* into tree *tree*.
        | Unlike ``llrb_insert``, it assigns *value* if *key* already exists in *tree*.
        | It returns the iterator of the inserted/assigned entry.

    ``void *llrb_erase(struct llrb_root *tree, const void *key)``

        | This function removes the entry from tree *tree* with specified key *key*.
        | It returns the value of the entry with the equivalent key.
        | If *key* is not present in *tree*, it returns ``NULL`` without removal.

    ``void llrb_clear(struct llrb_root *tree)``

        | This function erases all entries from tree *tree*.
        | If you inserted entries using ``llrb_insert`` or ``llrb_replace`` and did not erase all the entries, you must clear the tree using this function, or memory leak would occur.
        | After this call, ``llrb_size`` returns zero.

    ``struct llrb_iter llrb_iter_init(const struct llrb_root tree)``

        | This function initializes an iterator of tree *tree*.

    ``void llrb_iter_prev(struct llrb_iter *iter)``

        | This function finds logical previous entry of iterator *iter*.

    ``void llrb_iter_next(struct llrb_iter *iter)``

        | This function finds logical next entry of iterator *iter*.

    ``bool llrb_iter_end(const struct llrb_iter iter)``

        | This function checks if iterator *iter* reaches the end.

    ``struct llrb_reverse_iter llrb_reverse_iter_init(const struct llrb_root tree)``

        | This function initializes a reverse iterator of tree *tree*.

    ``void llrb_reverse_iter_prev(struct llrb_reverse_iter *iter)``

        | This function finds logical previous entry of reverse iterator *iter*.

    ``void llrb_reverse_iter_next(struct llrb_reverse_iter *iter)``

        | This function finds logical next entry of reverse iterator *iter*.

    ``bool llrb_reverse_iter_end(const struct llrb_reverse_iter iter)``

        | This function checks if reverse iterator *iter* reaches the end.
