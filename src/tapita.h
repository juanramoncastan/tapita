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
#include <gdk/gdkkeysyms.h>
#include <alsa/asoundlib.h>
#include <sys/time.h>
#include <jack/jack.h>
#include "utils.h"



// Function Declaration

void reset ();

double get_time ();

void on_event (double now);

void calculate_note_time();

gboolean event_kbd (GtkWidget *widget, GdkEventKey *event, gpointer data);

void event_midi(snd_seq_t *seq_handle );

static gboolean update_widgets();

static gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event,  gpointer data);

void draw_deviation();

void draw_meter();

void draw_edit(GtkWidget *area);

void set_threshold (GtkRange *range);

void set_autoreset(GtkWidget *button, gpointer data);

void change_input (GtkWidget *button, gpointer data);

gboolean edit_bpm (GtkWidget *widget, GdkEvent * event , gpointer data);

gboolean on_change_bpm (GtkWidget *widget,  gpointer data);

gboolean bpm_changed(GtkWidget *widget,  gpointer data);

gboolean edit_time_reset (GtkWidget *widget, GdkEvent * event , gpointer data);

gboolean on_change_time_reset (GtkWidget *widget,  gpointer data);

gboolean time_reset_changed(GtkWidget *widget,  gpointer data);

gboolean end_edit (GtkWidget *widget,  gpointer data);

void connect_port (GtkWidget *combo, gpointer data);

int create_ports();

void scrolled_ports (GtkWidget *widget, GdkEvent * event , gpointer data);

void create_notation();

void change_notation(GtkWidget *combo, gpointer data);

void jack_fail();

char *get_note_midi(int nt);

snd_seq_t *open_sequencer();

void jack_init();

int jack_process(jack_nframes_t nframes, void *arg );

void jack_shutdown (void *arg);

void help_message();

void quit();


// Global Variables

char *version;
bool startGTK;


int note_event;
char notePiano[6];

jack_port_t *inputPort;
jack_client_t *client;
jack_status_t jackStatus;
int sampleRate;
int jackWaitPeaks;
int timeWaitReset;
int minSliderValue;
float jackThreshold;
bool jackEvent;
float inputPeak;
float oldPeak;

int tapCounter;
int totalBPM;
int lastDeviation;
double timeOld , timeInit;
double beatTime;
double noteTime;
double avarageTime;
unsigned long countTimeSamples;
struct timeval timeOfDay;

bool enabledAutoreset;
bool enabledKbd;
bool enabledMidi;
bool enabledJack;
int activePortIndex;
float notationValue;

char iconEditFile[256];

bool ponderado;




