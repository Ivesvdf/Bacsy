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

Building Packages
-----------------
All of these commands are executed from the ``build/`` directory, after
compiling.


Building shell and tar.gz packages
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Execute ::

  cpack



Building Debian packages
^^^^^^^^^^^^^^^^^^^^^^^^
.. warning:: When building Debian packages, it is strongly recommended to use a
   *static* build of the Poco libraries. If you don't, the changes of someone
   being able to use the packages executables is really small.

Execute ::

  cpack -G DEB

Building RPM Packages
^^^^^^^^^^^^^^^^^^^^^

Execute ::

  cpack -G RPM

This requires rpmbuild. 

Building the documentation
==========================
The documentation is written in ReStructuredText and built with `sphinx
<http://sphinx.pocoo.org/>`_, it can be compiled into HTML by

::

  cd docs
  make

