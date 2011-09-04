.. Bacsy documentation master file, created by
   sphinx-quickstart on Wed Aug 24 18:58:24 2011.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

=================================
Welcome to Bacsy's documentation!
=================================

Getting Started
===============

 * `What is Bacsy?`_
 * `How does Bacsy work?`_
 * :ref:`Compiling the source`
 * :ref:`Configuring Bacsy`


What is Bacsy?
==============
Bacsy is a *backup system* (hence the name). 

Essential points:

 * Distributed 
     A device using Bacsy to backup its data will send the backups to other
     devices also running Bacsy. 
 * Easy to add a new device
     In a default configuration, if you add a new device running Bacsy to your
     LAN, backups will automatically be stored on that device. 
 * Redundant
     Backups will usually be stored on multiple devices. 
 * Fast to setup ...
     A basic configuration should be quick to achieve
 * ... yet extremely adjustable
     A plethora of configuration options are available. 

How does Bacsy work?
====================
Bacsy is made up out of two components:
 * `A Client <#client>`_
     The client generates backups and sends them to one or more servers. 
 * `A Server <#server>`_
     A server accepts and stores backups for one or more clients. 

**Most systems will have both a client and a server running.** Systems that have no need to generate backups of their own (e.g. dedicated backup servers) will have only a server running. 



Client
------
.. include:: client-description.rst


Server
------
.. include:: server-description.rst

 
Contents
========

.. toctree::
   :maxdepth: 2

   configurationfiles
   building
   configuring

Indices and tables
==================

* :ref:`genindex`
* :ref:`search`

