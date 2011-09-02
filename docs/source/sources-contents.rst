Every section name, except for ``global`` (which is optional), defines a new
data source. Source names obviously have to be unique. Each source may, but is
not required to, contain the following keys:

.. Order this stuff from most to least important/frequently used

Include[]
  **Default Value:** (not defined)

  **Semantics:** Each time this key is defined, the associated value is a
  *single directory or file*. This directory (and all its descendants) or file
  will be backed up when the source is run, unless it matches one or more
  `exclude rules`_. If the include is a directory, that directory
  does not match any exclude, but one of its descendants does, only that
  descendant will be excluded. 

  **Example 1:** ::

    Include[] = /home/ives
    Include[] = /home/naomi/
    Include[] = /home/john/.bashrc

  This example will include two directories, ``/home/ives`` and
  ``/home/naomi/``, and also a single file ``/home/john/.bashrc``. 

.. _exclude rules:

Exclude[]
  **Default Value:** (not defined)

  **Semantics:** (This stuff is a lot easier than it might seem, feel free to
  look at the examples at the bottom first). A file or directory is excluded
  by this rule, when it matches all of its non-negated subrules and does not
  match any of its negated subrules. 

  A subrule can be a simple path (pointing to a file or directory), it can be
  a path containing wildcards, and it can be a size exclusion subrule.
  Subrules can be unquoted, in which case only space and backslash (``\``) are
  escaped, both of them by prefixing with a backslash (``\``). They can also
  be quoted in double quotes (``"``), in which case both ``\`` and ``"`` have
  to be escaped with a ``\``.

  Subrules are separated by the symbol ``&`` and may be prefixed with a ``!`` to negate the subrule.  

  **Example 1:** ::

    Exclude[] = "/home/ives/some directory"
    Exclude[] = /home/ives/another\ directory
    Exclude[] = /home/ives/.vimrc
    Exclude[] = /home/ives/*.bak
    Exclude[] = *~

  The first exclude rule will exclude the directory
  ``/home/ives/some directory``. The second exclude rule will exclude the
  directory ``/home/ives/another directory``. The third one will exclude the
  file ``/home/ives/.vimrc``. The fourth one will exclude all files and
  directories ending in ``.bak`` in the directory ``/home/ives/```. The fifth
  and final one will exclude all files ending in ``~`` (e.g. ``bla.txt~``,
  ``data~``). 

  **Example 2:** ::

    Exclude[] = >50MB
    Exclude[] = <100B

  The first rule will exclude all files that are larger than 50 megabytes, the
  second rule will exclude all files smaller than 100 bytes. 
  

  **Example 3:** ::

    Exclude[] = *.avi & >500MB
    Exclude[] = /home/ives/*.vob & <1GB
    Exclude[] = /home/ives/movies & !/home/ives/movies/reallyGoodMovies

  The first exclusion rule excludes all avi files that are larger than 500MB.
  The second one excludes all vob files smaller than one gigabyte. The third
  one excludes all files in ``/home/ives/movies``, except for those in
  ``/home/ives/movies/reallyGoodMovies``. 

ExecuteAt
  **Default Value:** ?

  **Semantics:** Determines when a source will be executed. There can be
  multiple triggers, separated by the keyword ``and``. Some examples:

  **Example 1:** ::

    ExecuteAt = every week on Wednesday at 20h00

  Execute the source every week on Wednesday at 8 p.m.

  **Example 2:** ::

    ExecuteAt = every 3 hours

  Execute the source every three hours, the first time being 1 second after
  starting Bacsy. 

  **Example 3:** ::

    ExecuteAt = on start and every 1 minute and every day at 07:00h

  Execute the source every minute, when Bacsy start, and also every day at 7
  a.m. 


.. _priority:

Priority
  **Default Value:** ?
  
  **Domain:** [0-10]

  **Semantics:** A source's priority determines how important this source is.
  Higher priorities correspond to **lower** priority numbers. E.g. a source
  with Priority 5 is **less** important than a source with Priority 2. If you
  find this confusing, I don't blame you, but know that this was done with
  good reason; priorities in most systems, including operating systems work
  this way, so it would have been confusing either way. 

  A run will only be accepted by a store when the source's Priority is higher
  (thus the number is lower) than the store's `MinPriorityForStoring`_. 

  Use this key and the associated store key MinPriorityForStoring to make sure
  really important sources take precedence over less important sources when
  there isn't a lot of storage left. 


HostIdentification
  **Default Value:** Your computer's hostname. 

  **Semantics:** HostIdentification will be used by stores to categorize
  backups. When running several Bacsy clients on the same computer, set this
  key in the configuration to make sure things don't get messed up. This can
  happen easily when running Bacsy in both a Virtual PC and the host PC. 

MinBackups
  **Default Value:** 1
  
  **Domain:** [0 - 2 147 483 648[

  **Semantics:** The minimum number of stores Bacsy will try to copy the
  contents of this source to. When this number is not reached, an error will
  be produced. 

MaxBackups
  **Default Value:** 1
  
  **Domain:** [0 - 2 147 483 648[

  **Semantics:** The maximum number of stores Bacsy will try to copy the
  contents of this store to. Bacsy will never store more backups than this.
  Use this when you have a very large number of stores in the LAN. 


PreferredOrder
  **Default Value:** this, other

  **Semantics:** The order in which Bacsy will store to stores. ``this``
  refers to this  computer and ``other`` refers to all other computers.
  ``this, other`` will cause Bacsy to first try local stores, before resorting
  to stores connected to other PCs, resulting in speedier backups. ``other,
  this`` will make Bacsy connect to other PCs first, resulting in possibly
  better protected backups (against theft, fire, ...). 


Distribution
  **Default Value:** ?

  **Semantics:** Determines the distribution (spread) of where backups will be
  stored. Ceteris paribus, when storing 10 backups on 10 hosts, each of which
  has 10 stores, setting this option to ``spread`` will store 1 backup on each
  host. Setting it to ``focus`` will store all 10 backups on one host's
  stores. 

 
DryPrintRun
  **Default Value:** False

  **Domain:** { True, False }

  **Semantics:** When DryPrintRun is True, instead of sending files to hosts
  and their stores, the filenames of all files that *would* be sent are
  printed to standard output. 

  This option is most often used to fine tune your inclusion/exclusion rules.


Enabled
  **Default Value:** True

  **Domain:** { True, False }

  **Semantics:** If Enabled is False, the target won't actually be executed.
  Use this to (temporarily?) disable targets. 
