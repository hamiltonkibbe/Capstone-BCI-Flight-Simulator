/**********************************************************************

    Detector_Interface.cpp
    GUI functions for detection

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include "Callbacks.h"
#include "Detector.h"


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	detect_button_callback
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void detect_button_callback(GtkWidget * widget, GdkEvent * event)
{
	// Bring globals into scope
	extern Sequence_Wrapper g_sequences;
	extern bool amp_connected;

    static pthread_t detector = NULL;

	// Prevent this dialog from showing when deactivating button
	if (!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) return;

	// Verify sequences are loaded
	if(!g_sequences.is_loaded())
	{
		alert("No Sequences Loaded","Sequences must be loaded before running detectors");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),FALSE);
		return;
	}
/*
	if(!amp_connected)
	{
		alert("No Amp Detected","The system did not detect a g.USB amp.\n Please connect a compatible amplifier and try again");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),FALSE);
		return;
	}
*/
    pthread_create(&detector, NULL, detection_thread, NULL);
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	detection_thread
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void* detection_thread(void* t)
{
	// run detectors
	run_detectors();

	//exit thread
	pthread_exit(NULL);

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	run_detectors
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void run_detectors(void)
{

	// Bring Globals into scope
	extern Sequence_Wrapper g_sequences;
	extern eeg_whitening_filter *g_whitening_filter;
    extern GStaticMutex amplifier_mutex;

	// Local variables
    SystemParameters sysParams;
    Sequence_Wrapper sequences;
    Detector detector;
    SignalSource* src;

    gdk_threads_enter();    // Get GDK lock
    int Fs = FS();
    int Bs = BLOCKSIZE();
	int Br = BITRATE();
	gdk_threads_leave();    // Get GDK lock


    // Set up system parameters
    sysParams.setSamplingRate(Fs);
    sysParams.setBlockSize(Bs);
	sysParams.setBitRate(Br);

    // Set up detector
    detector.setSystemParameters(&sysParams);
    detector.setSequences(&g_sequences);
    detector.set_filter(g_whitening_filter);

    g_static_mutex_lock(&amplifier_mutex);


  	src = createUsbAmpSource(&sysParams,NUMBER_OF_CHANNELS);

    detector.setSignalSource(src);

    detector.run();         // Run detection

    g_static_mutex_unlock(&amplifier_mutex);

	// Clean up
	delete src;
}

