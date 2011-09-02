============
Bacsy-Client
============

Synopsis
========
bacsy-client  [-D <string>] ...  [-c <string>] [--] [--version] [-h]

Description
===========
.. include:: client-description.rst

Options
=======
-D <string>,  --definition <string>  (accepted multiple times)
   Add a definition to the sources configuration. Format:
   [section]key=value

-c <string>,  --configdir <string>
   Directory in which to look for configuration files.

--,  --ignore_rest
   Ignores the rest of the labeled arguments following this flag.

--version
   Displays version information and exits.

-h,  --help
   Displays usage information and exits.

