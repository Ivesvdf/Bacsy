Welcome to Bacsy's documentation!
*********************************

For the complete documentation, see http://bacsy.readthedocs.org/en/latest/index.html. 

What is Bacsy?
==============

Bacsy is a *backup system* (hence the name).

Essential points:

   * Distributed
        A device using Bacsy to backup its data will send the backups
        to other devices also running Bacsy.

   * Easy to add a new device
        In a default configuration, if you add a new device running
        Bacsy to your LAN, backups will automatically be stored on
        that device.

   * Redundant
        Backups will usually be stored on multiple devices.

   * Fast to setup ...
        A basic configuration should be quick to achieve

   * ... yet extremely adjustable
        A plethora of configuration options are available.


How does Bacsy work?
====================

Bacsy is made up out of two components:
   * A Client
        The client generates backups and sends them to one or more
        servers.

   * A Server
        A server accepts and stores backups for one or more clients.

**Most systems will have both a client and a server running.** Systems
that have no need to generate backups of their own (e.g. dedicated
backup servers) will have only a server running.


Client
------

A client has a number of *sources*. Each source specifies what files
should be backed up, how often they should be backed up, how important
they are, etc.

When the client has a backup it needs stored, it will ask qualifying
servers if they can store this backup. If the servers respond they
can, the backup will be sent to them.


Server
------

A Bacsy server stores backups. Each server has one or more *stores*. A
*store* is a location where backups can be stored, usually some
directory on some drive. Backups will often be stored to multiple
stores on the same server, for redundancy purposes.


