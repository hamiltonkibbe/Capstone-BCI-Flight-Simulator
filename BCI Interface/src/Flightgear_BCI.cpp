/**********************************************************************

    Flightgear_BCI.cpp
    A Brain Control Interface for the Flightgear flight simulator

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include "Flightgear_BCI.h"



using namespace std;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Globals
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool                    amp_connected = FALSE;
Socket                  g_control_port;
Socket                  g_data_port;
Sequence_Wrapper        g_sequences;
eeg_whitening_filter    *g_whitening_filter = NULL;
GStaticMutex            amplifier_mutex = G_STATIC_MUTEX_INIT;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Main
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main(int argc, char** argv)
{
    g_thread_init(NULL);

    gdk_threads_init();

    gtk_init (&argc, &argv);        // INitialize GUI

    init_gui();

    gdk_threads_enter();

    gtk_main();                     // Wait for callbacks

    gdk_threads_leave();

    // Exit
    return 0;
}

