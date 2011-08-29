========
Building
========

.. _Compiling the source:

Compiling the source
====================
Dependencies
------------
* Poco
* librsync
* cmake
* make
* A C++ compiler

Compiling
---------
First get the source by executing ::

  git clone git://github.com/Ivesvdf/Bacsy.git

or an equivalent git command. 

Navigate into the newly created directory with ::

  cd Bacsy

We'll be doing an out-of-source build, so ::

  mkdir build
  cd build

Then ::

  cmake ..
  make

When all your dependencies are installed, this should go smoothly. Currently,
no installer exists. 

Building the documentation
==========================
The documentation is written in ReStructuredText and built with `sphinx
<http://sphinx.pocoo.org/>`_, it can be compiled into HTML by

::

  cd docs
  make

