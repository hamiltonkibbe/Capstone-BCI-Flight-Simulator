#ifndef INSTRUMENT_H_
#define INSTRUMENT

// ====================================================================
// Includes
// ====================================================================
#include <gtk/gtk.h>
#include <cmath>

// ====================================================================
// Macros
// ====================================================================
#define INST_WIDTH 140
#define INST_HEIGHT 140


// ====================================================================
// Function prototypes
// ====================================================================

// Instrument manipulation functions
void draw_roll_plane(GtkWidget * instrument, double angle);
void draw_pitch_plane(GtkWidget * instrument, double angle);

#endif
