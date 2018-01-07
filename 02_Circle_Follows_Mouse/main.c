#include <gtk/gtk.h>

static GtkBuilder * g_gtk_builder = NULL;
static inline void * GTKW(char * widget) { return gtk_builder_get_object(g_gtk_builder, widget); }

static double g_mouse_x = 0;
static double g_mouse_y = 0;

G_MODULE_EXPORT gboolean on_draw(GtkWidget * widget, cairo_t * C) {
  cairo_set_source_rgb(C, 0.17, 0.63, 0.12);
  cairo_set_line_width(C, 2.0);
  cairo_arc(C, g_mouse_x, g_mouse_y, 20.0, 0.0, G_PI * 2.0);
  cairo_stroke(C);

  return FALSE;
}

G_MODULE_EXPORT gboolean on_motion(GtkWidget * widget, GdkEventMotion * event) {
  g_mouse_x = event->x;
  g_mouse_y = event->y;
  gtk_widget_queue_draw(widget);

  return FALSE;
}

int main(int ArgCount, char ** Args) {
  gtk_init(&ArgCount, &Args);
  g_gtk_builder = gtk_builder_new();
  gtk_builder_add_from_resource(g_gtk_builder, "/ui.glade", NULL);

  gtk_widget_set_events(GTKW("drawingarea"), GDK_ALL_EVENTS_MASK);
  g_signal_connect(GTKW("window"), "destroy", gtk_main_quit, 0);
  gtk_builder_connect_signals(g_gtk_builder, NULL);

  int w = 0;
  int h = 0;
  gtk_window_get_size(GTKW("window"), &w, &h);
  g_mouse_x = w / 2.0;
  g_mouse_y = h / 2.0;

  gtk_widget_show(GTKW("window"));
  gtk_main();

  return 0;
}
