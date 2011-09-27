/* info.c */
/* creates the UI for the info frame - 
 * controlled through the following interfaces:
 * 
 * void flush_dive_info_changes(struct dive *dive)
 * void show_dive_info(struct dive *dive)
 *
 * called from gtk-ui:
 * GtkWidget *extended_dive_info_widget(void)
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "dive.h"
#include "display.h"
#include "display-gtk.h"
#include "divelist.h"

static GtkEntry *location, *buddy, *divemaster;
static GtkTextBuffer *notes;
static int location_changed = 1, notes_changed = 1;
static int divemaster_changed = 1, buddy_changed = 1;

static char *get_text(GtkTextBuffer *buffer)
{
	GtkTextIter start;
	GtkTextIter end;

	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_end_iter(buffer, &end);
	return gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
}

/* old is NULL or a valid string, new is a valid string
 * NOTW: NULL and "" need to be treated as "unchanged" */
static int text_changed(char *old, char *new)
{
	return ((old && strcmp(old,new)) ||
		(!old && strcmp("",new)));
}

void flush_dive_info_changes(struct dive *dive)
{
	char *old_text;
	int changed = 0;

	if (!dive)
		return;

	if (location_changed) {
		old_text = dive->location;
		dive->location = gtk_editable_get_chars(GTK_EDITABLE(location), 0, -1);
		if (text_changed(old_text,dive->location))
			changed = 1;
		if (old_text)
			g_free(old_text);
	}

	if (divemaster_changed) {
		old_text = dive->divemaster;
		dive->divemaster = gtk_editable_get_chars(GTK_EDITABLE(divemaster), 0, -1);
		if (text_changed(old_text,dive->divemaster))
			changed = 1;
		if (old_text)
			g_free(old_text);
	}

	if (buddy_changed) {
		old_text = dive->buddy;
		dive->buddy = gtk_editable_get_chars(GTK_EDITABLE(buddy), 0, -1);
		if (text_changed(old_text,dive->buddy))
			changed = 1;
		if (old_text)
			g_free(old_text);
	}

	if (notes_changed) {
		old_text = dive->notes;
		dive->notes = get_text(notes);
		if (text_changed(old_text,dive->notes))
			changed = 1;
		if (old_text)
			g_free(old_text);
	}
	if (changed)
		mark_divelist_changed(TRUE);
}

#define SET_TEXT_ENTRY(x) \
	gtk_entry_set_text(x, dive && dive->x ? dive->x : "")

void show_dive_info(struct dive *dive)
{
	struct tm *tm;
	const char *text;
	char buffer[80];

	/* dive number and location (or lacking that, the date) go in the window title */
	tm = gmtime(&dive->when);
	text = dive->location;
	if (!text)
		text = "";
	if (*text) {
		snprintf(buffer, sizeof(buffer), "Dive #%d - %s", dive->number, text);
	} else {
		snprintf(buffer, sizeof(buffer), "Dive #%d - %s %02d/%02d/%04d at %d:%02d",
			dive->number,
			weekday(tm->tm_wday),
			tm->tm_mon+1, tm->tm_mday,
			tm->tm_year+1900,
			tm->tm_hour, tm->tm_min);
	}
	text = buffer;
	if (!dive->number)
		text += 10;     /* Skip the "Dive #0 - " part */
	gtk_window_set_title(GTK_WINDOW(main_window), text);

	SET_TEXT_ENTRY(divemaster);
	SET_TEXT_ENTRY(buddy);
	SET_TEXT_ENTRY(location);
	gtk_text_buffer_set_text(notes, dive && dive->notes ? dive->notes : "", -1);
}

void dive_info_init(GtkBuilder *builder)
{
	GtkTextView *notes_view;

	divemaster = GTK_ENTRY(gtk_builder_get_object(builder, "dive_info_divemaster"));
	buddy = GTK_ENTRY(gtk_builder_get_object(builder, "dive_info_buddy"));
	location = GTK_ENTRY(gtk_builder_get_object(builder, "dive_info_location"));
	notes_view = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "dive_info_notes"));
	notes = gtk_text_view_get_buffer(notes_view);
}
