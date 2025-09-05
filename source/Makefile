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
# Version: 4.0-5

#####  BUILD = "../package-name_version_architecture" given from "debianizador" script
PREFIX = /usr
SRC_PATH = /src
BIN_PATH = /bin
SHARE_PATH = /share


test:
	@ echo User: $(USER)


build:
	@ echo "Building"
	@ echo "in root directory: $(BUILD)"
	# It calls Makefile to compile the binary and 
	cd .$(SRC_PATH) && $(MAKE)


preparebin:
	@ echo "Creating directories"
	mkdir -p .$(BIN_PATH)
	cp .$(SRC_PATH)/tapita .$(BIN_PATH)


clean:
	@ echo "Cleaning"
	rm .$(BIN_PATH)/tapita
	cd .$(SRC_PATH) && $(MAKE) clean
	

install: test build preparebin
ifneq ($(PREFIX),)
	mkdir -p $(BUILD)$(PREFIX)$(BIN_PATH)
	mkdir -p $(BUILD)$(PREFIX)$(SHARE_PATH)/applications
	mkdir -p $(BUILD)$(PREFIX)$(SHARE_PATH)/icons
	cp .$(BIN_PATH)/tapita $(BUILD)$(PREFIX)$(BIN_PATH)
	cp .$(SHARE_PATH)/applications/* $(BUILD)$(PREFIX)$(SHARE_PATH)/applications
	cp .$(SHARE_PATH)/icons/* $(BUILD)$(PREFIX)$(SHARE_PATH)/icons
endif


uninstall:
	rm $(BUILD)$(PREFIX)$(BIN_PATH)/tapita
	rm $(BUILD)$(PREFIX)$(SHARE_PATH)/applications/tapita*
	rm $(BUILD)$(PREFIX)$(SHARE_PATH)/icons/tapita*




