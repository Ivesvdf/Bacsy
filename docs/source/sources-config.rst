==============
Sources.config
==============

Synopsis
========
*$XDG_CONFIG_HOME/bacsy/sources.config*

*~/.config/bacsy/sources.config*

Description
===========
The *sources.conf* is a Cascading Configuration File that contains information
on the sources bacsy-client will backup. When starting, bacsy-client will read
either from sources.config in the directory given in a ``-c`` switch, or when
no such switch is given from sources.config in *$XDG_CONFIG_HOME/bacsy*, or
from *~/.config/bacsy/sources.config* when *$XDG_CONFIG_HOME* is not defined. 

Contents
========
.. include:: sources-contents.rst

.. include:: configuring-contents.rst
