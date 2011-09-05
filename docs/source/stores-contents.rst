Every section name, except for ``global`` (which is optional), defines a new
data store. Store names obviously have to be unique. The following keys can
be defined:

Location
  **Default Value:** (none)

  **Semantics:** The base directory in which all backups and associated data
  will be stored. 

  **Example 1:** ::

    Location = /media/external_drive/backups

  Will store all backups this store accepts in
  ``/media/external_drive/backups``.

.. _MinPriorityForStoring:

MinPriorityForStoring
  **Default Value:** ?

  **Domain:** [0,10]

  **Semantics:** Determines how high a source's priority must be before it is
  accepted by this store. Only runs from sources with a *Priority* higher
  than or equal to (and thus a number that is smaller than or equal to) this
  store's MinPriorityForStoring will be stored. 


AlwaysPresent
  **Default Value:** True

  **Domain:** { True, False }

  **Semantics:** When AlwaysPresent is True, Bacsy will assume this store is
  permanently connected. If the directory given in ``Location`` does not
  exist, it will be created. When it cannot be created, an error will be
  produced. 

  If AlwaysPresent is False and the directory in ``Location`` cannot be found,
  this directory will not be created and the store will be silently ignored. 

Enabled
  **Default Value:** True

  **Domain:** { True, False }

  **Semantics:** If a store is disabled, nothing will ever be stored to it.
  Use this switch to keep the configuration for a store, without actually
  using it. 
