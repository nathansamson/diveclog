#ifndef DISPLAY_GTK_H
#define DISPLAY_GTK_H

#include <gtk/gtk.h>
#include <gdk/gdk.h>

extern GtkWidget *main_window;

/* we want a progress bar as part of the device_data_t - let's abstract this out */
typedef struct {
	GtkWidget *bar;
} progressbar_t;

/*extern gboolean on_delete(GtkWidget* w, gpointer data);
extern void on_destroy(GtkWidget* w, gpointer data);
extern void quit(GtkWidget *w, gpointer data);*/

extern const char *divelist_font;
extern void set_divelist_font(const char *);

extern void import_dialog(GtkWidget *, gpointer);
extern void report_error(GError* error);
extern int process_ui_events(void);
extern void update_progressbar(progressbar_t *progress, double value);

extern GtkWidget *dive_profile_widget(void);
extern GtkWidget *extended_dive_info_widget(void);
extern GtkWidget *equipment_widget(void);

extern void dive_info_init(GtkBuilder *builder);
extern void dive_list_init(GtkBuilder *builder);

#endif
