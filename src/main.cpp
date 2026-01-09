#include "cairo.h"
#include "gdk/gdk.h"
#include <gtk-2.0/gtk/gtk.h>
#include <fontconfig/fontconfig.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "widgets.hpp"

#define CELL_ROWCOUNT 5
#define CELL_COLCOUNT 5
#define PADDING 20
#define OFFSET_SIZE 5

static gboolean shadow_expose(GtkWidget* widget, GdkEventExpose* event, gpointer _data) {	
    cairo_t* cr = gdk_cairo_create(widget->window);
    
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, OFFSET_SIZE/2.0, OFFSET_SIZE*1.5, event->area.width - 2*OFFSET_SIZE, event->area.height - 2*OFFSET_SIZE);
    cairo_fill(cr);
    cairo_destroy(cr);
    
    return TRUE;
}


void gtk_table_add(GtkWidget* table, GtkWidget* elem,
                   guint left, guint right, guint top, guint bottom)
{
    GdkColor black = {0, 0, 0, 0};
    GdkColor white = {0, 255<<8, 255<<8, 255<<8};

    GtkWidget* event_box_inner = gtk_event_box_new();
    GtkWidget* event_box_outer = gtk_event_box_new();
    GtkWidget* event_box_shadow = gtk_event_box_new();

    gtk_container_add(GTK_CONTAINER(event_box_inner), elem);
    gtk_container_add(GTK_CONTAINER(event_box_outer), event_box_inner);
    gtk_container_add(GTK_CONTAINER(event_box_shadow), event_box_outer);

    gtk_widget_modify_bg(event_box_inner, GTK_STATE_NORMAL, &white);
    gtk_widget_modify_bg(event_box_outer, GTK_STATE_NORMAL, &black);
    gtk_widget_modify_bg(event_box_shadow, GTK_STATE_NORMAL, &white);


    gtk_container_set_border_width(GTK_CONTAINER(event_box_inner), 2*SCALE);
    gtk_container_set_border_width(GTK_CONTAINER(event_box_outer), OFFSET_SIZE);
    gtk_container_set_border_width(GTK_CONTAINER(event_box_shadow), PADDING);


    g_signal_connect(event_box_shadow, "expose-event", G_CALLBACK(shadow_expose), NULL);

    gtk_table_attach(
        GTK_TABLE(table), event_box_shadow,
        left, right,
        top, bottom,
        (GtkAttachOptions) (GTK_FILL|GTK_EXPAND),
        (GtkAttachOptions) (GTK_FILL|GTK_EXPAND),
        0, 0
    );
}

int main(int argc, char* argv[]) {
	
	FcConfigAppFontAddFile(FcConfigGetCurrent(), (FcChar8*) "./src/include/GeistMono-Regular.ttf");
	
	GtkWidget *window;
	gtk_init (&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "L:A_N:application_ID:xyz.emlyn.kindle_PC:N_O:R");
	
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	GtkWidget* table = gtk_table_new(CELL_ROWCOUNT, CELL_COLCOUNT, TRUE);
	gtk_table_set_row_spacings(GTK_TABLE(table), 0); //PADDING*SCALE);
	gtk_table_set_col_spacings(GTK_TABLE(table), 0); //PADDING*SCALE);
	gtk_container_set_border_width(GTK_CONTAINER(table), 2*PADDING*SCALE);
	gtk_container_add(GTK_CONTAINER(window), table);
		
	GdkColor color = { 0, 255<<8, 255<<8, 255<<8 };
	gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color);
		
	gtk_table_add(table, time_widget(),      0, 1, 0, 1);
	gtk_table_add(table, weather_widget(),   1, 4, 0, 1);
	gtk_table_add(table, telem_widget(100), 4, 5, 0, 4);
	gtk_table_add(table, calendar_widget(),  0, 2, 1, 3);
	gtk_table_add(table, tasks_widget(),     2, 4, 1, 5);
	gtk_table_add(table, alerts_widget(),    0, 2, 3, 5);
	gtk_table_add(table, label_widget(),     4, 5, 4, 5);
	
	// gtk_table_add(table, gtk_label_new("time"),     0, 1, 0, 1);
	// gtk_table_add(table, gtk_label_new("weather"),  1, 4, 0, 1);
	// gtk_table_add(table, gtk_label_new("telem"),    4, 5, 0, 4);
	// gtk_table_add(table, gtk_label_new("cal"),      0, 2, 1, 3);
	// gtk_table_add(table, gtk_label_new("tasks"),    2, 4, 1, 5);
	// gtk_table_add(table, gtk_label_new("alerts"),   0, 2, 3, 5);
	// gtk_table_add(table, gtk_label_new("label"),    4, 5, 4, 5);

	
	gtk_widget_show_all(window);
	
	gtk_main();
	
	return 0;
}
