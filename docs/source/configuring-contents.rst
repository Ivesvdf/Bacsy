.. note:: The complete list of all configuration options can be found on :ref:`Configuration Files`
    

.. note:: All good ASCII art is courtesy of `Joan Stark <http://www.geocities.com/SoHo/7373>`_, all bad ASCII art is me changing her work for the worse.

Example 1
=========
The easiest possible setup: a single source and only one store::


                       .----.
           .---------. | == |
           |.-"""""-.| |----|
           ||       || | == |
           ||       || |----|
           |'-.....-'| |::::|
           `"")---(""` |___.|
          /:::::::::::\" _  "
         /:::=======:::\`\`\
    jgs `"""""""""""""`  '-'

             1 Source
             1 Store


The file ``sources.config`` will contain something like: ::

  [ entireHomeDirectory ]
  Include[] = /home/ives
  Exclude[] = *.bak
  Exclude[] = *~
  ExecuteAt = every wednesday at 21:00h


The file ``stores.config`` will contain something like: ::

  [ externalStore ]
  Location = /media/usbDrive/backup

This will backup all files in /home/ives, except for those matching ``*.bak`` or ``*~`` on every Wednesday at 8 p.m. It will store backups for them in ``/media/usbDrive/backup``. 


Example 2
=========

A setup where there is one "server" and one "workstation". All backups for the workstation will be stored on the server. The server itself does not have any backup sources. ::


           WORKSTATION                    SERVER
                       .----.            _________
           .---------. | == |           | _______o|
           |.-"""""-.| |----|           ||_______||
           ||       || | == |           | _______ |
           ||       || |----| ========= ||_______||        
           |'-.....-'| |::::|           |---------|
           `"")---(""` |___.|           |       O |
          /:::::::::::\" _  "           |       | |
         /:::=======:::\`\`\            |       | |
    jgs `"""""""""""""`  '-'            |:::::::::|

             1 Source                    0 Sources
             0 Store                     1 Store

On the Workstation
------------------
The file ``sources.config`` will contain something like: ::

  [ entireHomeDirectory ]
  Include[] = /home/ives
  Include[] = /home/naomi/importantFile
  Exclude[] = /home/ives/movies
  ExecuteAt = every 6 hours

No ``stores.config`` is present on the workstation. 

On the Server
-------------
The file ``stores.config`` will contain something like: ::

  [ externalStore ]
  Location = /media/bigdrive/

Conclusion
----------
This setup will backup all files and directories in ``/home/ives``, except for
those in ``/home/ives/movies``. It will also backup the file
``/home/naomi/importantFile``. The backup will run every six hours. The
workstation and server obviously have to be connected to the same LAN. 
