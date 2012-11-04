/**********************************************************************

    FGFS_Interface.cpp
    GUI functions for interfacing with Flightgear

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include <string>
#include "Alert.h"
#include "Callbacks.h"
#include "Fgfs.h"


using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	connect_fgfs_button_callback
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void connect_fgfs_button_callback(GtkWidget *widget, GdkEvent * event)
{
    // Globals
    extern Socket g_control_port;
    extern Socket g_data_port;

    // Locals
    static guint timeout = 0;


	if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
	{
	    g_control_port.close();
	    g_data_port.close();

        if(timeout) g_source_remove(timeout);
	    timeout = 0;
	}

	else
	{

        if(!(g_control_port.create() && g_data_port.create()))
        {
            alert("Socket Error", "Failed to create sockets");
        }

        if(!(g_control_port.connect("127.0.0.1", CONTROLPORT()) && g_data_port.bind("127.0.0.1", DATAPORT())))
        {
            alert("Socket Error", "Failed to bind sockets");
        }

        timeout =  g_timeout_add(200, fgfs_data_timeout, NULL);
	}

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	fgfs_data_timeout
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int fgfs_data_timeout(void* data)
{
    //Globals
    static GStaticMutex socket_mutex = G_STATIC_MUTEX_INIT;

    if(!g_static_mutex_trylock(&socket_mutex))  return 1;

    // Get fgfs flight data
    string latitude = getData("get double /position/latitude-deg\n");
    string longitude = getData("get double /position/longitude-deg\n");
    string speed = getData("get double /velocities/airspeed-kt\n");
    string heading = getData("get double /orientation/heading-magnetic-deg\n");
    string altitude = getData("get double /position/altitude-ft\n");
    string pitch = getData("get double /orientation/pitch-deg\n");
    string roll = getData("get double /orientation/roll-deg\n");

    g_static_mutex_unlock(&socket_mutex);

    // Update gui
    update_fgfs_display(latitude,longitude,speed,heading,altitude,pitch,roll);

    return 1;
}





