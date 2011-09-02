==============
Stores.config
==============

Synopsis
========
*$XDG_CONFIG_HOME/bacsy/stores.config*

*~/.config/bacsy/stores.config*

Description
===========
The *stores.conf* is a Cascading Configuration File that contains information
on the stores bacsy-server will backup. When starting, bacsy-server will read
either from stores.config in the directory given in a ``-c`` switch, or when
no such switch is given from stores.config in *$XDG_CONFIG_HOME/bacsy*, or
from *~/.config/bacsy/stores.config* when *$XDG_CONFIG_HOME* is not defined. 

Contents
========
.. include:: stores-contents.rst

.. include:: configuring-contents.rst
