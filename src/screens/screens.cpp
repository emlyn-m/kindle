#include <gtk-2.0/gtk/gtk.h>
#include <gtk-2.0/gdk/gdk.h>

#include "screens.hpp"
#include "../widgets/widgets.hpp"
#include "./table.hpp"

void set_screen(GtkButton* _button, GdkEvent* _event, void* data_v) {
    set_screen_data_t* data = (set_screen_data*) data_v;
    gtk_notebook_set_current_page(GTK_NOTEBOOK(data->stack), data->target_screen_idx);
}

GtkWidget* generate_life_screen( GtkWidget* stack, void (*set_screen)(GtkButton*, GdkEvent*, void*) ) {
    
    const guint ROWS = 5;
    const guint COLS = 5;
    
    set_screen_data_t* ctrl_data = (set_screen_data_t*) malloc(sizeof(set_screen_data_t));
	ctrl_data->stack = stack;
	ctrl_data->target_screen_idx = SCREEN_IDX_CTRL;
    
    GtkWidget* table = gtk_table_custom(ROWS, COLS);
    
   	gtk_table_add(table, 0, 1, 0, 1, time_widget());
	gtk_table_add(table, 1, 4, 0, 1, weather_widget());
	gtk_table_add(table, 4, 5, 0, 4, telem_widget(100));
	gtk_table_add(table, 0, 2, 1, 3, calendar_widget());
	gtk_table_add(table, 2, 4, 1, 5, tasks_widget());
	gtk_table_add(table, 0, 2, 3, 5, alerts_widget());
	gtk_table_add(table, 4, 5, 4, 5, button_widget((char*) "۶ৎ₊˚⊹⋆ৎ", set_screen, ctrl_data));
	
	return table;
	
}



GtkWidget* generate_ctrl_screen( GtkWidget* stack, void (*set_screen)(GtkButton*, GdkEvent*, void*) ) {
    return gtk_label_new("test");
}
