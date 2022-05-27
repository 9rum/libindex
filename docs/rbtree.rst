1. Introduction

    | The red-black tree is a kind of self-balancing binary search tree.
    | Each node stores an extra bit representing "color" ("red" or "black"), used to ensure that the tree remains balanced during insertions and deletions.
    | When the tree is modified, the new tree is rearranged and recolored to restore the coloring properties that constrain how unbalanced the tree can become in the worst case.
    | The rebalancing is not perfect, but guarantees searching in logarithmic time, where n is the number of nodes in the tree.
    | The insertion and deletion, along with the tree rearranging and recoloring, are also performed in logarithmic time.
    | See `Symmetric Binary B-Trees: Data Structure and Algorithms for Random and Sequential Information Processing`_ for more details.

    .. _`Symmetric Binary B-Trees: Data Structure and Algorithms for Random and Sequential Information Processing`: https://docs.lib.purdue.edu/cgi/viewcontent.cgi?article=1457&context=cstech

2. Using the library from a C program

    To use the library from C code, include the following preprocessor directive in your source files:

    .. code-block::

      #include <index/rbtree.h>

3. The C API

    ``struct rb_node`` and ``struct rb_root``

        | These structures represent a node and the root of a red-black tree respectively.

    ``struct rb_iter`` and ``struct rb_reverse_iter``

        | These structures represent an iterator and a reverse iterator of a red-black tree respectively.

    The below function uses the operator with 3 different calling conventions. The operator denotes:

    .. code-block::

      a < b  := less(a, b)
      a > b  := less(b, a)
      a == b := !less(a, b) && !less(b, a)

    NOTE:

    *less* must describe a transitive ordering:
        * if both ``less(a, b)`` and ``less(b, c)`` are ``true``, then ``less(a, c)`` must be ``true`` as well
        * if both ``less(a, b)`` and ``less(b, c)`` are ``false``, then ``less(a, c)`` must be ``false`` as well

    ``struct rb_root rb_init(bool (*less)(const void *, const void *))``

        | This function initializes an empty tree with operator *less*.

    ``size_t rb_size(const struct rb_root tree)``

        | This function returns the number of entries in tree *tree*.

    ``bool rb_empty(const struct rb_root tree)``

        | This function checks whether tree *tree* is empty.

    ``bool rb_contains(const struct rb_root tree, const void *key)``

        | This function checks if tree *tree* contains an entry with specified key *key*.

    ``struct rb_iter rb_find(const struct rb_root tree, const void *key)``

        | This function searches tree *tree* for an entry with specified key *key*.
        | It returns the iterator of the entry with the equivalent key.
        | If *key* is not present in *tree*, the iterator points to ``NULL``.

    ``struct rb_iter rb_insert(struct rb_root *tree, const void *key, void *value)``

        | This function inserts an entry with key *key* and value *value* into tree *tree*.
        | It returns the iterator of the inserted entry.
        | If *key* already exists in *tree*, the iterator points to the entry that prevented the insertion.

    ``struct rb_iter rb_replace(struct rb_root *tree, const void *key, void *value)``

        | This function inserts an entry with key *key* and value *value* into tree *tree*.
        | Unlike ``rb_insert``, it assigns *value* if *key* already exists in *tree*.
        | It returns the iterator of the inserted/assigned entry.

    ``void *rb_erase(struct rb_root *tree, const void *key)``

        | This function removes the entry from tree *tree* with specified key *key*.
        | It returns the value of the entry with the equivalent key.
        | If *key* is not present in *tree*, it returns ``NULL`` without removal.

    ``void rb_clear(struct rb_root *tree)``

        | This function erases all entries from tree *tree*.
        | If you inserted entries using ``rb_insert`` or ``rb_replace`` and did not erase all the entries, you must clear the tree using this function, or memory leak would occur.
        | After this call, ``rb_size`` returns zero.

    ``struct rb_iter rb_iter_init(const struct rb_root tree)``

        | This function initializes an iterator of tree *tree*.

    ``void rb_iter_prev(struct rb_iter *iter)``

        | This function finds logical previous entry of iterator *iter*.

    ``void rb_iter_next(struct rb_iter *iter)``

        | This function finds logical next entry of iterator *iter*.

    ``bool rb_iter_end(const struct rb_iter iter)``

        | This function checks if iterator *iter* reaches the end.

    ``struct rb_reverse_iter rb_reverse_iter_init(const struct rb_root tree)``

        | This function initializes a reverse iterator of tree *tree*.

    ``void rb_reverse_iter_prev(struct rb_reverse_iter *iter)``

        | This function finds logical previous entry of reverse iterator *iter*.

    ``void rb_reverse_iter_next(struct rb_reverse_iter *iter)``

        | This function finds logical next entry of reverse iterator *iter*.

    ``bool rb_reverse_iter_end(const struct rb_reverse_iter iter)``

        | This function checks if reverse iterator *iter* reaches the end.
