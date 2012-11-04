#ifndef _CALLBACKS_H_
#define _CALLBACKS_H_


// Libraries
#include <glade/glade.h>


//=====================================================================
// GUI Access Functions
//
// THESE MUST ONLY BE
// CALLED FROM WITHIN
// THE GDK LOCK

// Get
int BITRATE();
int BLOCKSIZE();
int CHANNELS();
int CONTROLPORT();
int DATAPORT();
int FS();
char* SEQFILENAME();
double TESTLENGTH();
char* TESTNAME();
char* USERNAME();
char* WFLTFILENAME();


// Set
void GUESS(const char* guess);

//=====================================================================



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Calibrator callbacks
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern "C" void calibrate_button_callback(GtkWidget * widget, GdkEvent * event);
extern "C" void load_templates_button_callback(GtkWidget * widget, GdkEvent * event);
void calibrate_sequences(void);
void load_templates(void);
void* calibration_thread(void* t);
void* temp_loader_thread(void* t);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// DataLogger callbacks
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern "C" void run_test_button_callback(GtkWidget * widget, GdkEvent * event);
void run_test();
void* tester_thread(void* t);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Detector Callbacks
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern "C" void detect_button_callback(GtkWidget * widget, GdkEvent * event);
void run_detectors(void);
void* detection_thread(void* t);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// FGFS interface Callbacks
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern "C" void connect_fgfs_button_callback(GtkWidget *widget, GdkEvent * event);
void* listener_thread(void* t1);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Instrument Callbacks
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern "C" gboolean inst_pitch_expose(GtkWidget * instrument, GdkEventExpose * event);
extern "C" gboolean inst_roll_expose(GtkWidget * instrument, GdkEventExpose * event);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sequence Wrapper Callbacks
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern "C" void load_sequences_button_callback(GtkWidget * widget, GdkEvent * event);
void init_sequences(void);
void* loader_thread(void* t);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// System Parameters Callbacks
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern "C" void edit_system_parameters(GtkWidget * widget, GdkEvent * event);
extern "C" void update_system_parameters(GtkWidget * widget, GdkEvent * event);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Whitening Filter Interface
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern "C" void create_whitening_filter_button_callback(GtkWidget *widget, GdkEvent *event);
extern "C" void load_whitening_filter_button_callback(GtkWidget *widget, GdkEvent *event);
void *whitening_filter_thread(void *data);
void generate_whitening_filter();


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Gui callbacks
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern gboolean delete_event(GtkWidget *widget, GdkEvent * event, gpointer data);
extern void destroy(GtkWidget * widget, gpointer data);
void hide_widget(GtkWidget * widget, GdkEvent * event);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Timeout functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int close_splash_timeout(void* data);
int check_amp_timeout(void* data);
int fgfs_data_timeout(void* data);

#endif



