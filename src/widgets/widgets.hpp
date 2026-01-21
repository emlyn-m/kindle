#pragma once

#include <cstdint>
#include <gtk-2.0/gtk/gtk.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define SCALE 1

#define FONT_8  "Geist Mono, 4"
#define FONT_10 "Geist Mono, 5"
#define FONT_12 "Geist Mono, 6"
#define FONT_14 "Geist Mono, 7"
#define FONT_16 "Geist Mono, 8"
#define FONT_18 "Geist Mono, 9"
#define FONT_20 "Geist Mono, 10"
#define FONT_30 "Geist Mono, 15"
#define FONT_BOLD_8  "Geist Mono, Bold 4"
#define FONT_BOLD_10 "Geist Mono, Bold 5"
#define FONT_BOLD_12 "Geist Mono, Bold 6"
#define FONT_BOLD_14 "Geist Mono, Bold 7"
#define FONT_BOLD_16 "Geist Mono, Bold 8"
#define FONT_BOLD_18 "Geist Mono, Bold 9"
#define FONT_BOLD_20 "Geist Mono, Bold 10"
#define FONT_BOLD_30 "Geist Mono, Bold 15"


typedef struct ClockData {
	GtkWidget* month_widget;
	GtkWidget* day_widget;
	GtkWidget* time_widget;
} clock_data_t;
typedef struct DeviceData {
	char* name;
	gboolean online;
} device_t;
typedef struct ServiceData {
	char* name;
	char* status;
} service_t;
typedef struct TelemetryData {
	guint8 n_devices;
	guint8 n_services;
	device_t** devices;
	service_t** services;
	unsigned long ping;
	unsigned long last_ping;
	double jitter;
	unsigned long ping_count;
	char* ip;
	
	GtkWidget** device_widgets;
	GtkWidget** service_name_widgets;
	GtkWidget** service_status_widgets;
	GtkWidget* ping_label;
	GtkWidget* jitter_label;
	GtkWidget* ip_label;
} telem_t;

typedef struct CalendarEvent {
	uint id;
	int64_t start_time;
	int64_t end_time;
	char* title;
} cal_event_t;
typedef struct CalendarData {
	uint32_t num_events;
	uint32_t show_events;
	cal_event_t** events;
	cal_event_t* active_event;
	
	GtkWidget* title_widget;
	GtkWidget* title_date_widget;
	GtkWidget* title_day_widget;
	
	GtkWidget* active_event_widget;
	GtkWidget* active_event_title_widget;
	GtkWidget* active_event_duration_widget;
	
	GtkWidget** events_title_widgets;
	GtkWidget** events_time_widgets;
} calendar_t;

typedef struct WeatherEvent {
	uint64_t time;
} weather_ev_t;
typedef struct WeatherData {
	uint32_t num_weather_events;
	weather_ev_t* events;
} weather_t;

typedef struct AlertEvent {
	uint64_t time;
	char* category;
	uint8_t severity;
	char* msg;
} alert_ev_t;
typedef struct AlertData {
	uint32_t num_alerts;
	alert_ev_t** alerts;
	GtkWidget** alert_meta_widgets;
	GtkWidget** alert_widgets;
} alert_t;

typedef struct Task {
	char* task;
	gboolean completed;
} task_ev_t;
typedef struct TaskData {
	uint32_t num_tasks;
	task_ev_t** tasks;
	GtkWidget** task_checkboxes;
	GtkWidget** task_widgets;
} task_t;
typedef struct {
    GtkWidget *drawing_area;
    float value;      // 0.0 to 1.0
    gboolean dragging;
    void (*callback_change)(float);
    void (*callback_release)(float);
} KindleSlider;


GtkWidget* time_widget();
GtkWidget* telem_widget(double update_freq_ms);
GtkWidget* button_widget(char* label_content, void (*callback)(GtkButton*, GdkEvent*, void*), void* data);
GtkWidget* weather_widget();
GtkWidget* calendar_widget();
GtkWidget* alerts_widget();
GtkWidget* tasks_widget();
GtkWidget* label_widget(char* label_content);
KindleSlider* kindle_slider_new(void (*callback_change)(float), void (*callback_release)(float));
