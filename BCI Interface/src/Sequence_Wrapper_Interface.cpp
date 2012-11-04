/**********************************************************************

    Sequence_Wrapper_Interface.cpp
    GUI functions for loading sequences

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include <sys/stat.h>
#include "Callbacks.h"
#include "Sequence_Wrapper.h"



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// load_sequences_button_callback
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void load_sequences_button_callback(GtkWidget * widget, GdkEvent * event)
{
	// Bring globals into scope
	extern GladeXML* gui;

    struct stat st;
    char filename[100];
    g_snprintf(filename,100,"../res/%s",SEQFILENAME());

	if(!BITRATE())
	{
		alert("Invalid Bitrate","Please enter a valid bitrate greater than zero");
		return;
	}

    if(stat(filename,&st) != 0 )
    {
        alert("File Not Found", "Check that the filename you entered is correct");
        return;
    }


	// Show calibration progress window;
    GtkWidget * window = glade_xml_get_widget(gui, "seq_progress");
   	gtk_widget_show(window);

	// Launch loader thread
	pthread_t loader;
	pthread_create(&loader, NULL, loader_thread, NULL);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// loader_thread
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void* loader_thread(void* t)
{

	// Load sequences
	init_sequences();

	//exit thread
	pthread_exit(NULL);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// init_sequences
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void init_sequences()
{

	extern Sequence_Wrapper g_sequences;

	// Local Variables
	SystemParameters    sysParams;
    char filename[100];

    gdk_threads_enter();    // Get GDK lock
    int     Br = BITRATE();
    int     Bs = BLOCKSIZE();
    char*   Fn = SEQFILENAME();
    int     Fs = FS();
    gdk_threads_leave();    // Release GDK lock

    // Set up system parameters
    sysParams.setSamplingRate(Fs);
    sysParams.setBlockSize(Bs);
	sysParams.setBitRate(Br);


	// Set up the sequences!
    g_snprintf(filename,100,"../res/%s",Fn);
	g_sequences.setSystemParameters(&sysParams);
	g_sequences.load_sequences_from_file(filename);
	g_sequences.print_sequences();

}
