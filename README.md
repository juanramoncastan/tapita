# tapita
----------------------------------------------------------------------
Tapita version 0.3.1
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
    -j          Set "jack" as input.
    -m          Set MIDI as input.
    -p          Last beat is weighted in the avarage time.
    -a[TIME]    Enable Autoreset after TIME seconds. TIME is optional.

  BPM and Autoreset Time labels are editable.
  Jack client will be active when selected as input.

    "Space"     to tap.
    "Delete"    reset.
    "PageUp"    raise jack input threshold.
    "PageDown"  drop jack input threshold.
    "A"         Toggle Autoreset.
    "J"         Set JACK as input.
    "K"         Set "Space Bar" as input.
    "M"         Set MIDI as input.
    "Ctrl+Q"    Quit application.


BUILDING DEPENDENCIES
------------------------------------------------------------------------
You may need to install the next headers (developer) packages

libgtk-2.0 (developer)
libasound (developer)
libjack (developer)


COMPILATION
------------------------------------------------------------------------
go source directory

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




