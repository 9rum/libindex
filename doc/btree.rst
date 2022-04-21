1. Introduction

    | A B-tree is a self-balancing tree data structure that maintains sorted data and allows searches, sequential access, insertions, and deletions in logarithmic time.
    | The B-tree generalizes the binary search tree, allowing for nodes with more than two children.
    | Unlike other self-balancing binary search trees, the B-tree is well suited for storage systems that read and write relatively large blocks of data, such as disks.
    | It is commonly used in databases and file systems.
    | B-trees were invented by Rudolf Bayer and Edward M. McCreight while working at Boeing Research Labs, for the purpose of efficiently managing index pages for large random access files.
    | The basic assumption was that indexes would be so voluminous that only small chunks of the tree could fit in main memory.
    | Bayer and McCreight's paper, Organization and maintenance of large ordered indices, was first circulated in July 1970 and later published in Acta Informatica.
    | See `Organization and maintenance of large ordered indices`_ for more details.

    .. _`Organization and maintenance of large ordered indices`: https://infolab.usc.edu/csci585/Spring2010/den_ar/indexing.pdf

2. Using the library from a C program

    To use the library from C code, include the following preprocessor directive in your source files:

    .. code-block::

      #include <index/btree.h>

3. The C API

    ``struct btree_node`` and ``struct btree_root``

        | These structures represent a node and the root of a B-tree respectively.

    ``struct btree_iter`` and ``struct btree_reverse_iter``

        | These structures represent an iterator and a reverse iterator of a B-tree respectively.

    The below function uses the operator with 3 different calling conventions. The operator denotes:

    .. code-block::

      a < b  := less(a, b)
      a > b  := less(b, a)
      a == b := !less(a, b) && !less(b, a)

    NOTE:

    *less* must describe a transitive ordering:
        * if both ``less(a, b)`` and ``less(b, c)`` are ``true``, then ``less(a, c)`` must be ``true`` as well
        * if both ``less(a, b)`` and ``less(b, c)`` are ``false``, then ``less(a, c)`` must be ``false`` as well

    ``struct btree_root btree_init(const size_t order, bool (*less)(const void *, const void *))``

        | This function initializes an empty tree of order *order* with operator *less*.
        | To be cache conscious, it is recommended to set *order* to make each node fit on a single page.

    ``size_t btree_size(const struct btree_root tree)``

        | This function returns the number of entries in tree *tree*.

    ``bool btree_empty(const struct btree_root tree)``

        | This function checks whether tree *tree* is empty.

    ``bool btree_contains(const struct btree_root tree, const void *key)``

        | This function checks if tree *tree* contains an entry with specified key *key*.

    ``struct btree_iter btree_find(const struct btree_root tree, const void *key)``

        | This function searches tree *tree* for an entry with specified key *key*.
        | It returns the iterator of the entry with the equivalent key.
        | If *key* is not present in *tree*, the iterator points to ``NULL``.

    ``struct btree_iter btree_insert(struct btree_root *tree, const void *key, void *value)``

        | This function inserts an entry with key *key* and value *value* into tree *tree*.
        | It returns the iterator of the inserted entry.
        | If *key* already exists in *tree*, the iterator points to the entry that prevented the insertion.

    ``struct btree_iter btree_replace(struct btree_root *tree, const void *key, void *value)``

        | This function inserts an entry with key *key* and value *value* into tree *tree*.
        | Unlike ``btree_insert``, it assigns *value* if *key* already exists in *tree*.
        | It returns the iterator of the inserted/assigned entry.

    ``void *btree_erase(struct btree_root *tree, const void *key)``

        | This function removes the entry from tree *tree* with specified key *key*.
        | It returns the value of the entry with the equivalent key.
        | If *key* is not present in *tree*, it returns ``NULL`` without removal.

    ``void btree_clear(struct btree_root *tree)``

        | This function erases all entries from tree *tree*.
        | If you inserted entries using ``btree_insert`` or ``btree_replace`` and did not erase all the entries, you must clear the tree using this function, or memory leak would occur.
        | After this call, ``btree_size`` returns zero.

    ``struct btree_iter btree_iter_init(const struct btree_root tree)``

        | This function initializes an iterator of tree *tree*.

    ``void btree_iter_prev(struct btree_iter *iter)``

        | This function finds logical previous entry of iterator *iter*.

    ``void btree_iter_next(struct btree_iter *iter)``

        | This function finds logical next entry of iterator *iter*.

    ``bool btree_iter_end(const struct btree_iter iter)``

        | This function checks if iterator *iter* reaches the end.

    ``struct btree_reverse_iter btree_reverse_iter_init(const struct btree_root tree)``

        | This function initializes a reverse iterator of tree *tree*.

    ``void btree_reverse_iter_prev(struct btree_reverse_iter *iter)``

        | This function finds logical previous entry of reverse iterator *iter*.

    ``void btree_reverse_iter_next(struct btree_reverse_iter *iter)``

        | This function finds logical next entry of reverse iterator *iter*.

    ``bool btree_reverse_iter_end(const struct btree_reverse_iter iter)``

        | This function checks if reverse iterator *iter* reaches the end.
