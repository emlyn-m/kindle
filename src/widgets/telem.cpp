#include "./widgets.hpp"
#include "gtk/gtk.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifndef DISABLE_TELEM

#define DISABLE_TELEM_DEV 1
#define DISABLE_TELEM_PING 1
#define DISABLE_TELEM_IP 1

#define DISABLE_TELEM 1
#endif 

gboolean devices_update(gpointer* data_p) {
	if (DISABLE_TELEM_DEV) { return TRUE; }
	telem_t* data = (telem_t*) data_p;
	
	for (uint32_t i=0; i < data->n_devices; i++) {
		char* device_buf = (char*) malloc(64 * sizeof(char));
		snprintf(device_buf, 30, data->devices[i]->online ? "◉ %s" : "○ %s", data->devices[i]->name);
		gtk_label_set_text(GTK_LABEL(data->device_widgets[i]), device_buf);
		free(device_buf);
	}
	return TRUE;
}

gboolean ping_update(gpointer* data_p) {	
	if (DISABLE_TELEM_PING) { return TRUE; }
	telem_t* data = (telem_t*) data_p;
	
	FILE* fp = popen("ping -c 1 1.1.1.1 | head -n 2 | tail -n 1 | awk '{ split($8, arr, \"=\"); print arr[2] }'", "r");
	fflush(stdout);
 	if (fp == NULL) {
 		gtk_label_set_text(GTK_LABEL(data->ping_label), "[ err ]");
   		return TRUE;
    }
  	char path[1035];
	while (fgets(path, sizeof(path), fp) != NULL) {
		unsigned long ping = strtol(path, NULL, 10);
		unsigned long delta = (ping > data->ping) ? (ping - data->ping) : (data->ping - ping);
		
    	data->ping = ping;
		if (data->ping_count > 1) { data->jitter = ((data->jitter * (data->ping_count - 1)) + delta) / (data->ping_count); }
  	}
	
	data->ping_count++;
	if (data->ping_count > 2) { 
		char jitter_buf[32];
		snprintf(jitter_buf, 31, "%dms", (int) data->jitter);
		gtk_label_set_text(GTK_LABEL(data->jitter_label), jitter_buf);
	}
	
	char ping_buf[32];
	snprintf(ping_buf, 31, "%dms", (int) data->ping);
	gtk_label_set_text(GTK_LABEL(data->ping_label), ping_buf);
	
	return TRUE;
}

gboolean ip_update(gpointer* data_p) { 
	if (DISABLE_TELEM_IP) { return TRUE; }
	telem_t* data = (telem_t*) data_p;
	if (*(data->ip) == '\0') {
		FILE* fp = popen("curl -s ipinfo.io/ip", "r");
		if (fp == NULL) {
			gtk_label_set_text(GTK_LABEL(data->ip_label), "[ err ]");
			return TRUE;
		}
 		char path[32];
  		while (fgets(path, sizeof(path), fp) != NULL) { data->ip = path; }
    	gtk_label_set_text(GTK_LABEL(data->ip_label), data->ip);
	}
   	return TRUE;
}

GtkWidget* telem_widget(double update_freq_ms) {
	
	// wrapper
	telem_t* data = (telem_t*) malloc(sizeof(telem_t));
	data->n_devices = 5;
	data->device_widgets = (GtkWidget**) malloc(data->n_devices * sizeof(GtkWidget*));
	data->devices = (device_t**) malloc(data->n_devices * sizeof(device_t*));
	data->devices[0] = (device_t*) malloc(sizeof(device_t)); data->devices[0]->name=(char*) malloc(32 * sizeof(char)); memcpy(data->devices[0]->name, "autumn\0", 7); data->devices[0]->online=TRUE;
	data->devices[1] = (device_t*) malloc(sizeof(device_t)); data->devices[1]->name=(char*) malloc(32 * sizeof(char)); memcpy(data->devices[1]->name, "summer\0", 7); data->devices[1]->online=TRUE;
	data->devices[2] = (device_t*) malloc(sizeof(device_t)); data->devices[2]->name=(char*) malloc(32 * sizeof(char)); memcpy(data->devices[2]->name, "summer-wsl\0", 11); data->devices[2]->online=FALSE;
	data->devices[3] = (device_t*) malloc(sizeof(device_t)); data->devices[3]->name=(char*) malloc(32 * sizeof(char)); memcpy(data->devices[3]->name, "winter", 7); data->devices[3]->online=TRUE;
	data->devices[4] = (device_t*) malloc(sizeof(device_t)); data->devices[4]->name=(char*) malloc(32 * sizeof(char)); memcpy(data->devices[4]->name, "spring", 7); data->devices[4]->online=FALSE;
	data->n_services = 5;
	data->service_name_widgets = (GtkWidget**) malloc(data->n_devices * sizeof(GtkWidget*));
	data->service_status_widgets = (GtkWidget**) malloc(data->n_devices * sizeof(GtkWidget*));
	data->services = (service_t**) malloc(data->n_services * sizeof(service_t*));
	data->services[0] = (service_t*) malloc(sizeof(service_t)); data->services[0]->name = (char*) "TERRAIN"; data->services[0]->status = (char*) "nominal";
	data->services[1] = (service_t*) malloc(sizeof(service_t)); data->services[1]->name = (char*) "emlyn.xyz"; data->services[1]->status = (char*) "nominal";
	data->services[2] = (service_t*) malloc(sizeof(service_t)); data->services[2]->name = (char*) "nzpvt"; data->services[2]->status = (char*) "nominal";
	data->services[3] = (service_t*) malloc(sizeof(service_t)); data->services[3]->name = (char*) "XYZdle"; data->services[3]->status = (char*) "nominal";
	data->services[4] = (service_t*) malloc(sizeof(service_t)); data->services[4]->name = (char*) "winter"; data->services[4]->status = (char*) "nominal";

	GtkWidget* wrapper = gtk_vbox_new(FALSE, 5*SCALE);
	
	
	// telem label
	GtkWidget* telem_label = gtk_label_new("Telemetry");
	PangoFontDescription* font_desc_telem = pango_font_description_from_string(FONT_BOLD_14);
	gtk_misc_set_alignment (GTK_MISC(telem_label), 0.0, 0.5);
	gtk_widget_modify_font(telem_label, font_desc_telem);
	pango_font_description_free(font_desc_telem);
	gtk_container_set_border_width(GTK_CONTAINER(wrapper), 20*SCALE);
	gtk_box_pack_start(GTK_BOX(wrapper), telem_label, FALSE, FALSE, 0);

	GtkWidget* telem_info = gtk_vbox_new(false, 0);
	
	
	// devices
	GtkWidget* device_block = gtk_vbox_new(false, 5*SCALE);
	PangoFontDescription* font_desc_dev = pango_font_description_from_string(FONT_12);
	
	for (uint32_t i=0; i < data->n_devices; i++) {
		data->device_widgets[i] = gtk_label_new("");;
		gtk_misc_set_alignment (GTK_MISC(data->device_widgets[i]), 0.0, 0.5);
		gtk_widget_modify_font(data->device_widgets[i], font_desc_dev);
		gtk_box_pack_start(GTK_BOX(device_block), data->device_widgets[i], FALSE, FALSE, 5*SCALE);
	}
	
	pango_font_description_free(font_desc_dev);
	g_timeout_add(update_freq_ms, (GSourceFunc) devices_update, data);
	gtk_box_pack_start(GTK_BOX(telem_info), device_block, FALSE, FALSE, 5*SCALE);

	gtk_box_pack_start(GTK_BOX(wrapper), telem_info, TRUE, TRUE, 5*SCALE);

	
	// telem_stats
	PangoFontDescription* font_desc_label = pango_font_description_from_string(FONT_10);
	GtkWidget* telem_stats = gtk_vbox_new(FALSE, 0);
	
	// ping label
	GtkWidget* ping_info = gtk_hbox_new(false, 0);
	GtkWidget* ping_label = gtk_label_new("ping");
	GtkWidget* ping_value = gtk_label_new("-");
	data->ping = 0;
	data->last_ping = 0;
	data->ping_count = 0;
	data->ping_label = ping_value;
	gtk_widget_modify_font(ping_label, font_desc_label);
	gtk_widget_modify_font(ping_value, font_desc_label);
	gtk_box_pack_start(GTK_BOX(ping_info), ping_label, FALSE, FALSE, 5*SCALE);
	gtk_box_pack_end(GTK_BOX(ping_info), ping_value, FALSE, FALSE, 5*SCALE);
	gtk_box_pack_start(GTK_BOX(telem_stats), ping_info, FALSE, FALSE, 0);
	
	// jitter label
	GtkWidget* jitter_info = gtk_hbox_new(false, 0);
	GtkWidget* jitter_label = gtk_label_new("jitter");
	GtkWidget* jitter_value = gtk_label_new("-");
	data->jitter = 0;
	data->jitter_label = jitter_value;
	gtk_widget_modify_font(jitter_label, font_desc_label);
	gtk_widget_modify_font(jitter_value, font_desc_label);
	gtk_box_pack_start(GTK_BOX(jitter_info), jitter_label, FALSE, FALSE, 5*SCALE);
	gtk_box_pack_end(GTK_BOX(jitter_info), jitter_value, FALSE, FALSE, 5*SCALE);
	gtk_box_pack_start(GTK_BOX(telem_stats), jitter_info, FALSE, FALSE, 0);
	g_timeout_add(update_freq_ms, (GSourceFunc) ping_update, data);

	// ip label
	GtkWidget* ip_info = gtk_hbox_new(false, 0);
	GtkWidget* ip_label = gtk_label_new("ip");
	GtkWidget* ip_value = gtk_label_new("-");
	data->ip = (char*) malloc(30 * sizeof(char));
	memset(data->ip, 0, 30);
	data->ip_label = ip_value;
	gtk_widget_modify_font(ip_label, font_desc_label);
	gtk_widget_modify_font(ip_value, font_desc_label);
	gtk_box_pack_start(GTK_BOX(ip_info), ip_label, FALSE, FALSE, 5*SCALE);
	gtk_box_pack_end(GTK_BOX(ip_info), ip_value, FALSE, FALSE, 5*SCALE);
	gtk_box_pack_start(GTK_BOX(telem_stats), ip_info, FALSE, FALSE, 0);
	g_timeout_add(update_freq_ms, (GSourceFunc) ip_update, data);
	
	
	gtk_box_pack_start(GTK_BOX(wrapper), telem_stats, FALSE, FALSE, 5);
	pango_font_description_free(font_desc_label);	
	return wrapper;
}
