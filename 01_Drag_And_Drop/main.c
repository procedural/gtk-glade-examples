#include <time.h>
#include <string.h>
#include <gtk/gtk.h>

static GtkBuilder * g_gtk_builder = NULL;
static inline void * GTKW(char * widget) { return gtk_builder_get_object(g_gtk_builder, widget); }

static inline void GtkTextBufferPushText(GtkTextBuffer * buffer, char * text, size_t len) {
  GtkTextIter end;
  gtk_text_buffer_get_end_iter(buffer, &end);
  gtk_text_buffer_insert(buffer, &end, text, len);
}

G_MODULE_EXPORT void log_clicked() {
  gtk_box_set_child_packing(GTKW("box"), GTKW("expander"), gtk_expander_get_expanded(GTKW("expander")) == FALSE, TRUE, 0, GTK_PACK_START);
}

G_MODULE_EXPORT void dragged(GtkWidget *widget, GdkDragContext *context, int x, int y, GtkSelectionData *data) {
  time_t timer = time(NULL);
  char * timestamp = asctime(localtime(&timer));
  GtkTextBufferPushText(gtk_text_view_get_buffer(GTKW("log")), timestamp, strlen(timestamp));

  char ** uris = gtk_selection_data_get_uris(data);
  for (char ** uri = uris; uri[0]; uri += 1) {
    char * path = g_filename_from_uri(uri[0], NULL, NULL);
    GtkTextBufferPushText(gtk_text_view_get_buffer(GTKW("log")), path, strlen(path));
    GtkTextBufferPushText(gtk_text_view_get_buffer(GTKW("log")), "\n", 1);
  }
}

int main(int ArgCount, char ** Args) {
  gtk_init(&ArgCount, &Args);
  g_gtk_builder = gtk_builder_new();
  gtk_builder_add_from_resource(g_gtk_builder, "/ui.glade", NULL);

  gtk_drag_dest_set(GTKW("droparea"), (GtkDestDefaults)(GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_DROP), NULL, 0, GDK_ACTION_COPY);
  gtk_drag_dest_add_uri_targets(GTKW("droparea"));

  g_signal_connect(GTKW("window"), "destroy", gtk_main_quit, 0);
  gtk_builder_connect_signals(g_gtk_builder, NULL);

  gtk_widget_show(GTKW("window"));
  gtk_main();

  return 0;
}
