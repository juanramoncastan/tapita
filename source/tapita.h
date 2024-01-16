//----------------------------------------------------------------------
//
//  Copyright (C) 2013 Juan Ramon Castan Guillen <juanramoncastan@yahoo.es>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//----------------------------------------------------------------------

/***************************************************************************
 *
 * Tapita version 0.3.1 alpha 8
 *
 * tapita.h
 *
 ***************************************************************************/

#define GTK_DISABLE_DEPRECATED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <sys/time.h>

#include "utils.h"

// Vars Declaration
struct timeval timeOfDay;


// Function Declaration


gboolean event_kbd (GtkWidget *widget, GdkEventKey *event, gpointer data);
void reset();
unsigned long get_time ();
unsigned int get_bpm(unsigned int millis);
void update(char *bpm, char *millis);
void quit();

