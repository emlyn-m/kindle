#pragma once

#include <gtk-2.0/gtk/gtk.h>

void gtk_table_add(GtkWidget* table, guint left, guint right, guint top, guint bottom, GtkWidget* elem);
GtkWidget* gtk_table_custom(guint rows, guint cols);
