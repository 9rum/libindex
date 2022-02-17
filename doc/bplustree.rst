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

    ``struct bplus_internal_node`` and ``struct bplus_external_node``

    | These data structures represent an internal and external node in B+-tree respectively.
    | To initialize an empty tree, declare pointers of type ``struct bplus_internal_node *`` and ``struct bplus_external_node *`` to ``NULL``.

    .. code-block::

      struct bplus_internal_node *tree = NULL;
      struct bplus_external_node *list = NULL;

    The below functions use the operator with 3 different calling conventions. The operator denotes:

    .. code-block::

      a < b  := less(a, b)
      a > b  := less(b, a)
      a == b := !less(a, b) && !less(b, a)

    NOTE:

    *less* must describe a transitive ordering:
        * if both ``less(a, b)`` and ``less(b, c)`` are ``true``, then ``less(a, c)`` must be ``true`` as well
        * if both ``less(a, b)`` and ``less(b, c)`` are ``false``, then ``less(a, c)`` must be ``false`` as well

    ``void *bplus_find(const struct bplus_internal_node *tree, const struct bplus_external_node *list, const void *key, bool (*less)(const void *, const void *))``

        | This function finds element from tree *tree* and list *list* with specified key *key* using operator *less*.
        | It returns the value of element with matched key.
        | If *key* is not found in *list*, it returns ``NULL``.

    ``struct bplus_external_node *bplus_insert(struct bplus_internal_node **tree, struct bplus_external_node **list, const size_t order, const void *key, void *value, bool (*less)(const void *, const void *))``

        | This function inserts key *key* and value *value* into tree *tree* and list *list* of order *order* using operator *less*.
        | It returns the pointer to the inserted element.
        | If *key* already exists in *list*, it returns ``NULL`` without insertion.

    ``struct bplus_external_node *bplus_insert_or_assign(struct bplus_internal_node **tree, struct bplus_external_node **list, const size_t order, const void *key, void *value, bool (*less)(const void *, const void *))``

        | This function inserts key *key* and value *value* into tree *tree* and list *list* of order *order* using operator *less*.
        | Unlike ``bplus_insert``, it assigns *value* if *key* already exists in *list*.
        | It returns the pointer to the inserted/updated element.

    ``void *bplus_erase(struct bplus_internal_node **tree, struct bplus_external_node **list, const size_t order, const void *key, bool (*less)(const void *, const void *))``

        | This function erases element from tree *tree* and list *list* of order *order* with specified key *key* using operator *less*.
        | It returns the value of element with matched key.
        | If *key* does not exist in *list*, it returns ``NULL`` without deletion.

    ``void bplus_clear(struct bplus_internal_node **tree, struct bplus_external_node **list)``

        | This function clears tree *tree* and list *list*.
        | If you inserted element using ``bplus_insert`` or ``bplus_insert_or_assign`` and did not erase the entire element, you must clear the tree and list using this function, or memory leak would occur.
        | After calling this function, *tree* and *list* becomes ``NULL``.

    ``void bplus_for_each(const struct bplus_external_node *list, void (*func)(const void *, void *))``

        | This function applies function *func* to each element of list *list* sequentialwise.
