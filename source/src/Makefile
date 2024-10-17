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

BIN_NAME = tapita
PREFIX = /usr
PREFIX = 
BIN_PATH = /bin
GCC = gcc
OPT = -g -Wall
CFLAGS = -lm -ljack -lasound `pkg-config --libs --cflags gtk+-3.0 gdk-3.0`
OBJS = utils.o tapita.o

tapita: $(OBJS)
	@ echo "Linking objects...$(OBJS) to executable $@"
	$(GCC) $(OPT) $(OBJS) $(CFLAGS) -o $@
	#mv $@ ..$(BIN_PATH)

utils.o: utils.c
	@ echo "Compiling utils.c"
	$(GCC) $(OPT) -c $< -o $@

tapita.o: tapita.c
	@ echo "Compiling tapita.c"
	$(GCC) $(OPT) -c $<  $(CFLAGS) -o $@

clean:
	rm *.o
	rm $(BIN_NAME)

prova:
ifeq ($(PREFIX),)
	@ echo "siiiii"
endif

#install:  tapita
#	cp $(BIN_NAME) $(PREFIX)/bin/
#	cp ../share/applications/tapita.desktop $(PREFIX)/share/applications/
#	cp ../share/icons/tapita.svg $(PREFIX)/share/icons/


#uninstall:
#	rm  $(PREFIX)/bin/tapita
#	rm  $(PREFIX)/share/applications/tapita.desktop
#	rm  $(PREFIX)/share/icons/tapita.svg





