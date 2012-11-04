/**********************************************************************

    Instrument_Interface.cpp
    GUI functions for cairo instruments

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include "Callbacks.h"
#include "Instrument.h"


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	inst_pitch_expose
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
gboolean inst_pitch_expose(GtkWidget * instrument, GdkEventExpose * event)
{
	// Local Variables
	cairo_t * cr = gdk_cairo_create (instrument->window);

	cairo_rectangle (cr,
			event->area.x, event->area.y,
			event->area.width, event->area.height);

	cairo_clip (cr);

	draw_pitch_plane(instrument, 0);

	cairo_destroy(cr);

	return FALSE;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	inst_roll_expose
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
gboolean inst_roll_expose(GtkWidget * instrument, GdkEventExpose * event)
{
	// Local Variables
	cairo_t * cr = gdk_cairo_create (instrument->window);

	cairo_rectangle (cr,
			event->area.x, event->area.y,
			event->area.width, event->area.height);

	cairo_clip (cr);

	draw_roll_plane(instrument, 0);

	cairo_destroy(cr);

	return FALSE;
}


