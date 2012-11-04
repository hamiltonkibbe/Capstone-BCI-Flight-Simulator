/**********************************************************************

    Callbacks.cpp
    GUI functions

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include <cstdlib>
#include "Callbacks.h"
#include "UsbAmpSource.h"


int BITRATE()
{
    extern GladeXML *gui;
    return atoi(gtk_entry_get_text(GTK_ENTRY(glade_xml_get_widget(gui, "Br_view"))));
}

int BLOCKSIZE()
{
    extern GladeXML *gui;
    return atoi(gtk_entry_get_text(GTK_ENTRY(glade_xml_get_widget(gui, "Bs_view"))));
}

int CHANNELS()
{
    extern GladeXML *gui;
    return atoi(gtk_entry_get_text(GTK_ENTRY(glade_xml_get_widget(gui, "number_of_channels_view"))));
}

int CONTROLPORT()
{
    extern GladeXML *gui;
    return atoi(gtk_entry_get_text(GTK_ENTRY(glade_xml_get_widget(gui, "control_port_view"))));
}

int DATAPORT()
{
    extern GladeXML *gui;
    return atoi(gtk_entry_get_text(GTK_ENTRY(glade_xml_get_widget(gui, "data_port_view"))));
}

int FS()
{
    extern GladeXML *gui;
    return atoi(gtk_entry_get_text(GTK_ENTRY(glade_xml_get_widget(gui, "Fs_view"))));
}


char* SEQFILENAME()
{
    extern GladeXML *gui;
    return (char*)gtk_entry_get_text(GTK_ENTRY(glade_xml_get_widget(gui, "sequence_filename")));
}

double TESTLENGTH()
{
    extern GladeXML *gui;
    return atof(gtk_entry_get_text(GTK_ENTRY(glade_xml_get_widget(gui, "test_length_view"))));
}

char* TESTNAME()
{
   extern GladeXML *gui;
    return (char*)gtk_entry_get_text(GTK_ENTRY(glade_xml_get_widget(gui, "test_name_view")));
}

char* USERNAME()
{
    extern GladeXML *gui;
    return (char*)gtk_entry_get_text(GTK_ENTRY(glade_xml_get_widget(gui, "user_field")));
}

char* WFLTFILENAME()
{
    extern GladeXML *gui;
    return (char*)gtk_entry_get_text(GTK_ENTRY(glade_xml_get_widget(gui, "wfilter_name_view")));
}



void GUESS(const char* guess)
{
    extern GladeXML *gui;

    gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(gui, "guess_field")),guess);
}



//=====================================================================
// GUI Callbacks
//=====================================================================

extern gboolean delete_event(GtkWidget * widget, GdkEvent * event, gpointer data ){ return FALSE; }
extern void destroy(GtkWidget * widget, gpointer data) { gtk_main_quit(); }
void hide_widget(GtkWidget * widget, GdkEvent * event){ gtk_widget_hide(widget); }



//=====================================================================
// Timeout functions
//=====================================================================
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	check_amp_timeout
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int check_amp_timeout(void* data)
{
	// Globals
	extern GladeXML* gui;
	extern GStaticMutex amplifier_mutex;
	extern bool amp_connected;

    // Try to lock the amplifier mutex and exit if it is in use
    if(!g_static_mutex_trylock(&amplifier_mutex)) return 1;

	amp_connected = amp_is_connected();         // check for connected amp

    g_static_mutex_unlock(&amplifier_mutex);    // unlock amplifier mutex

    // Update gui
    gdk_threads_enter();    // Get GDK lock
    if(amp_connected) { gtk_image_set_from_file(GTK_IMAGE(glade_xml_get_widget(gui,"amp_indicator")),"../res/gui/img/amp.jpg"); }
    else { gtk_image_set_from_file(GTK_IMAGE(glade_xml_get_widget(gui,"amp_indicator")),"../res/gui/img/noamp.jpg"); }
    gdk_threads_leave();    // Release GDK lock

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	close_splash_timeout
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int close_splash_timeout(void* data)
{
	// Globals
	extern GtkWidget *window;

	// hide splash screen and show gui!
	gdk_threads_enter();
	hide_widget((GtkWidget*)data,NULL);
	gtk_widget_show_all(window);
	gdk_threads_leave();

	return 0;
}
