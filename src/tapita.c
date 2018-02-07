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
 *
 * Tapita version 0.3.1 alpha 8
 *
 * tapita.c
 *
 *   To build the file object "tapita.o"
 *      gcc -g -Wall -c tapita.c -lm -ljack -lasound
 * 				`pkg-config --libs --cflags gtk+-2.0` -o tapita.o
 *
 *   To link all objects and create binari file "tapita"
 *      gcc -g -Wall utils.o tapita.o -lm -ljack -lasound
 * 				`pkg-config --libs --cflags gtk+-2.0` -o tapita
 *
 ***********************************************************************/

#include "tapita.h"
#include "gthreadsupport.h"
#include "buildconf.h"

GtkWidget *mainWindow;
GtkWidget *eventMain;
GtkWidget *hboxMain;
GtkWidget *vboxLeft;
GtkWidget *vboxCenter;
GtkWidget *vboxRight;



GtkWidget *hboxBPM;
GtkWidget *eventBpm;
GtkWidget *hboxBeat;
GtkWidget *hboxBeat4;
GtkWidget *frameDeviation;
GtkWidget *hboxTaps;
GtkWidget *areaEditBpm;
GtkAdjustment *adjustBPM;
GtkWidget *entryBPM;
GtkWidget *etiqBpmValue;
GtkWidget *etiqBpmUnit;
GtkWidget *comboNotation;
GtkListStore *listNotation;
GtkCellRenderer *cellNotation;
GtkWidget *etiqBeatValue;
GtkWidget *etiqBeatUnit;
GtkWidget *drawAreaDeviation;
GtkWidget *etiqTapName;
GtkWidget *etiqTapCount;
GtkWidget *buttonReset;

GtkWidget *hboxThreshold;
GtkWidget *sliderThreshold;
GtkWidget *vboxMeter;
GtkWidget *frameMeter;
GtkWidget *drawAreaMeter;

GtkWidget *frameSetup;
GtkWidget *checkAutoreset;
GtkWidget *hboxTimeReset;
GtkWidget *eventTimeReset;
GtkWidget *areaEditTimeReset;
GtkWidget *etiqTimeResetName;
GtkAdjustment *adjustTimeReset;
GtkWidget *entryTimeReset;
GtkWidget *etiqTimeResetValue;
GtkWidget *etiqTimeResetUnit;
GtkWidget *radioJack;
GtkWidget *comboPorts;
GtkListStore *listPorts;
GtkCellRenderer *cellPorts;
GtkWidget *radioKbd;
GtkWidget *hboxSpacebar;
GtkWidget *etiqSpacebar;
GtkWidget *radioMidi;
GtkWidget *hboxMidi;
GtkWidget *etiqMidi;
GtkWidget *etiqNote;
GtkWidget *hboxQuit;
GtkWidget *buttonQuit;

int main(int argc, char *argv[]) // Function main()
{

    version="0.3.1-alpha8";
    client=NULL;
    startGTK=true;
    inputPort = NULL;
    tapCounter=0;
    totalBPM=0;
    avarageTime=0;
    lastDeviation=0;
    timeOld=0;
    timeInit=0;
    beatTime=0;
    noteTime=0;
    note_event= -1;
    timeWaitReset=3; // Seconds
    enabledAutoreset = false;
    enabledKbd = true;
    enabledMidi = false;
    enabledJack = false;
    jackEvent=false;
    jackThreshold=db2linear(-20); // db
    minSliderValue=-40; // db
    countTimeSamples=0;
    inputPeak=0;
    oldPeak=0;
    activePortIndex=-1;
    notationValue=1;
    strcpy(iconEditFile, prefix);
    strcat(iconEditFile,"/share/tapita/edit-icon.png");
    ponderado=FALSE;



    // Arguments input
    int arguments;
    while ((arguments = getopt (argc, argv, "pkjmha::")) != -1)
    {
        int seconds;
        switch (arguments)
        {
            case 'a':
                enabledAutoreset = true;
                if (optarg)
                {
                    seconds=atoi(optarg) ;
                    if (seconds > 0 )
                    {
                        timeWaitReset=seconds;
                    }
                }
                break;
            case 'k':
                enabledKbd = true;
                enabledJack = false;
                enabledMidi = false;
                break;
            case 'j':
                enabledKbd = false;
                enabledJack = true;
                enabledMidi = false;
                break;
            case 'm':
                enabledKbd = false;
                enabledJack = false;
                enabledMidi = true;
                break;
            case 'p':
                ponderado=TRUE;
                break;
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

    // Sequencer Handler
    snd_seq_t *seq_handle;
    struct pollfd *pfd;
    int npfd;
    seq_handle = open_sequencer();
    npfd = snd_seq_poll_descriptors_count(seq_handle, POLLIN);
    pfd = (struct pollfd *)alloca(npfd * sizeof(struct pollfd));
    snd_seq_poll_descriptors(seq_handle, pfd, npfd, POLLIN);

    // Init GTK
    gtk_init(&argc, &argv);
    g_thread_support();
    gdk_threads_init();
    // Main build
    mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    eventMain = gtk_event_box_new ();
    gtk_container_add (GTK_CONTAINER (mainWindow), eventMain);
    hboxMain = gtk_hbox_new (FALSE , 5);
    gtk_container_add (GTK_CONTAINER (eventMain), hboxMain);
    vboxLeft = gtk_vbox_new (FALSE , 5);
    gtk_box_pack_start (GTK_BOX (hboxMain), vboxLeft, TRUE, TRUE, 5 );
    vboxCenter = gtk_vbox_new (FALSE , 5);
    gtk_box_pack_start (GTK_BOX (hboxMain), vboxCenter, FALSE, FALSE, 5 );

    // Left build
    // BPM
    hboxBPM = gtk_hbox_new (FALSE , 5);
    gtk_box_pack_start (GTK_BOX (vboxLeft), hboxBPM, FALSE, FALSE, 5 );
    adjustBPM = (GtkAdjustment *) gtk_adjustment_new (0.0, 0.0, 1000, 1, 0, 0);
    entryBPM = gtk_spin_button_new (GTK_ADJUSTMENT(adjustBPM), 1.0, 1.0);
    gtk_spin_button_set_digits(GTK_SPIN_BUTTON(entryBPM),0);
    gtk_box_pack_start (GTK_BOX (hboxBPM), entryBPM, TRUE, TRUE, 5);
    char *bpm = double_to_char(totalBPM,0);
    etiqBpmValue = gtk_label_new (bpm);
    gtk_box_pack_start (GTK_BOX (hboxBPM), etiqBpmValue, TRUE, TRUE, 5);
    gtk_widget_modify_font (etiqBpmValue, pango_font_description_from_string ("16") );
    eventBpm = gtk_event_box_new ();
    gtk_box_pack_end (GTK_BOX (hboxBPM), eventBpm, FALSE, FALSE, 5);
    etiqBpmUnit = gtk_label_new ("BPM");
    gtk_box_pack_end (GTK_BOX (hboxBPM), etiqBpmUnit, FALSE, FALSE, 5);
    areaEditBpm = gtk_drawing_area_new();
    gtk_container_add (GTK_CONTAINER (eventBpm), areaEditBpm);
    // Beat
    hboxBeat = gtk_hbox_new (FALSE , 5);
    gtk_box_pack_start (GTK_BOX (vboxLeft), hboxBeat, FALSE, FALSE, 5 );
    listNotation = gtk_list_store_new( 1, G_TYPE_STRING );
    comboNotation= gtk_combo_box_new_with_model( GTK_TREE_MODEL( listNotation ) );
    g_object_unref( G_OBJECT( listNotation ) );
    gtk_box_pack_start (GTK_BOX (hboxBeat), comboNotation, FALSE, FALSE, 5 );
    create_notation();
    cellNotation = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start( GTK_CELL_LAYOUT( comboNotation ), cellNotation, TRUE );
    gtk_cell_layout_set_attributes( GTK_CELL_LAYOUT( comboNotation ), cellNotation, "text", 0, NULL );
    char *notetime = double_to_char(noteTime,1);
    etiqBeatValue = gtk_label_new (notetime);
    etiqBeatUnit = gtk_label_new ("ms.");
    gtk_box_pack_end (GTK_BOX (hboxBeat), etiqBeatUnit, FALSE, FALSE, 5 );
    gtk_box_pack_end (GTK_BOX (hboxBeat), etiqBeatValue, FALSE, FALSE, 5 );
    // Deviation DrawArea
    frameDeviation = gtk_frame_new("deviation");
    gtk_box_pack_start (GTK_BOX (vboxLeft), frameDeviation, FALSE, FALSE, 5 );
    drawAreaDeviation = gtk_drawing_area_new();
    gtk_container_add (GTK_CONTAINER (frameDeviation), drawAreaDeviation );
    // Taps
    hboxTaps = gtk_hbox_new (FALSE , 5);
    gtk_box_pack_start (GTK_BOX (vboxLeft), hboxTaps, FALSE, FALSE, 5 );
    etiqTapName = gtk_label_new ("Beats");
    gtk_box_pack_start (GTK_BOX (hboxTaps), etiqTapName, FALSE, FALSE, 5 );
    char *counter = double_to_char(tapCounter, 0);
    etiqTapCount = gtk_label_new (counter);
    gtk_box_pack_start (GTK_BOX (hboxTaps), etiqTapCount, TRUE, TRUE, 5 );
    // Reset Button
    buttonReset = gtk_button_new_with_label ("Reset");
    gtk_box_pack_end (GTK_BOX (vboxLeft), buttonReset , FALSE, FALSE, 5 );

    // Center build
    hboxThreshold = gtk_hbox_new (FALSE , 5);
    gtk_box_pack_start (GTK_BOX (vboxCenter), hboxThreshold, TRUE, TRUE, 15 );
    // Threshold Slider
    sliderThreshold = gtk_vscale_new_with_range(minSliderValue,0,0.05);
    gtk_box_pack_start (GTK_BOX (hboxThreshold), sliderThreshold, FALSE, FALSE, 0 );
    gtk_range_set_inverted (GTK_RANGE(sliderThreshold), TRUE);
    gtk_scale_set_draw_value (GTK_SCALE(sliderThreshold), FALSE);
    gtk_range_set_value (GTK_RANGE(sliderThreshold), linear2db(jackThreshold));
    // Meter
    vboxMeter = gtk_vbox_new (FALSE,5);
    gtk_box_pack_start (GTK_BOX (hboxThreshold), vboxMeter, TRUE, TRUE, 0 );
    frameMeter = gtk_frame_new(NULL);
    gtk_box_pack_start (GTK_BOX (vboxMeter), frameMeter, TRUE, TRUE, 5 );
    drawAreaMeter = gtk_drawing_area_new();
    gtk_container_add (GTK_CONTAINER (frameMeter), drawAreaMeter );

    // Right buid
    frameSetup = gtk_frame_new("setup");
    gtk_box_pack_start (GTK_BOX (hboxMain), frameSetup, FALSE, FALSE, 5 );
    vboxRight = gtk_vbox_new (FALSE , 10 );
    gtk_container_add  (GTK_CONTAINER (frameSetup), vboxRight);
    // Quit Widgets
    hboxQuit = gtk_hbox_new (FALSE , 0);
    gtk_box_pack_end (GTK_BOX (vboxRight), hboxQuit, FALSE, FALSE, 0 );
    buttonQuit = gtk_button_new_with_label ("Quit");
    gtk_box_pack_start (GTK_BOX (hboxQuit), buttonQuit , TRUE, FALSE, 5 );
    // Midi widgets
    hboxMidi = gtk_hbox_new (FALSE , 0);
    gtk_box_pack_end (GTK_BOX (vboxRight), hboxMidi, FALSE, FALSE, 0 );
    etiqMidi = gtk_label_new ("Midi Note: ");
    gtk_box_pack_start (GTK_BOX (hboxMidi), etiqMidi, FALSE, FALSE, 20 );
    etiqNote = gtk_label_new (get_note_midi(-1));
    gtk_box_pack_start (GTK_BOX (hboxMidi), etiqNote, FALSE, FALSE, 5 );
    radioMidi = gtk_radio_button_new_with_label(NULL,"Midi input");
    gtk_box_pack_end (GTK_BOX (vboxRight), radioMidi, FALSE, FALSE, 0 );
    // Keyboard Widgets
    hboxSpacebar = gtk_hbox_new (FALSE , 0);
    gtk_box_pack_end (GTK_BOX (vboxRight), hboxSpacebar, FALSE, FALSE, 0 );
    etiqSpacebar = gtk_label_new("Space Bar");
    gtk_box_pack_start (GTK_BOX (hboxSpacebar), etiqSpacebar, FALSE, FALSE, 20 );
    radioKbd = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radioMidi),"Keyboard input");
    gtk_box_pack_end (GTK_BOX (vboxRight), radioKbd, FALSE, FALSE, 0 );
    // Jack Widgets
    listPorts = gtk_list_store_new( 1, G_TYPE_STRING );
    comboPorts= gtk_combo_box_new_with_model( GTK_TREE_MODEL( listPorts ) );
    g_object_unref( G_OBJECT( listPorts ) );
    gtk_box_pack_end (GTK_BOX (vboxRight), comboPorts, FALSE, FALSE, 0 );
    cellPorts = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start( GTK_CELL_LAYOUT( comboPorts ), cellPorts, TRUE );
    gtk_cell_layout_set_attributes( GTK_CELL_LAYOUT( comboPorts ), cellPorts, "text", 0, NULL );
    radioJack = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radioMidi),"Jack input");
    gtk_box_pack_end (GTK_BOX (vboxRight), radioJack, FALSE, FALSE, 0 );
    // Autoreset widgets
    hboxTimeReset = gtk_hbox_new (FALSE , 0);
    gtk_box_pack_end (GTK_BOX (vboxRight), hboxTimeReset, FALSE, FALSE, 0 );
    etiqTimeResetName = gtk_label_new ("after: ");
    gtk_box_pack_start (GTK_BOX (hboxTimeReset), etiqTimeResetName, FALSE, FALSE, 20 );
    adjustTimeReset =(GtkAdjustment *) gtk_adjustment_new (timeWaitReset, 1, 1000, 1, 0, 0);
    entryTimeReset = gtk_spin_button_new (GTK_ADJUSTMENT(adjustTimeReset), 1.0, 1.0);
    gtk_box_pack_start (GTK_BOX (hboxTimeReset), entryTimeReset, FALSE, FALSE, 5 );
    gtk_spin_button_set_digits(GTK_SPIN_BUTTON(entryTimeReset),0);
    char *waitreset = double_to_char(timeWaitReset,0);
    etiqTimeResetValue = gtk_label_new (waitreset);
    gtk_box_pack_start (GTK_BOX (hboxTimeReset), etiqTimeResetValue, TRUE, TRUE, 5 );
    etiqTimeResetUnit = gtk_label_new ("sec.");
    gtk_box_pack_start (GTK_BOX (hboxTimeReset), etiqTimeResetUnit, FALSE, FALSE, 5 );
    eventTimeReset = gtk_event_box_new ();
    gtk_box_pack_end (GTK_BOX (hboxTimeReset), eventTimeReset, FALSE, FALSE, 5 );
	areaEditTimeReset = gtk_drawing_area_new();
    gtk_container_add (GTK_CONTAINER (eventTimeReset), areaEditTimeReset);
    checkAutoreset =  gtk_check_button_new_with_label ("Auto Reset");
    gtk_box_pack_end (GTK_BOX (vboxRight), checkAutoreset, FALSE, FALSE, 0 );

    // Signals
    g_signal_connect ( G_OBJECT (mainWindow), "destroy",  G_CALLBACK (quit), NULL ); // Main Quit Signal
    g_signal_connect(frameDeviation, "expose-event",  G_CALLBACK(on_expose_event), NULL); // Needed to drawingAreas
    g_signal_connect (mainWindow, "key_press_event", G_CALLBACK (event_kbd), NULL);  // Handle KBD
    gdk_input_add ( pfd[0].fd, GDK_INPUT_READ, (GdkInputFunction)event_midi, seq_handle ); // Handle MIDI
    g_signal_connect (GTK_RANGE (sliderThreshold), "value_changed",  G_CALLBACK (set_threshold), NULL);

    g_signal_connect ( GTK_WIDGET(eventBpm), "button-press-event",  G_CALLBACK (edit_bpm), NULL ); // Edit BPM SpinButton
    g_signal_connect (GTK_ENTRY(entryBPM), "activate", G_CALLBACK (on_change_bpm), NULL); //
    g_signal_connect (entryBPM, "value-changed", G_CALLBACK (bpm_changed), NULL);
    g_signal_connect ( GTK_WIDGET(eventTimeReset), "button-press-event",  G_CALLBACK (edit_time_reset), NULL );
    g_signal_connect (GTK_ENTRY(entryTimeReset), "activate", G_CALLBACK (on_change_time_reset), NULL);
    g_signal_connect (entryTimeReset, "value-changed", G_CALLBACK (time_reset_changed), NULL);
    g_signal_connect ( GTK_WIDGET(mainWindow), "button-press-event",  G_CALLBACK (end_edit), NULL ); // End edit spinbuttons

    g_signal_connect (G_OBJECT (buttonReset), "clicked", G_CALLBACK (reset), NULL );
    g_signal_connect (GTK_TOGGLE_BUTTON (checkAutoreset), "toggled", G_CALLBACK (set_autoreset), NULL );
    g_signal_connect (GTK_TOGGLE_BUTTON (radioKbd), "toggled", G_CALLBACK (change_input), NULL );
    g_signal_connect (GTK_TOGGLE_BUTTON (radioJack), "toggled", G_CALLBACK (change_input), NULL );
    g_signal_connect (GTK_TOGGLE_BUTTON (radioMidi), "toggled", G_CALLBACK (change_input), NULL );
    g_signal_connect (GTK_COMBO_BOX (comboPorts), "changed", G_CALLBACK (connect_port), NULL );
    g_signal_connect (GTK_WIDGET (comboPorts), "scroll-event", G_CALLBACK (scrolled_ports), NULL );
    g_signal_connect (GTK_COMBO_BOX (comboNotation), "changed", G_CALLBACK (change_notation), NULL );
    g_signal_connect (G_OBJECT (buttonQuit), "clicked", G_CALLBACK (quit), NULL );

    // Show and set widgets size
    gtk_widget_show_all(mainWindow);
    gtk_widget_hide(entryBPM); // Hide BPM spin
    gtk_widget_hide(entryTimeReset); // Hide Autoreset Time spin
    gtk_widget_set_size_request (frameDeviation, 250, 90 );
    gtk_widget_set_size_request (frameMeter, 15, 260 );
    gtk_widget_set_size_request (frameSetup, 200, -1 );
    gtk_widget_set_size_request (areaEditBpm, 24, 24 );
    gtk_widget_set_size_request (areaEditTimeReset, 24, 24 );


    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(checkAutoreset),enabledAutoreset);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(radioKbd),enabledKbd);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(radioJack),enabledJack);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(radioMidi),enabledMidi);

    free(bpm);
    free(notetime);
    free(counter);
    free(waitreset);

    g_timeout_add(60, update_widgets, NULL); //  Loop function thread every X ms.

    // Loop GTK Main
    if (startGTK==true) gtk_main();
    return 0;
}

// Reset initial variables
void reset ()
{
    tapCounter=0;
    totalBPM=0;
    avarageTime=0;
    lastDeviation=0;
    timeOld=0;
    timeInit=0;
    beatTime=0;
    noteTime=0;
    note_event= -1;

}

// Get time when an event happens, but only on_event() increments tapCounter.
double get_time ()
{
    unsigned long now;
    gettimeofday(&timeOfDay,NULL);
    now=((timeOfDay.tv_sec * 1000000) + timeOfDay.tv_usec)/1000;
    if ( now-timeOld >= timeWaitReset*1000 && enabledAutoreset==TRUE)
    {
        reset();
    }

    return now;
}

// Called when an event ( jack, midi or keyboard ) happens
void on_event (double now)
{
    double dif;
    if (timeInit == 0)
    {
        timeInit=now;
        dif=0;
    }
    else
    {
        dif=now-timeOld;
    }
    if ( tapCounter > 0 )
    {
        avarageTime=(now-timeInit)/tapCounter;
		if (ponderado == TRUE)
		{
			avarageTime=(dif+avarageTime)/2;
		}
        if (avarageTime > 0)
        {
            totalBPM=60000/avarageTime;
            calculate_note_time();
            lastDeviation = 100-((dif/beatTime)*100);
        }
    }
    timeOld=now;
    tapCounter++;
}

// We need because not only on_event() uses it.
// But also on_changes_bpm() and change_notation()
void calculate_note_time()
{
    if (totalBPM > 0) beatTime = 60000/totalBPM;
    noteTime=beatTime*notationValue;
}

// Callback for "key_press_event" in mainWindow
gboolean event_kbd (GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    double now;
    float sliderThresValue;
    sliderThresValue = gtk_range_get_value (GTK_RANGE(sliderThreshold));
    switch (event->keyval)
    {
        case GDK_space: // 0x020
            if ( enabledKbd == true)
            {
                now=get_time();
                on_event(now);
            }
            break;
        case GDK_Delete: // 0xffff
            reset();
            break;
        case GDK_Page_Up: // 0xff55
            gtk_range_set_value (GTK_RANGE(sliderThreshold), sliderThresValue+1);
            break;
        case GDK_Page_Down: // 0xff56
            gtk_range_set_value (GTK_RANGE(sliderThreshold), sliderThresValue-1);
            break;
        case GDK_a: // 0x061
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkAutoreset),
                        !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkAutoreset)));
            break;
        case GDK_j: // 0x06a
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radioJack),TRUE);
            break;
        case GDK_k: // 0x06b
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radioKbd),TRUE);
            break;
        case GDK_m: // 0x06d
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radioMidi),TRUE);
            break;
        case GDK_A: // 0x041
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkAutoreset),
                        !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkAutoreset)));
            break;
        case GDK_J: // 0x04a
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radioJack),TRUE);
            break;
        case GDK_K: // 0x04b
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radioKbd),TRUE);
            break;
        case GDK_M: // 0x04d
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radioMidi),TRUE);
            break;
        case GDK_q: // 0x071
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

// Callback for signal added by gdk_input_add() poll sequencer handler
void event_midi(snd_seq_t *seq_handle )
{
    double now;
    snd_seq_event_t *ev;
    do
    {
        snd_seq_event_input(seq_handle, &ev);
        if (ev->type == SND_SEQ_EVENT_NOTEON && ev->data.note.velocity > 0 )
        {
            now=get_time();
            if ( enabledMidi == true)
            {
                if ( note_event == ev->data.note.note)
                {
                    on_event(now);
                }
                else if ( note_event == -1 )
                {
                    get_note_midi(note_event = ev->data.note.note);
                    on_event(now);
                }
            }
        }
        snd_seq_free_event(ev);
    } while (snd_seq_event_input_pending(seq_handle, 0) > 0);
}

// Only called by g_timeout_add() loop
static gboolean update_widgets()
{
    char dev[24]="Deviation: ";
    char *desviation =NULL;
    char *bpm =NULL;
    char *notetime =NULL;
    char *counter =NULL;
    if (lastDeviation >= 0 ) strcat(dev," ");
    desviation = double_to_char(lastDeviation,0);
    strcat(dev,desviation);
    strcat(dev," ");
    strcat(dev,"%");
    gdk_threads_enter();
    bpm =  double_to_char(totalBPM, 0);
    notetime =  double_to_char(noteTime, 1);
    counter =  double_to_char(tapCounter, 0);
    gtk_label_set_text ( GTK_LABEL (etiqBpmValue) ,bpm);
    gtk_label_set_text ( GTK_LABEL (etiqBeatValue) , notetime);
    // Next is important because calls repeatly on_expose_event()
    gtk_frame_set_label ( GTK_FRAME (frameDeviation) , dev);
    gtk_label_set_text ( GTK_LABEL (etiqTapCount) , counter);
    gtk_label_set_text ( GTK_LABEL (etiqNote)  , get_note_midi(note_event));
    if (inputPeak >= db2linear(minSliderValue))
    {
        if ( inputPeak <= oldPeak ) inputPeak = db2linear(linear2db(inputPeak)-1)-0.005;
    }
    else inputPeak = 0;
    oldPeak=inputPeak;
    gdk_threads_leave();
    free(desviation);
    free(bpm);
    free(notetime);
    free(counter);
    return TRUE;
}

// Needed for draw and redraw drawingAreas
static gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event,  gpointer data)
{
    draw_deviation();
    draw_meter();
    draw_edit(areaEditBpm);
    draw_edit(areaEditTimeReset);
    return FALSE;
}

// Draws semicircle of deviation
void draw_deviation()
{
    int desv = lastDeviation;
    float arcinit=0;
    float arcend=0;
    float arccenter=(PI/2)*3;
    GtkAllocation rectangle = drawAreaDeviation->allocation;
    int origX, origY;
    origX = rectangle.width/2;
    origY = 60;

    cairo_t *cr1, *cr2 ;
    cr1 = gdk_cairo_create(drawAreaDeviation->window);
    cr2 = gdk_cairo_create(drawAreaDeviation->window);
    cairo_move_to (cr1, origX, origY);
    cairo_set_source_rgb (cr1,0,0.7,0);
    cairo_arc(cr1, origX, origY, 40, PI, 2*PI);
    cairo_close_path (cr1);
    cairo_fill(cr1);
    cairo_set_source_rgb (cr2,0.8,1,0.3);

    if ( desv >= 0 )
    {
        if (desv >= 50) desv = 50;
        arcend = arccenter;
        arcinit = (arccenter + (((PI/2)/100) * (desv * 2))) ;
        if (arcinit >= 2*PI)
            arcinit = 2*PI;
        cairo_arc(cr2, origX, origY, 40, arcend, arcinit);
    }
    if ( desv < 0 )
    {
        if (desv <= -50) desv = -50;
        arcinit = arccenter - (((PI/2)/100) * (-desv * 2));
        if (arcinit <= PI) arcinit = PI;
        arcend = arccenter;
        cairo_arc(cr2, origX, origY, 40, arcinit, arcend);
    }
    cairo_line_to (cr2, origX, origY);
    cairo_close_path (cr2);
    cairo_fill(cr2);
    cairo_destroy(cr1);
    cairo_destroy(cr2);
}

// Draw JACK input peaks
void draw_meter()
{
    float minDB = -minSliderValue;
    GtkAllocation rectangle = drawAreaMeter->allocation;
    float h = rectangle.height-4;
    float peak= -((linear2db(inputPeak)+minDB) * ((h)/minDB));
    if (peak > h ) peak = h;
    if (peak > 0 ) peak = 0;
    float threshold = -((linear2db(jackThreshold)+minDB) * ((h)/minDB));

    cairo_t *cr0, *cr1 ;
    cr0 = gdk_cairo_create(drawAreaMeter->window);
    cr1 = gdk_cairo_create(drawAreaMeter->window);

    cairo_set_source_rgb (cr0,0,0,0);
    if (client == NULL) cairo_set_source_rgb (cr0,0.6,0.6,0.6);
    cairo_rectangle(cr0,0,h+4,15,-h-4);
    cairo_close_path (cr0);
    cairo_fill(cr0);

    cairo_set_source_rgb (cr1,0,0.7,0);
    cairo_rectangle(cr1,2,h+2,9,(int)peak);
    cairo_close_path (cr1);
    cairo_fill(cr1);

    int h2= (int)peak-(int)threshold;
    //if (h2 > -10 ) h2 = -10;
    if ( peak < threshold )
    {
        cairo_set_source_rgb (cr1,0.8,1,0.3);
        cairo_rectangle(cr1,2,h+(int)threshold+2,9,h2);
        cairo_close_path (cr1);
        cairo_fill(cr1);
    }
    cairo_set_source_rgb (cr1,0,0.7,0);
    cairo_rectangle(cr1,2,h+(int)threshold+3,9,-2);
    cairo_close_path (cr1);
    cairo_fill(cr1);

    cairo_destroy(cr0);
    cairo_destroy(cr1);
}

// Draw Icon edit spin buttons
void draw_edit(GtkWidget *area)
{
	GdkPixbuf *pix;
    GError *err = NULL;
    pix = gdk_pixbuf_new_from_file(iconEditFile, &err);
    if(err)
    {
		printf("Error : %s\n", err->message);
		g_error_free(err);
		exit(1);
    }
    else
    {
		cairo_t *cr;
		cr = gdk_cairo_create (area->window);
		gdk_cairo_set_source_pixbuf(cr, pix, 0, 4);
		cairo_paint(cr);
		cairo_fill (cr);
		cairo_destroy (cr);
	}
}

// Callback for "value-changed" signal in emitted by sliderThreshold
void set_threshold (GtkRange *range)
{
    jackThreshold = db2linear(gtk_range_get_value (range));
}

// Callback by checkAutoreset button
void set_autoreset(GtkWidget *button, gpointer data)
{
    enabledAutoreset=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkAutoreset));
}

// Callback when input radioButtons change
void change_input (GtkWidget *button, gpointer data)
{
    enabledJack=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radioJack));
    enabledKbd=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radioKbd));
    enabledMidi=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radioMidi));
    if ( enabledJack == TRUE && client == NULL)
    {
		jack_init();
	}
}

// Callback when double click happens on BPM box
gboolean edit_bpm (GtkWidget *widget, GdkEvent * event , gpointer data)
{
    if (event->type == GDK_BUTTON_PRESS)
    {
        gtk_widget_show(entryBPM);
        gtk_widget_grab_focus (entryBPM);
        gtk_widget_hide(etiqBpmValue);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(entryBPM),totalBPM);
    }
    return TRUE;
}

// Callback when "enter" key is pressed in BPM spinButton as entry
gboolean on_change_bpm (GtkWidget *widget,  gpointer data)
{
      char *bpm = double_to_char(totalBPM, 0);
      gtk_label_set_text ( GTK_LABEL (etiqBpmValue) , bpm );
      gtk_widget_hide(entryBPM);
      gtk_widget_show(etiqBpmValue);
      if ( totalBPM > 0 )
      {
        calculate_note_time();
      }
      free(bpm);
      return TRUE;
}

// Callback when BPM spinButton has changed its value
gboolean bpm_changed(GtkWidget *widget,  gpointer data)
{
    totalBPM = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget));
    return TRUE;
}

// Callback when double click happens on Autoreset time box
gboolean edit_time_reset (GtkWidget *widget, GdkEvent * event , gpointer data)
{
    if (event->type == GDK_BUTTON_PRESS)
    {
        gtk_widget_show(entryTimeReset);
        gtk_widget_grab_focus (entryTimeReset);
        gtk_widget_hide(etiqTimeResetValue);
        gtk_widget_hide(etiqTimeResetUnit);
    }
    return TRUE;
}

// Callback when "enter" key is pressed in Autoreset time spinButton as entry
gboolean on_change_time_reset (GtkWidget *widget,  gpointer data)
{
    char *waitreset = double_to_char(timeWaitReset, 0);
    gtk_label_set_text ( GTK_LABEL (etiqTimeResetValue) , waitreset);
    gtk_widget_hide(entryTimeReset);
    gtk_widget_show(etiqTimeResetValue);
    gtk_widget_show(etiqTimeResetUnit);
    free(waitreset);
    return TRUE;
}

// Callback when Autoreset time spinButton has changed its value
gboolean time_reset_changed(GtkWidget *widget,  gpointer data)
{
    char *waitreset=NULL;
    timeWaitReset = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget));
    waitreset = double_to_char(timeWaitReset, 0);
    gtk_label_set_text ( GTK_LABEL (etiqTimeResetValue) , waitreset);
    free(waitreset);
    return TRUE;
}

// Force to change BPM and Autoreset time spinButtons when click is over window
// Imitates press "Enter" key
gboolean end_edit (GtkWidget *widget,  gpointer data)
{
    on_change_bpm(NULL,NULL);
    on_change_time_reset(NULL,NULL);
    return TRUE;
}

// Callback "value-changed" by comboPorts
void connect_port (GtkWidget *combo, gpointer data)
{
    GtkTreeIter iter;
    GtkTreeModel *tree;
    char *portToConnect;
    if ( gtk_combo_box_get_active_iter( GTK_COMBO_BOX(comboPorts), &iter ) )
    {
        tree = gtk_combo_box_get_model( GTK_COMBO_BOX(comboPorts));
        gtk_tree_model_get( tree, &iter, 0, &portToConnect, -1 );
    }
    const char **sourcePorts;
    sourcePorts = jack_get_ports (client, NULL, NULL, JackPortIsOutput);
    int i=0;
    while ( sourcePorts[i] != NULL )
    {
        if (jack_port_connected_to (inputPort , sourcePorts[i]) == true)
        {
            jack_disconnect (client, sourcePorts[i], jack_port_name (inputPort));
        }
		i++;
    }
    if ( gtk_combo_box_get_active(GTK_COMBO_BOX(comboPorts)) < i)
    {
        if (jack_connect (client, portToConnect, jack_port_name (inputPort)))
        {
            fprintf (stderr, "error: cannot connect ports\n");
        }
        else
        {
            printf("Port connected: %s\n", portToConnect);
        }
    }
    free(portToConnect);
    jack_free(sourcePorts);
}

// Callback of jack_set_graph_order_callback(). Create jack ports menu.
int create_ports()
{
    gdk_threads_enter();
    g_signal_handlers_block_by_func(comboPorts, (GFunc)connect_port, NULL);
    GtkTreeModel *tree;
    GtkListStore *lista;
    GtkTreeIter iter;
    activePortIndex=-1;
    tree = gtk_combo_box_get_model (GTK_COMBO_BOX(comboPorts));
    lista = GTK_LIST_STORE(tree);
    gtk_list_store_clear(lista); // Clean ListStore
    const char **sourcePorts;
    sourcePorts = jack_get_ports (client, NULL, NULL, JackPortIsOutput);
    int i=0;
    while ( sourcePorts[i] != NULL )
    {
        if (jack_port_connected_to (inputPort , sourcePorts[i]) == true) activePortIndex=i;
        gtk_list_store_append( lista, &iter );
		gtk_list_store_set( lista, &iter, 0, sourcePorts[i] , -1 );
		i++;
    }
    gtk_list_store_append( lista, &iter );
	gtk_list_store_set( lista, &iter, 0, "None" , -1 );
    jack_free(sourcePorts);
    if (jack_port_connected (inputPort) > 1) activePortIndex = -1;
	else if (jack_port_connected (inputPort) == 0) activePortIndex = i;
    gtk_combo_box_set_active(GTK_COMBO_BOX( comboPorts ),activePortIndex);
    g_signal_handlers_unblock_by_func(comboPorts, (GFunc)connect_port, NULL);
    gdk_threads_leave();
    return 0;
}

// Disable scroll mouse to change value in comboPorts
void scrolled_ports (GtkWidget *widget, GdkEvent * event , gpointer data)
{
    // Do nothing. Just to avoid action when scroll
    ;
}

// Creates note size menu
void create_notation()
{
    char *notation[6]={"Semibreve","Dotted Minim","Minim","Crotchet (Beat)","Quaver","Semiquaver"};
    GtkTreeModel *tree;
    GtkListStore *lista;
    GtkTreeIter iter;
    tree = gtk_combo_box_get_model (GTK_COMBO_BOX(comboNotation));
    lista = GTK_LIST_STORE(tree);
    gtk_list_store_clear(lista);
    int i;
    for ( i=0; i < 6 ; i++ )
    {
		gtk_list_store_append( lista, &iter );
		gtk_list_store_set( lista, &iter, 0, notation[i] , -1 );
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX( comboNotation ),3);
}

// Callback "value-changed" by comboNotation
void change_notation(GtkWidget *combo, gpointer data)
{
    switch (gtk_combo_box_get_active(GTK_COMBO_BOX( comboNotation )))
    {
        case 0:
            notationValue=4;
            break;
        case 1:
            notationValue=3;
            break;
        case 2:
            notationValue=2;
            break;
        case 3:
            notationValue=1;
            break;
        case 4:
            notationValue=0.5;
            break;
        case 5:
            notationValue=0.25;
            break;
        default:
            notationValue=1;
            break;
    }
    calculate_note_time();
}

// called if JACK client registration fails
void jack_fail()
{
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radioKbd),TRUE);
	gtk_button_set_label (GTK_BUTTON(radioJack),"Jack input fails... retry!");
}

// return a string of MIDI note
char *get_note_midi(int nt)
{
    char *nota[12]={"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
    if ( nt >= 0 )
    {
        char * octave = double_to_char((note_event/12)-1,0);
        strcpy(notePiano,nota[note_event%12]);
        strcat(notePiano," ");
        strcat(notePiano, octave);
        free(octave);
        return notePiano;
    }

    else return "None";
}

// ALSA Sequencer Handler
snd_seq_t *open_sequencer()
{
    snd_seq_t *seq_handle;
    int portid;
    if (snd_seq_open(&seq_handle, "hw",  SND_SEQ_OPEN_INPUT, SND_SEQ_NONBLOCK) < 0)
    {
        fprintf(stderr, "Error opening ALSA sequencer.\n");
        exit(1);
    }
    snd_seq_set_client_name(seq_handle, "Tapita");
    if ((portid = snd_seq_create_simple_port(seq_handle, "input",
              SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
              SND_SEQ_PORT_TYPE_APPLICATION)) < 0)
    {
        fprintf(stderr, "Error creating sequencer port.\n");
        exit(1);
    }
    return(seq_handle);
}

// Initialize JACK client , create input port and activate
// it will be initialized by function change_input() when
// option -j , "j" pressed or selecting "Jack input" radio button.
void jack_init()
{
	if (client == NULL)
	{

		// Init Jack Client
		char *clientName=(char*)"Tapita";
		// create a JACK client
		client = jack_client_open (clientName, JackNullOption, &jackStatus, NULL );
		if (client == NULL)
		{
			fprintf (stderr, "jack_client_open() failed, " "status = 0x%2.0x\n", jackStatus);
			if (jackStatus & JackServerFailed)
			{
				fprintf (stderr, "Unable to connect to JACK server\n");
			}
			jack_fail();
			//exit (1);
		}
		else
		{
			if (jackStatus & JackServerStarted)
			{
				fprintf (stderr, "JACK server started\n");
			}
			if (jackStatus & JackNameNotUnique)
			{
				clientName = jack_get_client_name(client);
				fprintf (stderr, "unique name `%s' assigned\n", clientName);
			}

			sampleRate = jack_get_sample_rate(client);
			jackWaitPeaks=(sampleRate/1000)*100;

			// Registration callback
			jack_set_graph_order_callback (client, create_ports, NULL);
			// calls a final function
			jack_on_shutdown (client, jack_shutdown, NULL);
			// register the process callback
			jack_set_process_callback (client, jack_process , 0);
			// Create input port
			inputPort = jack_port_register ( client, "in", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0 );
			if (inputPort == NULL)
			{
				fprintf(stderr, "no more JACK ports available\n");
				exit (1);
			}

			// Activate JACK client
			if (jack_activate (client))
			{
				fprintf (stderr, "cannot activate client\n");
				exit (1);
			}
			gtk_button_set_label (GTK_BUTTON(radioJack),"Jack input");
		}
	}
}

// Process JACK input. Might it  be namend event_jack()?
int jack_process(jack_nframes_t nframes, void *arg )
{
    if (enabledJack == true)
    {
        float *inputBuffer = (float *)jack_port_get_buffer (inputPort , nframes);
        double now;
        int countSamples;
        int i;
        float input;
        for (i=0 ; i<nframes ; i++)
        {
			input = fabs(inputBuffer[i]);
            if (input > inputPeak ) inputPeak = input;
            if (input > jackThreshold )
            {
                if (countSamples == 0) now=get_time();
                if ( countSamples == 3 && jackEvent == false)
                {
                    now=get_time();
                    on_event(now);
                    jackEvent=true;
                }
                countTimeSamples=0;
                countSamples++;
            }
            else
            {
                if ( countTimeSamples >= jackWaitPeaks && jackEvent==true )
                {
                    jackEvent=false;
                }
                countSamples=0;
                if (countTimeSamples >= 4294967294u) countTimeSamples=countTimeSamples;
                countTimeSamples++;
            }
        }
    }
    return 0;
}

// Before quitting application deactivate JACK
void jack_shutdown (void *arg)
{
	if (client != NULL )
	{
		jack_port_unregister (client, inputPort);
		jack_deactivate (client);
		jack_client_close (client);
	}
}

// Help output
void help_message() {
    printf("\n"  );
    printf("tapita %s,", version  );
    printf(" (c) juan ramon castan\n"  );
    printf("\n"  );
    printf("Tapita (snack in spanish) is a BPM detector\n"  );
    printf("with alphanumeric keyboard, MIDI and jack as inputs.\n");
    printf("\n"  );
    printf("usage: tapita [options]\n"  );
    printf("\n"  );
    printf("  options:\n"  );
    printf("\n"  );
	printf("    -h  Show this message.\n"  );
    printf("\n"  );
    printf("    -k  Set \"Space Bar\" as input. (default)\n"  );
    printf("\n"  );
    printf("    -j  Set \"jack\" as input.\n"  );
    printf("\n"  );
    printf("    -m  Set MIDI as input.\n"  );
    printf("\n"  );
    printf("    -p  Last beat is weighted in the avarage time.\n"  );
    printf("\n"  );
	printf("    -a[TIME]  Enable Autoreset after TIME seconds. TIME is optional.\n"  );
    printf("\n"  );
    printf("  BPM and Autoreset Time labels are editable.\n");
    printf("  Jack client will be active when selected as input.\n");
    printf("\n"  );
    printf("  \"Space\"\tto tap.\n"  );
    printf("  \"Delete\"\treset.\n"  );
    printf("  \"PageUp\"\traise jack input threshold.\n"  );
    printf("  \"PageDown\"\tdrop jack input threshold.\n"  );
    printf("  \"A\"\t\ttoggle Autoreset.\n"  );
    printf("  \"J\"\t\tset \"jack\" as input.\n"  );
    printf("  \"K\"\t\tset \"Space Bar\" as input.\n"  );
    printf("  \"M\"\t\tset MIDI as input.\n"  );
    printf("  \"Ctrl+Q\"\tquit application.\n"  );
    printf("\n"  );
}

// Quitting application
void quit(){
    printf("Quitting tapita...\n");
    jack_shutdown(NULL);
    gtk_main_quit();
}


