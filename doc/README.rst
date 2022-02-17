Documentation
=============

| These are the API documentations for using the Index library. Read them carefully, as they tell you what this is all about and explain how to use the library.
| Each documentation contains the guide for using the data structure and algorithms:

* `avltree.rst`_: AVL tree
* `rbtree.rst`_: Red-black tree
* `llrbtree.rst`_: Left-leaning red-black tree
* `btree.rst`_: B-tree
* `bplustree.rst`_: B+-tree

.. _`avltree.rst`: https://github.com/9rum/libindex/blob/master/doc/avltree.rst
.. _`rbtree.rst`: https://github.com/9rum/libindex/blob/master/doc/rbtree.rst
.. _`llrbtree.rst`: https://github.com/9rum/libindex/blob/master/doc/llrbtree.rst
.. _`btree.rst`: https://github.com/9rum/libindex/blob/master/doc/btree.rst
.. _`bplustree.rst`: https://github.com/9rum/libindex/blob/master/doc/bplustree.rst

Linking the Index library
-------------------------

| After installing the Index library, you can use the library by statically linking it.
| As the library is installed on your standard system directory, *i.e.* ``/usr/local`` specify ``-lindex`` as an argument to the linker.
