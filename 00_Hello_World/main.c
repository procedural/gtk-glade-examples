#include <gtk/gtk.h>

static GtkBuilder * g_gtk_builder = NULL;
static inline void * GTKW(char * widget) { return gtk_builder_get_object(g_gtk_builder, widget); }

G_MODULE_EXPORT void print_hello() {
  printf("Hello, World!\n");
}

int main(int ArgCount, char ** Args) {
  gtk_init(&ArgCount, &Args);
  g_gtk_builder = gtk_builder_new();
  gtk_builder_add_from_resource(g_gtk_builder, "/ui.glade", NULL);

  g_signal_connect(GTKW("window"), "destroy", gtk_main_quit, 0);
  gtk_builder_connect_signals(g_gtk_builder, NULL);

  gtk_widget_show(GTKW("window"));
  gtk_main();

  return 0;
}
