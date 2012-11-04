/**********************************************************************

    DataLogger_Interface.cpp
    GUI functions for Data logging

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include "Callbacks.h"
#include "DataLogger.h"


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	run_test_button_callback
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void run_test_button_callback(GtkWidget * widget, GdkEvent * event)
{
    // Bring globals into scope
	extern GladeXML* gui;
	extern bool amp_connected;

	bool valid_length = ( TESTLENGTH() > 0 );
	bool valid_name = (TESTNAME() != NULL );

	if(!(valid_length && valid_name))
	{
		alert("Invalid Parameters", "Please specify a valid test length and filename.");
		return;
	}

	if(!amp_connected)
	{
		alert("No Amp Detected","The system did not detect a g.USB amp.\n Please connect a compatible amplifier and try again");
		return;
	}

	// Show calibration progress window;
    GtkWidget * window = glade_xml_get_widget(gui, "log_progress");
   	gtk_widget_show(window);

	// Launch Calibration thread
	pthread_t log;
	pthread_create(&log, NULL, tester_thread, NULL);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	tester_thread
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void* tester_thread(void* t)
{
	run_test();
	pthread_exit(NULL);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	run_test
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void run_test(void)
{
	// Bring globals into scope
    extern GStaticMutex amplifier_mutex;

    // Local variables
    SystemParameters sysParams;
    DataLogger tester;
    testDef* theTest;
    SignalSource* src;
    string testName;

    gdk_threads_enter();    // Get GDK lock
    int     Fs = FS();
    int     Bs = BLOCKSIZE();
    double  Tl = TESTLENGTH();
    char*   Tn = TESTNAME();
    gdk_threads_leave();    // Release GDK lock


    // Set up system parameters
    sysParams.setSamplingRate(Fs);
    sysParams.setBlockSize(Bs);

    // Set up the test
    testName.assign(Tn);
    theTest = createTest(&sysParams, Tl);
    theTest->setTestName(testName);
    theTest->setSystemParameters(&sysParams);


    // Set up tester
    tester.setSystemParameters(&sysParams);
    tester.setTest(theTest);

    g_static_mutex_lock(&amplifier_mutex);      // Lock amplifier


  	src = createUsbAmpSource(&sysParams,NUMBER_OF_CHANNELS);

   	tester.setSignalSource(src);

    tester.runTest();


    g_static_mutex_unlock(&amplifier_mutex);    // unlock amplifier

	// cleanup
    delete theTest;
	delete src;
}
