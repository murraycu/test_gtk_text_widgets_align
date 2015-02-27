/**
 * Copyright (C) 2015 Murray Cumming.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 */

/**
 * Build like so:
 * gcc test_gtk_text_widgets_align.c -o test -Wall `pkg-config gtk+-3.0 --libs --cflags`
 */

#include<gtk/gtk.h>

GList *text_widgets = NULL;
const char* EXAMPLE_TEXT_SINGLE =
  "Single line of example text.";
const char* EXAMPLE_TEXT_MULTIPLE =
  "Multiple lines of example text. Multiple lines of example text. Multiple lines of example text. Multiple lines of example text.";
 
static gint
on_delete_event (GtkWidget* w, GdkEventAny* e, gpointer data)
{
  gtk_main_quit();
  return TRUE;
}

static void
on_check_button_halign_toggled (GtkToggleButton *togglebutton, gpointer user_data)
{
  gboolean set = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (togglebutton));
  GtkAlign align = set ? GTK_ALIGN_END : GTK_ALIGN_BASELINE;

  GList *l = text_widgets;
  while (l != NULL)
  { 
    GtkWidget *text_widget = GTK_WIDGET (l->data);
    gtk_widget_set_halign (text_widget, align);
    
    l = l->next;
  }
}

static void
on_check_button_misc_alignment_toggled (GtkToggleButton *togglebutton, gpointer user_data)
{
  gboolean set = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (togglebutton));
  float align = set ? 1.0 : 0.5;

  GList *l = text_widgets;
  while (l != NULL)
  { 
    GtkWidget *text_widget = GTK_WIDGET (l->data);
    if (GTK_IS_MISC (text_widget)) {
      gtk_misc_set_alignment (GTK_MISC (text_widget), align, 0.5);
    }
    
    l = l->next;
  }
}


static void
on_check_button_justify_toggled (GtkToggleButton *togglebutton, gpointer user_data)
{
  gboolean set = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (togglebutton));
  GtkJustification justification = set ? GTK_JUSTIFY_RIGHT : GTK_JUSTIFY_LEFT;

  GList *l = text_widgets;
  while (l != NULL)
  {
    GtkWidget *text_widget = GTK_WIDGET (l->data);

    //Use either set_justify() or set_justificatoin(), depending on the widget type:
    if (GTK_IS_LABEL (text_widget)) {
      gtk_label_set_justify (GTK_LABEL (text_widget), justification);
    } else if (GTK_IS_TEXT_VIEW (text_widget)) {
      gtk_text_view_set_justification (GTK_TEXT_VIEW (text_widget), justification);
    }
    
    l = l->next;
  }
}

static void
on_check_button_background_color_toggled (GtkToggleButton *togglebutton, gpointer user_data)
{
  gboolean set = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (togglebutton));
  const gchar* str_color = "red";

  GList *l = text_widgets;
  while (l != NULL)
  { 
    GtkWidget *text_widget = GTK_WIDGET (l->data);

    GdkRGBA color;
    gdk_rgba_parse(&color, str_color);

    if (set) {
      gtk_widget_override_background_color (text_widget, GTK_STATE_FLAG_NORMAL, &color);
    } else {
      gtk_widget_override_background_color (text_widget, GTK_STATE_FLAG_NORMAL, NULL);
    }
    
    l = l->next;
  }
}

static GtkWidget *
create_frame (const gchar *title, GtkWidget *widget)
{
  GtkWidget *frame;
  GtkWidget *label;

  frame = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);

  label = gtk_label_new (NULL);
  gchar *bold_title = g_strdup_printf ("<b>%s</b>", title);
  gtk_label_set_markup (GTK_LABEL (label), bold_title);
  g_free (bold_title);
  gtk_frame_set_label_widget (GTK_FRAME (frame), label);
  gtk_widget_show (label);

  gtk_container_add (GTK_CONTAINER (frame), widget);
  gtk_widget_set_margin_top (widget, 6);
  gtk_widget_set_margin_start (widget, 12);
  gtk_widget_show (widget);

  return frame;
}

static GtkWidget *
create_grid ()
{
  GtkWidget *grid = gtk_grid_new ();
  gtk_grid_set_row_spacing (GTK_GRID(grid), 6);
  gtk_grid_set_column_spacing (GTK_GRID(grid), 6);
  return grid;
}

static GtkWidget *
create_title_label (const gchar *title)
{
  GtkWidget *label = gtk_label_new(title);
  gtk_widget_set_halign (label, GTK_ALIGN_END);
  gtk_widget_set_valign (label, GTK_ALIGN_START);
  return label;
}

static void
add_row_to_grid_of_text_widgets (GtkGrid *grid, gint row, const gchar *title, GtkWidget *widget)
{
  GtkWidget *title_label = create_title_label(title);
  gtk_grid_attach (GTK_GRID (grid), title_label, 0, row, 1, 1);
  gtk_widget_show (title_label);

  gtk_grid_attach (GTK_GRID (grid), widget, 1, row, 1, 1);
  gtk_widget_show (widget);
}

/**
 * Make the widget high enough for right-alignment/justification to be more obvious
 */
static void
make_widget_higher (GtkWidget *widget)
{
  gtk_widget_set_size_request (widget, -1, 120);
  gtk_widget_set_valign (widget, GTK_ALIGN_START);
}

static GtkWidget *
create_grid_of_check_buttons ()
{
  GtkWidget *grid;
  GtkWidget *check_button;

  grid = create_grid ();

  check_button = gtk_check_button_new_with_label ("gtk_widget_set_halign (widget, GTK_ALIGN_END)");
  gtk_grid_attach (GTK_GRID (grid), check_button, 0, 0, 1, 1);
  gtk_widget_show (check_button);
  g_signal_connect(G_OBJECT(check_button),
 	     "toggled",
 	     G_CALLBACK(on_check_button_halign_toggled),
 	     NULL);

  check_button = gtk_check_button_new_with_label ("gtk_misc_set_alignment (widget, 1.0): Label-only. Deprecated.");
  gtk_grid_attach (GTK_GRID (grid), check_button, 0, 1, 1, 1);
  gtk_widget_show (check_button);
  g_signal_connect(G_OBJECT(check_button),
 	     "toggled",
 	     G_CALLBACK(on_check_button_misc_alignment_toggled),
 	     NULL);

  check_button = gtk_check_button_new_with_label ("gtk_label_set_justify() / gtk_text_view_set_justification()");
  gtk_grid_attach (GTK_GRID (grid), check_button, 0, 2, 1, 1);
  gtk_widget_show (check_button);
  g_signal_connect(G_OBJECT(check_button),
 	     "toggled",
 	     G_CALLBACK(on_check_button_justify_toggled),
 	     NULL);

  check_button = gtk_check_button_new_with_label ("Background Color");
  gtk_grid_attach (GTK_GRID (grid), check_button, 0, 3, 1, 1);
  gtk_widget_show (check_button);
  g_signal_connect(G_OBJECT(check_button),
 	     "toggled",
 	     G_CALLBACK(on_check_button_background_color_toggled),
 	     NULL);

  return grid;
}

static GtkWidget *
create_grid_of_text_widgets ()
{
  GtkWidget *grid;
  GtkWidget *label;
  GtkWidget *entry;
  GtkWidget *textview;
  GtkTextBuffer *buffer;

  grid = create_grid ();
  gtk_grid_set_row_spacing (GTK_GRID(grid), 6);
  gtk_grid_set_column_spacing (GTK_GRID(grid), 6);

  // Row 0:
  label = gtk_label_new(EXAMPLE_TEXT_SINGLE);
  make_widget_higher (label);
  add_row_to_grid_of_text_widgets (GTK_GRID (grid), 0, "GtkLabel (single line):", label);
  text_widgets = g_list_append (text_widgets, label);

  // Row 1:
  label = gtk_label_new(EXAMPLE_TEXT_MULTIPLE);
  make_widget_higher (label);
  gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);
  add_row_to_grid_of_text_widgets (GTK_GRID (grid), 1, "GtkLabel (multi-line):", label);
  text_widgets = g_list_append (text_widgets, label);

  // Row 2:
  entry = gtk_entry_new ();
  gtk_entry_set_text (GTK_ENTRY (entry), EXAMPLE_TEXT_SINGLE);
  add_row_to_grid_of_text_widgets (GTK_GRID (grid), 2, "GtkEntry:", entry);
  text_widgets = g_list_append (text_widgets, entry);

  // Row 3:
  textview = gtk_text_view_new ();
  make_widget_higher (textview);
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW(textview), GTK_WRAP_WORD);
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview));
  gtk_text_buffer_set_text (buffer, EXAMPLE_TEXT_MULTIPLE, -1);
  add_row_to_grid_of_text_widgets (GTK_GRID (grid), 3, "GtkTextView:", textview);
  text_widgets = g_list_append (text_widgets, textview);

  // Row 4:
  textview = gtk_text_view_new ();
  make_widget_higher (textview);
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW(textview), GTK_WRAP_WORD);
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview));
  gtk_text_buffer_set_text (buffer, EXAMPLE_TEXT_SINGLE, -1);
  add_row_to_grid_of_text_widgets (GTK_GRID (grid), 4, "GtkTextView:", textview);
  text_widgets = g_list_append (text_widgets, textview);

  return grid;
}

int
main (int   argc,
      char *argv[])
{
  GtkWidget *window;
  GtkWidget *grid_outer;
  GtkWidget *frame;
  GtkWidget *grid;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 300, 300);
  gtk_window_set_title (GTK_WINDOW (window), "test");
  gtk_container_set_border_width (GTK_CONTAINER (window), 12);

  grid_outer = create_grid ();

  grid = create_grid_of_check_buttons ();
  frame = create_frame ("Options", grid);
  gtk_grid_attach (GTK_GRID (grid_outer), frame, 0, 0, 1, 1);
  gtk_widget_show (frame);
  

  grid = create_grid_of_text_widgets ();
  frame = create_frame ("Text Widgets", grid);
  gtk_grid_attach (GTK_GRID (grid_outer), frame, 0, 1, 1, 1);
  gtk_widget_show (frame);

  gtk_container_add (GTK_CONTAINER(window), grid_outer);
  gtk_widget_show (grid_outer);

  g_signal_connect(G_OBJECT(window),
 	     "delete_event",
 	     G_CALLBACK(on_delete_event),
 	     NULL);

  gtk_widget_show (window);

  gtk_main ();

  return 0;
}

