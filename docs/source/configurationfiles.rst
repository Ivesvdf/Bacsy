.. _Configuration Files:

===================
Configuration Files
===================

Introduction
============
.. note::
    Feel free to read :ref:`Configuring Bacsy` before you read this document.
    You probably won't need 95% of what's written here. 

Basic
-----
The primary way of configuring Bacsy is by writing a set of human readable
files in a format called *Cascading Configuration File*, or *CCF*. A Cascading
Configuration File is a normal `INI file
<http://en.wikipedia.org/wiki/INI_file>`_, with some added semantical meaning.

A Cascading Configuration File will contain a number of sections, each of
which start with ::

   [ sectionName ]

where sectionName is some arbitrary string. A section ends when another
section starts or the file ends. Each of these sections will contain a number
of assignments that look like this: ::

   key = some value

For example: :: 

   [ global ]
   Priority = 6
   HostIdentification = IX_XUBUNTU

This fragment defines a section ``global``, which contains two keys
``Priority`` and ``HostIdentification``. The key ``Priority`` in section
``global`` will then correspond to a value ``6``, and the key
``HostIdentification`` will correspond to a value ``IX_XUBUNTU``. 


Keys, section names and values are all case sensitive. Spaces may occur in
values. 

Array Keys
----------
If a key ends in ``[]``, this key can be defined multiple times in the same
section. When more than one value is assigned to this key, the key will
correspond to a list of all values assigned to that key in that section.  For
example: ::

   Include[] = /home/ives
   Include[] = /home/naomi

is used to include both ``/home/ives`` **and** ``/home/naomi``. 


.. note::
  Keys by the names of ``Include[]`` and ``Include`` are unrelated.
  Only defining ``Include[]`` once is **not** equivalent with defining
  ``Include``. 

Comments
--------
All lines that start with a ``#`` or a ``;`` character will be ignored. 


The Cascading Part
------------------
Each CCF may have a section called ``global``. When such a section exists, all
key-value pairs defined in this section are inherited by every section that is
defined in that file. When that same key is defined in a section other than
the global section, it overrides the value that was given to it in section
``global``. 

For example, if you would like to set the default priority for all sources to
``3``, you'd write something like: ::

   [ global ]
   Priority = 3

   [ firstSource ]
   Include[] = /home/ives
   Exclude[] = *.bak


   [ anotherSource ]
   Priority = 4

In this example, ``firstSource`` will have priority of 3, and
``anotherSource`` will have a priority of 4 (because the inherited value of
``3`` was overwritten. 

Configuration Files
===================
The following configuration files exist in the directory ``.bacsy``. 

`sources.configuration`_
   This file contains all information relating to the data sources for which backups should be generated. 

`stores.configuration`_
   This file contains all information relating to the data stores present on the current system. 


sources.configuration
---------------------
.. include:: sources-contents.rst


stores.configuration
--------------------
.. include:: stores-contents.rst

