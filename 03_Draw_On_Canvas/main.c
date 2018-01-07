#include <gtk/gtk.h>

static GtkBuilder * g_gtk_builder = NULL;
static inline void * GTKW(char * widget) { return gtk_builder_get_object(g_gtk_builder, widget); }

static cairo_surface_t * surface = NULL;

void clear_surface() {
  cairo_t * C = cairo_create(surface);

  cairo_set_source_rgb(C, 1, 1, 1);
  cairo_paint(C);

  cairo_destroy(C);
}

G_MODULE_EXPORT gboolean configure_event_cb(GtkWidget * widget, GdkEventConfigure * event) {
  if (surface)
    cairo_surface_destroy(surface);

  surface = gdk_window_create_similar_surface(
      gtk_widget_get_window(widget),
      CAIRO_CONTENT_COLOR,
      gtk_widget_get_allocated_width(widget),
      gtk_widget_get_allocated_height(widget)
  );

  clear_surface();

  return TRUE;
}

G_MODULE_EXPORT gboolean draw_cb(GtkWidget * widget, cairo_t * C) {
  cairo_set_source_surface(C, surface, 0, 0);
  cairo_paint(C);

  return FALSE;
}

void draw_brush(GtkWidget * widget, gdouble x, gdouble y) {
  cairo_t * C = cairo_create(surface);

  cairo_rectangle(C, x - 3, y - 3, 6, 6);
  cairo_fill(C);

  cairo_destroy(C);

  gtk_widget_queue_draw_area(widget, (int)(x) - 3, (int)(y) - 3, 6, 6);
}

G_MODULE_EXPORT gboolean button_press_event_cb(GtkWidget * widget, GdkEventButton * event) {
  if (surface == NULL)
    return FALSE;

  if (event->button == GDK_BUTTON_PRIMARY) {
    draw_brush(widget, event->x, event->y);
  }
  else if (event->button == GDK_BUTTON_SECONDARY) {
    clear_surface();
    gtk_widget_queue_draw(widget);
  }

  return TRUE;
}


G_MODULE_EXPORT gboolean motion_notify_event_cb( GtkWidget *widget, GdkEventMotion *event ) {
  if (surface == NULL)
    return FALSE;

  if (event->state & GDK_BUTTON1_MASK)
    draw_brush(widget, event->x, event->y);

  return TRUE;
}

static void close_window() {
  if (surface)
    cairo_surface_destroy(surface);

  gtk_main_quit();
}

int main(int ArgCount, char ** Args) {
  gtk_init(&ArgCount, &Args);
  g_gtk_builder = gtk_builder_new();
  gtk_builder_add_from_resource(g_gtk_builder, "/ui.glade", NULL);

  gtk_widget_set_events(GTKW("drawingarea"), GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);
  g_signal_connect(GTKW("window"), "destroy", close_window, 0);
  gtk_builder_connect_signals(g_gtk_builder, NULL);

  gtk_widget_show(GTKW("window"));
  gtk_main();

  return 0;
}
