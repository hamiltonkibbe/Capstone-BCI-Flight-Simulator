/**********************************************************************

    Alert.cpp
    Alert and statusbar methods

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include "Alert.h"


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	alert
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void alert(const char * message)
{
    // Glocbals
	extern GladeXML *gui;

	GtkWidget *label = glade_xml_get_widget(gui, "alert_label");
	GtkWidget *dialog = glade_xml_get_widget(gui, "alert");

	gtk_label_set_text(GTK_LABEL(label),message);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_hide (dialog);
	gtk_label_set_text(GTK_LABEL(label),"");

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	alert
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void alert(const char* title, const char* message)
{
	extern GladeXML *gui;


	GtkWidget *label = glade_xml_get_widget(gui, "alert_label");
	GtkWidget *dialog = glade_xml_get_widget(gui, "alert");

	gtk_label_set_text(GTK_LABEL(label),message);
	gtk_window_set_title(GTK_WINDOW(dialog),title);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_hide (dialog);
	gtk_label_set_text(GTK_LABEL(label),"");
	gtk_window_set_title(GTK_WINDOW(dialog),"Alert");

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	status
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void status(const char* status)
{
    extern GladeXML *gui;

    int BUFFER_SIZE = 100;
    char buffer[BUFFER_SIZE];
    guint *status_id = new guint;

    // generate messsage
    g_snprintf(buffer,BUFFER_SIZE,"%s",status);

    // update status bar
    *status_id = gtk_statusbar_get_context_id( GTK_STATUSBAR(glade_xml_get_widget(gui,"status_bar")),"Status Bar");
    gtk_statusbar_push(GTK_STATUSBAR(glade_xml_get_widget(gui,"status_bar")), *status_id, buffer);

    //say(status);

    // remove status after 2 seconds
    g_timeout_add(2000,status_remove_timeout, (void*)status_id);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	status_remove_timeout
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int status_remove_timeout(void* data)
{
    extern GladeXML *gui;

    guint *status_id = (guint*)data;

    gdk_threads_enter();    // Get GDK lock

    gtk_statusbar_pop(GTK_STATUSBAR(glade_xml_get_widget(gui,"status_bar")),*status_id);

    gdk_threads_leave();    // Release GDK lock

    delete status_id;

    return 0;

}

void say(const char* string)
{
    int BUFFER_SIZE = 100;
    char command[BUFFER_SIZE];

    g_snprintf(command,BUFFER_SIZE,"espeak \"%s\" &",string);
    system(command);
}
