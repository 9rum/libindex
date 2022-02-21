Index library
=============

This library is a stack-based in-memory implementation of five widely-used search trees for making efficient data index. *i.e.*

* AVL tree
* Red-black tree
* Left-leaning red-black tree
* B-tree
* B+-tree

Building the Index library
--------------------------

| You should have `GNU Autotools`_ already installed on your machine, before generating Makefile.
| Run the following commands:

.. code-block::

  $ autoreconf --install
  $ ./configure
  $ make

.. _`GNU Autotools`: https://www.gnu.org/software/automake/manual/html_node/index.html

Running tests
-------------

Use ``make check``.

Installing the Index library
----------------------------

Use ``make install``.

Documentation
-------------

There are several guides for users. Please read `doc/README.rst`_ first.

.. _`doc/README.rst`: https://github.com/9rum/libindex/blob/master/doc/README.rst

License
-------

| The Index library is distributed under the terms of the GNU Lesser General Public License, version 2.1;
| see `COPYING`_ for the full license text.

.. _`COPYING`: https://github.com/9rum/libindex/blob/master/COPYING

References
----------

* `Keller, Stack und automatisches Gedächtnis – eine Struktur mit Potenzial`_
* `An algorithm for the organization of information`_
* `Symmetric Binary B-Trees: Data Structure and Algorithms for Random and Sequential Information Processing`_
* `Left-Leaning Red-Black Trees`_
* `Organization and maintenance of large ordered indices`_
* `The Ubiquitous B-Tree`_

.. _`Keller, Stack und automatisches Gedächtnis – eine Struktur mit Potenzial`: https://dl.gi.de/bitstream/handle/20.500.12116/4381/lni-t-7.pdf
.. _`An algorithm for the organization of information`: https://zhjwpku.com/assets/pdf/AED2-10-avl-paper.pdf
.. _`Symmetric Binary B-Trees: Data Structure and Algorithms for Random and Sequential Information Processing`: https://docs.lib.purdue.edu/cgi/viewcontent.cgi?article=1457&context=cstech
.. _`Left-Leaning Red-Black Trees`: https://www.cs.princeton.edu/~rs/talks/LLRB/LLRB.pdf
.. _`Organization and maintenance of large ordered indices`: https://infolab.usc.edu/csci585/Spring2010/den_ar/indexing.pdf
.. _`The Ubiquitous B-Tree`: http://carlosproal.com/ir/papers/p121-comer.pdf
