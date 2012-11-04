/**********************************************************************

    Fgfs.cpp
    Functions for interfacing with flightgear

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include "Fgfs.h"



using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	update_fgfs_display
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void update_fgfs_display(string latitude,string longitude,string speed,string heading,
                         string altitude,string pitch,string roll)
{
	// Bring globals into scope
	extern GladeXML * gui;

    int alt;
	char lat_buffer[10];
    char lon_buffer[10];
    char spd_buffer[9];
    char hdg_buffer[6];
    char alt_buffer[9];


    if ((atof(latitude.c_str()) < 0)) g_snprintf(lat_buffer,10,"%0.4fS",-1*atof(latitude.c_str()));
    else    g_snprintf(lat_buffer,10,"%0.4fN",atof(latitude.c_str()));


    if (atof(longitude.c_str()) < 0) g_snprintf(lon_buffer,10,"%0.4fW",-1*atof(longitude.c_str()));
    else    g_snprintf(lon_buffer,10,"%0.4fE",atof(longitude.c_str()));

    alt = atoi(altitude.c_str());
    alt *= (alt > 0);

    g_snprintf(alt_buffer,9,"%i ft",alt);
    g_snprintf(spd_buffer,9,"%0.1f kt",atof(speed.c_str()));
    g_snprintf(hdg_buffer,6,"%0.1f",atof(heading.c_str()));


    gdk_threads_enter();    // Get GDK lock
    gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(gui,"lat")), lat_buffer);
	gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(gui,"lon")), lon_buffer);
	gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(gui,"spd")), spd_buffer);
	gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(gui,"hdg")), hdg_buffer);
	gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(gui,"alt")), alt_buffer);
	draw_pitch_plane(glade_xml_get_widget(gui,"inst_pitch"),atoi(pitch.c_str()));
	draw_roll_plane(glade_xml_get_widget(gui,"inst_roll"),atoi(roll.c_str()));
    gdk_threads_leave();    // Release GDK lock
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	getData
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
string getData(string s)
{
    // Globals
    extern Socket g_control_port;
    extern Socket g_data_port;

    // Locals
    string recvBuf;

	g_control_port.send(s);
	g_data_port.recv(recvBuf);

	return recvBuf;
}
