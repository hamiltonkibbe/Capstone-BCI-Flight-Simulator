/**********************************************************************

    Instrument.cpp
    Cairo pitch and roll indicators

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include "Instrument.h"



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	draw_pitch_plane
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void draw_pitch_plane(GtkWidget * instrument, double angle)
{
    // Local Variables
	cairo_surface_t     *plane = cairo_image_surface_create_from_png("../res/gui/img/pitch_plane.png");
	cairo_surface_t     *guage = cairo_image_surface_create_from_png("../res/gui/img/guage.png");
	cairo_surface_t     *cover = cairo_image_surface_create_from_png("../res/gui/img/cover.png");
	cairo_t             *cr = cairo_create(guage);

	double  x = INST_WIDTH / 2,
            y = INST_HEIGHT / 2,
            w = cairo_image_surface_get_width(plane),
            h = cairo_image_surface_get_height(plane);
    char buffer[6];

    // Generate text readout
    g_snprintf(buffer,6,"%3.1f",angle);
    cairo_text_extents_t extents;
    cairo_text_extents(cr,buffer,&extents);

    // Clip to circle
    cairo_arc(cr,x,y,72,0,M_PI*2);
    cairo_clip(cr);

    // Readout text box
    cairo_set_source_rgba(cr,0,0.03125,0.1758,0.5);
    cairo_rectangle(cr,x-50,y-35,50,20);
    cairo_fill(cr);

    // Readout text
    cairo_set_source_rgb(cr,1,1,1);
    cairo_select_font_face(cr, "Sans",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr,14);
    cairo_move_to(cr,x-15-extents.width,y-20);
    cairo_show_text(cr,buffer);


    // Draw the plane
    cairo_save(cr);
    cairo_translate(cr,x,y);
	cairo_rotate(cr,angle * M_PI / -180);
    cairo_translate(cr, -0.5 * w, -0.5 * h);
    cairo_rectangle(cr,0,0,w,h);
    cairo_clip(cr);
    cairo_set_source_surface (cr, plane, 0, 0);
    cairo_paint(cr);

    // Draw the cover
    cairo_restore(cr);
    cairo_set_source_surface(cr,cover,0,0);
    cairo_paint_with_alpha(cr,0.85);
    cairo_destroy(cr);

    // Paint to screen
    cr = gdk_cairo_create (instrument->window);
    cairo_set_source_surface(cr,guage,0,0);
    cairo_paint(cr);

    // Cleanup
    cairo_destroy(cr);
    cairo_surface_destroy(plane);
    cairo_surface_destroy(guage);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	draw_roll_plane
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void draw_roll_plane(GtkWidget * instrument, double angle)
{
	// Local Variables
	cairo_surface_t     *plane = cairo_image_surface_create_from_png("../res/gui/img/roll_plane.png");
	cairo_surface_t     *guage = cairo_image_surface_create_from_png("../res/gui/img/guage.png");
	cairo_surface_t     *cover = cairo_image_surface_create_from_png("../res/gui/img/cover.png");
    cairo_t             *cr = cairo_create(guage);

	double  x = INST_WIDTH / 2,
            y = INST_HEIGHT / 2,
            w = cairo_image_surface_get_width(plane),
            h = cairo_image_surface_get_height(plane);
    char buffer[7];

    // Generate text readout
    g_snprintf(buffer,7,"%3.1f",angle);
    cairo_text_extents_t extents;
    cairo_text_extents(cr,buffer,&extents);

    // Clip to circle
    cairo_arc(cr,x,y,72,0,M_PI*2);
    cairo_clip(cr);

    // Readout text box
    cairo_set_source_rgba(cr,0,0.03125,0.1758,0.5);
    cairo_rectangle(cr,x-50,y-35,50,20);
    cairo_fill(cr);

    // Readout text
    cairo_set_source_rgb(cr,1,1,1);
    cairo_select_font_face(cr, "Sans",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr,14);
    cairo_move_to(cr,x-15-extents.width,y-20);
    cairo_show_text(cr,buffer);

    // Draw the plane
    cairo_save(cr);
	cairo_translate(cr,x,y);
	cairo_rotate(cr,angle * M_PI / 180);
    cairo_translate(cr, -0.5 * w, -0.5 * h);
    cairo_rectangle(cr,0,0,w,h);
    cairo_clip(cr);
    cairo_set_source_surface (cr, plane, 0, 0);
    cairo_paint(cr);

    // Draw the cover
    cairo_restore(cr);
    cairo_set_source_surface(cr,cover,0,0);
    cairo_paint_with_alpha(cr,0.85);
    cairo_destroy(cr);

    // Paint to screen
    cr = gdk_cairo_create (instrument->window);
    cairo_set_source_surface(cr,guage,0,0);
    cairo_paint(cr);

    cairo_destroy(cr);
    cairo_surface_destroy(plane);
    cairo_surface_destroy(guage);
}



