# tapita
----------------------------------------------------------------------
Tapita
Version: 4.0-5
----------------------------------------------------------------------
Tapita (snack in spanish) is a BPM detector
Tapita uses alphanumeric keyboard, MIDI or audio via jack as inputs
to detect pulses.

Web: It is not ready yet

USAGE
------------------------------------------------------------------------
tapita [options]
  
    -h          Show this message.
    -k          Set "Space Bar" as input. (default)


Keybindings
    "Space"     to tap.
    "Delete"    reset.
    "Ctrl+q"    Quit application.


BUILDING DEPENDENCIES
------------------------------------------------------------------------
You may need to install the next (developer) packages

libgtk2.0 (developer)
libasound (developer)
libjack (developer)


COMPILATION
------------------------------------------------------------------------
go to source directory

$ cd ./src

build application

$ make


INSTALATION
------------------------------------------------------------------------
To install into the system (as root).
By default PREFIX is /usr but you can change it in Makefile.

$ make install


CLEANING and UNINSTALL
------------------------------------------------------------------------
To clean the last compilation

$ make clean

To uninstall from system (as root)

$ make uninstall




