# ----------------------------------------------------------------------
#
#  Copyright (C) 2013 Juan Ramon Castan Guillen <juanramoncastan@yahoo.es>
#    
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
# ----------------------------------------------------------------------

# ###########      tapita Makefile (builder)    ###########################


PREFIX = /usr
BIN_PATH = /bin
GCC = gcc
OPT = -g -Wall
CFLAGS = -lm -ljack -lasound `pkg-config --libs --cflags gtk+-3.0 gdk-3.0`
OBJS = utils.o tapita.o

tapita: directories $(OBJS)
	@ echo "Linking objects...$(OBJS) to executable $@"
	$(GCC) $(OPT) $(OBJS) $(CFLAGS) -o $@
	cp $@ ..$(BIN_PATH)

utils.o: utils.c
	@ echo "Compiling utils.c"
	$(GCC) $(OPT) -c $< -o $@

tapita.o: tapita.c
	@ echo "Compiling tapita.c"
	$(GCC) $(OPT) -c $<  $(CFLAGS) -o $@

directories:
	@ echo "Creating directories"
	@ mkdir -p ../bin

clean:
	rm *.o
	rm tapita

install: tapita
	cp tapita $(PREFIX)/bin/
	cp ../share/tapita.desktop $(PREFIX)/share/applications/
	cp ../share/tapita.svg $(PREFIX)/share/icons/
	cp -R ../share/tapita $(PREFIX)/share/

uninstall:
	rm  $(PREFIX)/bin/tapita
	rm  $(PREFIX)/share/applications/tapita.desktop
	rm  $(PREFIX)/share/icons/tapita.svg
	rm -fr $(PREFIX)/share/tapita




