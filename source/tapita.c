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

/***********************************************************************
 
 Tapita
 Version: 4.0-4
 
 tapita.c
 
 ***********************************************************************
 *   To build the file object "utils.o"
 *      gcc -g -Wall -c utils.c -o utils.o
 *
 *   To build the file object "tapita.o"
 *      gcc -g -Wall -c tapita.c -lm -ljack -lasound `pkg-config --libs --cflags gtk+-3.0` -o tapita.o
 *
 *   To link all objects and create binari file "tapita"
 *      gcc -g -Wall utils.o tapita.o -lm -ljack -lasound `pkg-config --libs --cflags gtk+-3.0` -o tapita
 *
 ***********************************************************************/

#include "tapita.h"
//#include "gthreadsupport.h"
//#include "buildconf.h"
//#include <gtk/gtk.h>



GtkWidget *mainWindow;
GtkWidget *boxMain;

GtkWidget *labelTapCount;
GtkWidget *labelTapMillis;

unsigned long initTime;


int tapCounter;


void reset()
{
    tapCounter = 0;
}

unsigned long get_time()
{
    unsigned long lastTime = 0;
    unsigned int  milliseconds = 0;
    gettimeofday(&timeOfDay,NULL);
    lastTime=((timeOfDay.tv_sec * 1000000) + timeOfDay.tv_usec)/1000;
    milliseconds = 0;
    if( tapCounter == 0 )
    {
        initTime=lastTime;
    }
    if( tapCounter > 0)
    {
        milliseconds=(lastTime-initTime)/(tapCounter);
    }
    
    return milliseconds;
}



float get_bpm(unsigned int milliseconds)
{
    float out;
    if( milliseconds > 0 )
    {
        out = (float)60000/milliseconds;
    }
    else
    {
        out = 0;
    }
    return  out;
}

gboolean event_kbd (GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    unsigned int milliseconds;
    unsigned long ms;
    float bpm;
    char *millis;
    char *bpmstr;
    millis="";
    bpmstr="";
    switch (event->keyval)
    {
        case GDK_KEY_space: // 0x020
            ms=1;
            milliseconds = get_time();
            bpm = get_bpm(milliseconds);
            if(bpm > 0)
            {
                ms = (int) 60000/bpm;
            }
            bpmstr = double_to_char( bpm, 1 );
            millis = double_to_char( ms, 0 );
            update(bpmstr, millis);
            tapCounter++;
            break;
        case GDK_KEY_Delete: // 0xffff
            reset();
            update("", "");
            break;
        case GDK_KEY_q: // 0x071
            if (event->state & GDK_CONTROL_MASK)
            {
                quit();
            }
            break;
        default:
            return FALSE;

    }
    return TRUE;
}

void update(char *bpm, char *millis)
{
    char *markup_bpm;
    char *markup_ms;
    const char *format_bpm = "<big><b>\%s</b></big> BPM";
    const char *format_ms = "\%s <small>ms.</small>";
    markup_bpm = g_markup_printf_escaped (format_bpm, bpm);
    markup_ms = g_markup_printf_escaped (format_ms, millis);
    gtk_label_set_markup( GTK_LABEL( labelTapCount ), markup_bpm  );
    gtk_label_set_markup( GTK_LABEL( labelTapMillis ), markup_ms  );

    g_free( markup_bpm );
    g_free( markup_ms );
}


int main(int argc, char *argv[]) // Function main()
{


    // Arguments input
    int arguments;
    while ((arguments = getopt (argc, argv, "h")) != -1)
    {
        switch (arguments)
        {
            case 'h':
                help_message();
                exit(0);
                break;
            case '?':
                help_message();
                exit(0);
                break;
        }
    }
    
    // Main window
    gtk_init(&argc, &argv);
    
    mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request (mainWindow, 400, 200 );
    
    boxMain = gtk_box_new (GTK_ORIENTATION_VERTICAL , 5);
    gtk_container_add (GTK_CONTAINER (mainWindow), boxMain);
    labelTapCount = gtk_label_new ("BPM");
    labelTapMillis = gtk_label_new ("ms.");

    
    gtk_box_pack_start (GTK_BOX (boxMain), labelTapCount, FALSE, FALSE, 20 );
    gtk_box_pack_start(GTK_BOX (boxMain), labelTapMillis, FALSE, FALSE, 20);
    //gtk_widget_set_halign(labelTapMillis, GTK_ALIGN_CENTER);

    gtk_widget_show_all(mainWindow);
    
    g_signal_connect(mainWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL); // Handle Main Window Destroy 
    g_signal_connect (mainWindow, "key_press_event", G_CALLBACK (event_kbd), NULL);  // Handle KBD
    gtk_main();
    return 0;
}

// Help output
void help_message() {
    printf("\n"  );
    // printf("tapita %s,", version  );
    printf(" (c) juan ramon castan\n"  );
    printf("\n"  );
    printf("Tapita (snack in spanish) is a BPM detector\n"  );
    printf("with alphanumeric keyboard");
    printf("\n"  );
    printf("usage: tapita [options]\n"  );
    printf("\n"  );
    printf("  options:\n"  );
    printf("\n"  );
	printf("    -h  Show this message.\n"  );
    printf("    -k  Set \"Space Bar\" as input. (default)\n"  );
	printf("    -a[TIME]  Enable Autoreset after TIME seconds. TIME is optional.\n"  );
    printf("\n"  );
    printf("  \"Space\"\tto tap.\n"  );
    printf("  \"Delete\"\treset.\n"  );
    printf("  \"Ctrl+Q\"\tquit application.\n"  );
    printf("\n"  );
}

// Quitting application
void quit(){
    printf("Exiting...\n");
    gtk_main_quit();
}
