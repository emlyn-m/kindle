#include "./widgets.hpp"
#include "gtk/gtk.h"
#include <cstdlib>
#include <cstring>

GtkWidget* weather_widget() {
	
	weather_t* weather = (weather_t*) malloc(sizeof(weather_t));
	weather->num_weather_events = 10;
	
	// wrapper
	GtkWidget* wrapper = gtk_hbox_new(FALSE, 30*SCALE);
	gtk_container_set_border_width(GTK_CONTAINER(wrapper), 5*SCALE);
	PangoFontDescription* font_chance = pango_font_description_from_string(FONT_8);
	PangoFontDescription* font_time = pango_font_description_from_string(FONT_BOLD_8);

	for (uint32_t i=0; i < weather->num_weather_events; i++) {
		GtkWidget* event_vbox = gtk_vbox_new(FALSE, 0);
		GtkWidget* chance = gtk_label_new("10%");
		GtkWidget* time = gtk_label_new("8 AM");
	
		gtk_widget_modify_font(chance, font_chance);
		gtk_widget_modify_font(time, font_time);
		gtk_misc_set_alignment(GTK_MISC(chance), 0.5, 0.0);
		gtk_misc_set_alignment(GTK_MISC(time), 0.5, 1.0);
		gtk_box_pack_start(GTK_BOX(event_vbox), chance, TRUE, TRUE, 10*SCALE);
		gtk_box_pack_end(GTK_BOX(event_vbox), time, TRUE, TRUE, 10*SCALE);
	
		gtk_container_set_border_width(GTK_CONTAINER(wrapper), 10);
		gtk_box_pack_start(GTK_BOX(wrapper), event_vbox, TRUE, TRUE, 0);
	}

	pango_font_description_free(font_chance);
	pango_font_description_free(font_time);

	return wrapper;
}
