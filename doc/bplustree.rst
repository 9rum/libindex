1. Introduction

    | A B+-tree is an m-ary tree with large number of children per node.
    | It can be viewed as a B-tree in which each node contains only keys (not key–value pairs), and to which an additional level is added at the bottom with linked leaves.
    | The primary value of a B+-tree is in storing data for efficient retrieval in a block-oriented storage context — in particular, filesystems.
    | This is primarily because unlike binary search trees, B+-trees have very high fanout, which reduces the number of I/O operations required to find an element in the tree.
    | The ReiserFS, NSS, XFS, JFS, ReFS, and BFS filesystems all use this type of tree for metadata indexing;
    | BFS also uses B+-trees for storing directories.
    | NTFS uses B+-trees for directory and security-related metadata indexing.
    | EXT4 uses extent trees for file extent indexing.
    | APFS uses B+-trees to store mappings from filesystem object IDs to their locations on disk, and to store filesystem records (including directories), though these trees' leaf nodes lack sibling pointers.
    | Relational database management systems such as IBM DB2, Informix, Microsoft SQL Server, Oracle 8, Sybase ASE, and SQLite support this type of tree for table indices.
    | Key–value database management systems such as CouchDB and Tokyo Cabinet support this type of tree for data access.
    | There is no single paper introducing the B+-tree concept. Instead, the notion of maintaining all data in leaf nodes is repeatedly brought up as an interesting variant.
    | An early survey of B-trees also covering B+-trees is Douglas Comer. Comer notes that the B+-tree was used in IBM's VSAM data access software and he refers to an IBM published article from 1973.
    | See `The Ubiquitous B-Tree`_ for more details.

    .. _`The Ubiquitous B-Tree`: http://carlosproal.com/ir/papers/p121-comer.pdf

2. Using the library from a C program

    To use the library from C code, include the following preprocessor directive in your source files:

    .. code-block::

      #include <index/bplustree.h>

3. The C API

    ``struct bplus_internal_node``, ``struct bplus_external_node`` and ``struct bplus_root``

        | These structures represent an internal, external node and the root of a B+-tree respectively.

    The below function uses the operator with 3 different calling conventions. The operator denotes:

    .. code-block::

      a < b  := less(a, b)
      a > b  := less(b, a)
      a == b := !less(a, b) && !less(b, a)

    NOTE:

    *less* must describe a transitive ordering:
        * if both ``less(a, b)`` and ``less(b, c)`` are ``true``, then ``less(a, c)`` must be ``true`` as well
        * if both ``less(a, b)`` and ``less(b, c)`` are ``false``, then ``less(a, c)`` must be ``false`` as well

    ``struct bplus_root bplus_init(const size_t order, bool (*less)(const void *, const void *))``

        | This function initializes an empty tree of order *order* with operator *less*.
        | To be cache conscious, setting *order* to page size / node size is recommended. e.g. ``getpagesize()/sizeof(struct bplus_internal_node)``

    ``size_t bplus_size(const struct bplus_root tree)``

        | This function returns the number of elements in tree *tree*.

    ``bool bplus_empty(const struct bplus_root tree)``

        | This function checks whether tree *tree* is empty.

    ``void *bplus_find(const struct bplus_root tree, const void *key)``

        | This function finds element from tree *tree* with specified key *key*.
        | It returns the value of the element with the equivalent key.
        | If *key* is not found in *tree*, it returns ``NULL``.

    ``bool bplus_contains(const struct bplus_root tree, const void *key)``

        | This function checks if tree *tree* contains element with specified key *key*.

    ``struct bplus_external_node *bplus_insert(struct bplus_root *tree, const void *key, void *value)``

        | This function inserts an element with key *key* and value *value* into tree *tree*.
        | It returns the address of the inserted element.
        | If *key* already exists in *tree*, it returns ``NULL`` without insertion.

    ``struct bplus_external_node *bplus_insert_or_assign(struct bplus_root *tree, const void *key, void *value)``

        | This function inserts an element with key *key* and value *value* into tree *tree*.
        | Unlike ``bplus_insert``, it assigns *value* if *key* already exists in *tree*.
        | It returns the address of the inserted/assigned element.

    ``void *bplus_erase(struct bplus_root *tree, const void *key)``

        | This function removes the element from tree *tree* with specified key *key*.
        | It returns the value of the element with the equivalent key.
        | If *key* does not exist in *tree*, it returns ``NULL`` without removal.

    ``void bplus_clear(struct bplus_root *tree)``

        | This function erases all elements from tree *tree*.
        | If you inserted elements using ``bplus_insert`` or ``bplus_insert_or_assign`` and did not erase all the elements, you must clear the tree using this function, or memory leak would occur.
        | After calling this function, ``bplus_size`` returns zero.

    ``void bplus_for_each(const struct bplus_root tree, void (*func)(const void *, void *))``

        | This function applies function *func* to each element of tree *tree* in ascending order.

    ``void bplus_rev_each(const struct bplus_root tree, void (*func)(const void *, void *))``

        | This function applies function *func* to each element of tree *tree* in descending order.

    ``void bplus_range_each(const struct bplus_root tree, const void *inf, const void *sup, void (*func)(const void *, void *))``

        | This function applies function *func* to each element of tree *tree* greater than or equal to lower bound *inf* and less than upper bound *sup*.
