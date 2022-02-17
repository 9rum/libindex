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

    ``struct btree_node``

    | This data structure represents a node in B-tree.
    | To initialize an empty tree, declare a pointer of type ``struct btree_node *`` to ``NULL``.

    .. code-block::

      struct btree_node *tree = NULL;

    The below functions use the operator with 3 different calling conventions. The operator denotes:

    .. code-block::

      a < b  := less(a, b)
      a > b  := less(b, a)
      a == b := !less(a, b) && !less(b, a)

    NOTE:

    *less* must describe a transitive ordering:
        * if both ``less(a, b)`` and ``less(b, c)`` are ``true``, then ``less(a, c)`` must be ``true`` as well
        * if both ``less(a, b)`` and ``less(b, c)`` are ``false``, then ``less(a, c)`` must be ``false`` as well

    ``void *btree_find(const struct btree_node *tree, const void *key, bool (*less)(const void *, const void *))``

        | This function finds element from tree *tree* with specified key *key* using operator *less*.
        | It returns the value of element with matched key.
        | If *key* is not found in *tree*, it returns ``NULL``.

    ``struct btree_node *btree_insert(struct btree_node **tree, const size_t order, const void *key, void *value, bool (*less)(const void *, const void *))``

        | This function inserts key *key* and value *value* into tree *tree* of order *order* using operator *less*.
        | It returns the pointer to the inserted element.
        | If *key* already exists in *tree*, it returns ``NULL`` without insertion.

    ``struct btree_node *btree_insert_or_assign(struct btree_node **tree, const size_t order, const void *key, void *value, bool (*less)(const void *, const void *))``

        | This function inserts key *key* and value *value* into tree *tree* of order *order* using operator *less*.
        | Unlike ``btree_insert``, it assigns *value* if *key* already exists in *tree*.
        | It returns the pointer to the inserted/updated element.

    ``void *btree_erase(struct btree_node **tree, const size_t order, const void *key, bool (*less)(const void *, const void *))``

        | This function erases element from tree *tree* of order *order* with specified key *key* using operator *less*.
        | It returns the value of element with matched key.
        | If *key* does not exist in *tree*, it returns ``NULL`` without deletion.

    ``void btree_clear(struct btree_node **tree)``

        | This function clears tree *tree*.
        | If you inserted element using ``btree_insert`` or ``btree_insert_or_assign`` and did not erase the entire element, you must clear the tree using this function, or memory leak would occur.
        | After calling this function, *tree* becomes ``NULL``.

    ``void btree_preorder(const struct btree_node *tree, void (*func)(const void *, void *))``

        | This function applies function *func* to each element of tree *tree* preorderwise.

    ``void btree_inorder(const struct btree_node *tree, void (*func)(const void *, void *))``

        | This function applies function *func* to each element of tree *tree* inorderwise.

    ``void btree_postorder(const struct btree_node *tree, void (*func)(const void *, void *))``

        | This function applies function *func* to each element of tree *tree* postorderwise.
