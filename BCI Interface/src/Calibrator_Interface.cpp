/**********************************************************************

    Calibrator_Interface.cpp
    GUI functions for calibration

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include <sys/stat.h>
#include "Calibrator.h"
#include "Callbacks.h"




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	calibrate_button_callback
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void calibrate_button_callback(GtkWidget * widget, GdkEvent * event)
{

	// Bring globals into scope
	extern GladeXML* gui;
	extern Sequence_Wrapper g_sequences;
	extern bool amp_connected;


	if(!g_sequences.is_loaded())
	{
		alert("No Sequences Loaded","Sequences must be loaded before running calibration");
		return;
	}
	if(!amp_connected)
	{
		alert("No Amp Detected","The system did not detect a g.USB amp.\n Please connect a compatible amplifier and try again");
		return;
	}

   	gtk_widget_show(glade_xml_get_widget(gui, "cal_progress"));

    // Launch Calibration thread
	pthread_t cal;
	pthread_create(&cal, NULL, calibration_thread, NULL);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	load_template_button_callback
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void load_templates_button_callback(GtkWidget * widget, GdkEvent * event)
{
    // GLobals
	extern Sequence_Wrapper g_sequences;

    struct stat st;
    char filename[100];

    g_snprintf(filename,100,"../res/cal/%s_seq%d_%dbps.tmpl",USERNAME(),1,BITRATE());

	if(!g_sequences.is_loaded())
	{
		alert("No Sequences Loaded","Sequences must be loaded before running calibration");
		return;
	}

    if(stat(filename,&st) != 0 )
    {
        alert("Templates Not Found", "Check that the username and bitrate you entered are correct");
        return;
    }






	// Launch Calibration thread
	pthread_t temp_loader;
	pthread_create(&temp_loader, NULL, temp_loader_thread, NULL);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	calibrate_sequences
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void calibrate_sequences(void)
{
    // Globals
	extern Sequence_Wrapper g_sequences;
	extern GStaticMutex amplifier_mutex;

    // Locals
    SignalSource * src;
	SystemParameters sysParams;
    Calibrator *cal = new Calibrator();


    gdk_threads_enter();    //Get GDK lock
    int Br = BITRATE();
    int Fs = FS();
    int Bs = BLOCKSIZE();
    gdk_threads_leave();    // Release GDK lock


    // Set up System Parameters
    sysParams.setSamplingRate(Fs);
    sysParams.setBlockSize(Bs);

    cal->set_system_parameters(&sysParams);
    cal->set_sequences(&g_sequences);
    cal->set_bit_rate(Br);


    g_static_mutex_lock(&amplifier_mutex);      // Lock amplifier

	// Set up the usb amp
  	src = createUsbAmpSource(&sysParams,NUMBER_OF_CHANNELS);

	cal->set_source(src);

	cal->calibrate();

    g_static_mutex_unlock(&amplifier_mutex);    // Unlock amplifier

	// Clean up
    delete cal;
	delete src;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	load_templates
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void load_templates(void)
{
	// Bring Globals into scope
	extern Sequence_Wrapper g_sequences;

	// Locals
	SystemParameters sysParams;
    Calibrator * cal = new Calibrator();

    cal->set_sequences(&g_sequences);
	cal->set_bit_rate(BITRATE());

	// Set up system parameters
    sysParams.setSamplingRate(FS());
    sysParams.setBlockSize(BLOCKSIZE());
	cal->set_system_parameters(&sysParams);

	// Load files
	cal->load_calibration_files();

	// Clean up
	delete cal;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	calibration_thread
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void* calibration_thread(void* t)
{
	// Run calibration
	calibrate_sequences();

	// Exit thread
	pthread_exit(NULL);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	temp_loader_thread
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void* temp_loader_thread(void* t)
{
	// Load templates
	load_templates();

	// Exit thread
	pthread_exit(NULL);
}

