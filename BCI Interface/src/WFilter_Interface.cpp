/**********************************************************************

    WFilter_Interface.cpp
    GUI functions for whitening filter

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include <sys/stat.h>
#include "Callbacks.h"
#include "Wfilter/eeg_whitening_filter.hpp"


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	load_whitening_filter_button_callback
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void load_whitening_filter_button_callback(GtkWidget *widget, GdkEvent *event)
{
    // Globals
    extern eeg_whitening_filter *g_whitening_filter;

    // Locals
    int         BUFFER_SIZE = 100;
    char        filename[BUFFER_SIZE];
    struct stat st;

    g_snprintf(filename,BUFFER_SIZE,"../res/%s",WFLTFILENAME());            // get full path

    // Check that file exists
    if(stat(filename,&st) == 0)
    {
        if(g_whitening_filter) delete g_whitening_filter;                   // Delete existing filter

        g_whitening_filter = new eeg_whitening_filter(filename);            // Create new filter

        status("Loaded whitening filter");                                  // Confirmation

    }

    else alert("File Not Found", "Check that the filename you entered is correct");

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	create_whitening_filter_button_callback
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void create_whitening_filter_button_callback(GtkWidget *widget, GdkEvent *event)
{
	extern bool amp_connected;

    if(!amp_connected)
	{
		alert("No Amp Detected", "The system did not detect a g.USB amp.\n Please connect a compatible amplifier and try again");
		return;
	}

    // Launch filter generator thread
	pthread_t thread;
	pthread_create(&thread, NULL, whitening_filter_thread, NULL);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	generate_whitening_filter
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void generate_whitening_filter()
{
    // Bring globals into scope
    extern eeg_whitening_filter     *g_whitening_filter;
    extern GStaticMutex             amplifier_mutex;

    // Local variables
    int                 BUFFER_SIZE = 100;
    char                filename[BUFFER_SIZE];
    SignalSource        *src;
    SystemParameters    sysParams;

    gdk_threads_enter();    // Get GDK lock
    int     Bs = BLOCKSIZE();
    int     Fs = FS();
    char*   Fn = WFLTFILENAME();
    gdk_threads_leave();    // Release GDK lock

    // get full path
    g_snprintf(filename,BUFFER_SIZE,"../res/%s",Fn);

    // Set up system parameters
    sysParams.setSamplingRate(Fs);
    sysParams.setBlockSize(Bs);


    g_static_mutex_lock(&amplifier_mutex);      // Lock amplifier mutex


    src = createUsbAmpSource(&sysParams,NUMBER_OF_CHANNELS);

    if(g_whitening_filter) delete g_whitening_filter;                   // Delete existing filter

    g_whitening_filter = new_whitening_filter(src, &sysParams, 128);    // Create new filter

    g_whitening_filter->save(filename);                                 // Save new filter


    gdk_threads_enter();    // Get GDK lock
    status("Created whitening filter");
    gdk_threads_leave();    // Release GDK lock


    g_static_mutex_unlock(&amplifier_mutex);    // Unlock amplifier mutex

    //cleanup
    delete src;

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	whitening_filter_thread
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void* whitening_filter_thread(void* data)
{
	// Log data
	generate_whitening_filter();
	// exit thread
	pthread_exit(NULL);
}

