/**********************************************************************

    GUI.cpp
    GUI support functions

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include "GUI.h"



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global GUI Handles
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GladeXML * gui;					// Libglade GUI constructor
GtkWidget * window;				// App window




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	init_gui
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void init_gui(void)
{
    // Bring globals into scope
    extern GladeXML * gui;
    extern GtkWidget * window;

	// Set up global GUI Handles
    gui = glade_xml_new ("../res/gui/gui.glade", NULL, NULL);
	window = glade_xml_get_widget (gui, "window");

	// Set up splash screen
	GtkWidget *splash = glade_xml_get_widget(gui,"splash");
	gtk_widget_show(GTK_WIDGET(splash));

	// update splash status
	splash_status("Initializing User Interface...");


	// Add timeout functions
	g_timeout_add(1000, check_amp_timeout, NULL);
	g_timeout_add(1000,close_splash_timeout, (gpointer)splash);

	// Initialize
	connect_event_handlers();

    init_data();

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	connect_event_handlers
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void connect_event_handlers(void)
{
    extern GladeXML * gui;
    GtkWidget * widget;

	splash_status("Initializing Interface Events...");


    // Windows ---------------------------------------

    // Main window
    widget = glade_xml_get_widget (gui, "window");
    g_signal_connect (widget, "delete-event",G_CALLBACK (delete_event), NULL);
    g_signal_connect (widget, "destroy",G_CALLBACK (destroy), NULL);

    // System parameters window
    widget = glade_xml_get_widget(gui, "system_parameters");
    g_signal_connect (widget, "delete_event",G_CALLBACK (hide_widget), NULL);

	// Calibration Progress window
	widget = glade_xml_get_widget(gui, "cal_progress");
	g_signal_connect (widget, "delete_event", G_CALLBACK (hide_widget), NULL);

	// Sequence loading window
	widget = glade_xml_get_widget(gui, "seq_progress");
	g_signal_connect (widget, "delete_event", G_CALLBACK (hide_widget), NULL);



    // Buttons ---------------------------------------

    // Edit parameters button
    widget = glade_xml_get_widget (gui, "edit_parameters_button");
    g_signal_connect (widget, "clicked",G_CALLBACK (edit_system_parameters), NULL);

    // Update parameters button
    widget = glade_xml_get_widget (gui, "update_parameters_button");
    g_signal_connect (widget, "clicked",G_CALLBACK (update_system_parameters), NULL);

	// Close logger button
	widget = glade_xml_get_widget (gui, "close_log_button");
    g_signal_connect (widget, "clicked",G_CALLBACK (close_log), NULL);

    // Run test button
    widget = glade_xml_get_widget (gui, "run_test_button");
    g_signal_connect (widget, "clicked",G_CALLBACK (run_test_button_callback), NULL);

	// Connect FGFS button
	widget = glade_xml_get_widget(gui, "connect_fgfs_button");
	g_signal_connect (widget, "clicked", G_CALLBACK(connect_fgfs_button_callback), NULL);

	// Detect button
	widget = glade_xml_get_widget(gui, "detect_button");
	g_signal_connect (widget, "clicked", G_CALLBACK(detect_button_callback), NULL);

	// Load Sequences button
	widget = glade_xml_get_widget(gui, "load_sequences_button");
	g_signal_connect (widget, "clicked", G_CALLBACK(load_sequences_button_callback), NULL);

	// Calibrate button
	widget = glade_xml_get_widget(gui, "calibrate_button");
	g_signal_connect (widget, "clicked", G_CALLBACK(calibrate_button_callback), NULL);

	// Load templates button
	widget = glade_xml_get_widget(gui, "load_templates_button");
	g_signal_connect (widget, "clicked", G_CALLBACK(load_templates_button_callback), NULL);


    // create_whitening_filter_button
	widget = glade_xml_get_widget(gui, "create_whitening_filter_button");
	g_signal_connect (widget, "clicked", G_CALLBACK(create_whitening_filter_button_callback), NULL);

    // load whitening filter button
    widget = glade_xml_get_widget(gui, "load_whitening_filter_button");
	g_signal_connect (widget, "clicked", G_CALLBACK(load_whitening_filter_button_callback), NULL);


	// Drawing Areas ---------------------------------

	widget = glade_xml_get_widget (gui, "inst_pitch");
	g_signal_connect (widget, "expose_event", G_CALLBACK(inst_pitch_expose), NULL);

	widget = glade_xml_get_widget (gui, "inst_roll");
    g_signal_connect (widget, "expose_event", G_CALLBACK(inst_roll_expose), NULL);


}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	init_data
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void init_data(void)
{
    // Bring globals into scope
    extern GladeXML * gui;

    // Locals
    int BUFFER_SIZE = 7;

	// update splash screen status
	splash_status("Initializing Data...");


    // Local variables
    char bs_buffer[BUFFER_SIZE];
	char cc_buffer[BUFFER_SIZE];
	char cp_buffer[BUFFER_SIZE];
	char dp_buffer[BUFFER_SIZE];
	char fs_buffer[BUFFER_SIZE];

    // Load data into text buffers
    g_snprintf(fs_buffer,BUFFER_SIZE,"%d",DEFAULT_FS);
    g_snprintf(bs_buffer,BUFFER_SIZE,"%d",DEFAULT_BLOCKSIZE);
	g_snprintf(cc_buffer,BUFFER_SIZE,"%d",NUMBER_OF_CHANNELS);
	g_snprintf(cp_buffer,BUFFER_SIZE,"%d",DEFAULT_CP);
	g_snprintf(dp_buffer,BUFFER_SIZE,"%d",DEFAULT_DP);

	// Set amp indicator
	gtk_image_set_from_file(GTK_IMAGE(glade_xml_get_widget(gui, "amp_indicator")), "../res/gui/img/noamp.jpg");

    // Update GUI text boxes
    gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(gui, "Fs_view")),fs_buffer);
    gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(gui, "Bs_view")),bs_buffer);
    gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(gui, "number_of_channels_view")),cc_buffer);
	gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(gui, "sequence_filename")),"sequences.txt");
    gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(gui, "control_port_view")),cp_buffer);
    gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(gui, "data_port_view")),dp_buffer);
    gtk_editable_set_editable(GTK_EDITABLE(glade_xml_get_widget(gui, "control_port_view")),FALSE);
    gtk_editable_set_editable(GTK_EDITABLE(glade_xml_get_widget(gui, "data_port_view")),FALSE);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	close_loader
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void close_loader(GtkWidget * widget, GdkEvent * event)
{
	extern GladeXML* gui;

	gtk_widget_hide(glade_xml_get_widget(gui, "seq_progress"));
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	close_log
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void close_log(GtkWidget * widget, GdkEvent * event)
{
	extern GladeXML* gui;

	gtk_widget_hide(glade_xml_get_widget(gui, "log_progress"));
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	splash_status
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void splash_status(const char* status)
{

	extern GladeXML *gui;
	char buffer[50];
	GtkWidget *status_bar = glade_xml_get_widget(gui,"splash_status_bar");
	guint status_id = gtk_statusbar_get_context_id( GTK_STATUSBAR(status_bar),"Splash status");

	g_snprintf(buffer,50,"%s",status);

	gtk_statusbar_push (GTK_STATUSBAR(status_bar), status_id, buffer);

}






