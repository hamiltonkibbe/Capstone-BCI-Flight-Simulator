/**********************************************************************

    SystemParameters_Interface.cpp
    GUI functions for editing system parameters

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include "Callbacks.h"
#include "SystemParameters.h"



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// edit_system_parameters
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void edit_system_parameters(GtkWidget * widget, GdkEvent * event)
{
    // Bring globals into scope
    extern GladeXML * gui;

    // Local variables
    GtkWidget *params_pane = glade_xml_get_widget(gui, "system_parameters");

    // Show the system parameters editor window
    gtk_widget_show(params_pane);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// update_system_parameters
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void update_system_parameters(GtkWidget * widget, GdkEvent * event)
{
    // Bring globals into scope
    extern GladeXML * gui;

    // Local variables
    GtkWidget *pane = glade_xml_get_widget(gui, "system_parameters");
    GtkWidget *new_fs = glade_xml_get_widget(gui, "Fs_edit_box");
    GtkWidget *new_bs = glade_xml_get_widget(gui, "Bs_edit_box");
    GtkWidget *disp_fs = glade_xml_get_widget(gui, "Fs_view");
    GtkWidget *disp_bs = glade_xml_get_widget(gui, "Bs_view");
    char fs_buffer[5];
    char bs_buffer[5];
    int Fs;
    int Bs;

    //Set the system paramaters in the main window
    Fs = atoi(gtk_entry_get_text(GTK_ENTRY(new_fs)));
    Bs = atoi(gtk_entry_get_text(GTK_ENTRY(new_bs)));
    sprintf(fs_buffer,"%d",Fs);
    sprintf(bs_buffer,"%d",Bs);
    gtk_entry_set_text(GTK_ENTRY(disp_fs),fs_buffer);
    gtk_entry_set_text(GTK_ENTRY(disp_bs),bs_buffer);

    //cleanup and close editor window
    gtk_entry_set_text(GTK_ENTRY(new_fs),"");
    gtk_entry_set_text(GTK_ENTRY(new_bs),"");
    gtk_widget_hide(pane);

}
