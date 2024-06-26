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

# ###########      tapita Makefile (toplevel)     ###########################


# BUILD = "../package-name_version_architecture" given from "deb_package_creator" app
PREFIX = /usr
SRC_PATH = /source
BIN_PATH = /bin
SHARE_PATH = /share

nothing:
	@ echo "Test"

build:
	@ echo "Building"
	cd .$(SRC_PATH) && $(MAKE)

clean:
	@ echo "Cleaning"
	rm .$(BIN_PATH)/tapita
	cd .$(SRC_PATH) && $(MAKE) clean
	

install: build  
	mkdir -p $(BUILD)$(PREFIX)$(BIN_PATH)
	mkdir -p $(BUILD)$(PREFIX)$(SHARE_PATH)/applications
	mkdir -p $(BUILD)$(PREFIX)$(SHARE_PATH)/icons
	cp .$(BIN_PATH)/tapita $(BUILD)$(PREFIX)$(BIN_PATH)
	cp .$(SHARE_PATH)/applications/* $(BUILD)$(PREFIX)$(SHARE_PATH)/applications
	cp .$(SHARE_PATH)/icons/* $(BUILD)$(PREFIX)$(SHARE_PATH)/icons

uninstall:
	rm $(BUILD)$(PREFIX)$(BIN_PATH)/tapita
	rm $(BUILD)$(PREFIX)$(SHARE_PATH)/applications/tapita*
	rm $(BUILD)$(PREFIX)$(SHARE_PATH)/icons/tapita*




