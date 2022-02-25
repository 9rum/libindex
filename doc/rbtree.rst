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

        | This function returns the number of elements in tree *tree*.

    ``bool rb_empty(const struct rb_root tree)``

        | This function checks whether tree *tree* is empty.

    ``void *rb_find(const struct rb_root tree, const void *key)``

        | This function finds element from tree *tree* with specified key *key*.
        | It returns the value of the element with the equivalent key.
        | If *key* is not found in *tree*, it returns ``NULL``.

    ``bool rb_contains(const struct rb_root tree, const void *key)``

        | This function checks if tree *tree* contains element with specified key *key*.

    ``struct rb_node *rb_insert(struct rb_root *tree, const void *key, void *value)``

        | This function inserts an element with key *key* and value *value* into tree *tree*.
        | It returns the address of the inserted element.
        | If *key* already exists in *tree*, it returns ``NULL`` without insertion.

    ``struct rb_node *rb_insert_or_assign(struct rb_root *tree, const void *key, void *value)``

        | This function inserts an element with key *key* and value *value* into tree *tree*.
        | Unlike ``rb_insert``, it assigns *value* if *key* already exists in *tree*.
        | It returns the address of the inserted/assigned element.

    ``void *rb_erase(struct rb_root *tree, const void *key)``

        | This function removes the element from tree *tree* with specified key *key*.
        | It returns the value of the element with the equivalent key.
        | If *key* does not exist in *tree*, it returns ``NULL`` without removal.

    ``void rb_clear(struct rb_root *tree)``

        | This function erases all elements from tree *tree*.
        | If you inserted elements using ``rb_insert`` or ``rb_insert_or_assign`` and did not erase all the elements, you must clear the tree using this function, or memory leak would occur.
        | After calling this function, ``rb_size`` returns zero.

    ``void rb_preorder(const struct rb_root tree, void (*func)(const void *, void *))``

        | This function applies function *func* to each element of tree *tree* preorderwise.

    ``void rb_inorder(const struct rb_root tree, void (*func)(const void *, void *))``

        | This function applies function *func* to each element of tree *tree* inorderwise.

    ``void rb_postorder(const struct rb_root tree, void (*func)(const void *, void *))``

        | This function applies function *func* to each element of tree *tree* postorderwise.
