#ifndef GUI_H_
#define GUI_H_

// ====================================================================
// Includes
// ====================================================================

// Libraries
#include <glade/glade.h>

// Project Headers
#include "Callbacks.h"
#include "SystemParameters.h"

void init_gui();

void init_data();

void add_plots();

void connect_event_handlers();

void close_log(GtkWidget * widget, GdkEvent * event);

void splash_status(const char* status);



#endif
