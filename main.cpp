#include <gtk/gtk.h>

// Global variable for the display entry
GtkWidget *display;

void on_number_clicked(GtkButton *button, gpointer user_data)
{
  const char *current_text = gtk_entry_get_text(GTK_ENTRY(display));
  const char *button_text = gtk_button_get_label(button);

  // Append the clicked number to the display
  char new_text[256];
  snprintf(new_text, sizeof(new_text), "%s%s", current_text, button_text);
  gtk_entry_set_text(GTK_ENTRY(display), new_text);
}

void on_clear_clicked(GtkButton *button, gpointer user_data)
{
  gtk_entry_set_text(GTK_ENTRY(display), "");
}

void on_equal_clicked(GtkButton *button, gpointer user_data)
{
  const char *current_text = gtk_entry_get_text(GTK_ENTRY(display));
  const char *data = (const char *)user_data;

  printf("User data: %s\n", data);
  int result = 0;
  sscanf(current_text, "%d", &result);

  char result_text[256];
  snprintf(result_text, sizeof(result_text), "%d", result);
  gtk_entry_set_text(GTK_ENTRY(display), result_text);
}

int main(int argc, char *argv[])
{
  // Initialize GTK
  gtk_init(&argc, &argv);

  // Create main window
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Simple Calculator");
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);

  // Create a vertical box layout
  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  // Create display entry
  display = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(vbox), display, FALSE, FALSE, 2);

  // Create grid for buttons
  GtkWidget *grid = gtk_grid_new();
  gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 2);

  // Create buttons for digits and operations
  const char *buttons[4][4] = {
      {"7", "8", "9", "C"},
      {"4", "5", "6", "+"},
      {"1", "2", "3", "-"},
      {"0", "=", NULL, NULL}};

  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      if (buttons[i][j] != NULL)
      {
        GtkWidget *button = gtk_button_new_with_label(buttons[i][j]);
        gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1);

        if (g_strcmp0(buttons[i][j], "C") == 0)
        {
          g_signal_connect(button, "clicked", G_CALLBACK(on_clear_clicked), NULL);
        }
        else if (g_strcmp0(buttons[i][j], "=") == 0)
        {
          g_signal_connect(button, "clicked", G_CALLBACK(on_equal_clicked), NULL);
        }
        else
        {
          g_signal_connect(button, "clicked", G_CALLBACK(on_number_clicked), NULL);
        }
      }
    }
  }

  // Connect window close signal
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  // Show all widgets
  gtk_widget_show_all(window);

  // Start the GTK main loop
  gtk_main();

  return 0;
}
