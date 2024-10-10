#include <ctype.h>
#include <gtk/gtk.h>

GtkWidget *display;
GtkWidget *window;
GtkWidget *grid;
char operation;

void append_character_to_display(GtkButton *button) {
  const char *current_text = gtk_entry_get_text(GTK_ENTRY(display));
  const char *button_text = gtk_button_get_label(button);

  char new_text[256];
  snprintf(new_text, sizeof(new_text), "%s%s", current_text, button_text);

  gtk_entry_set_text(GTK_ENTRY(display), new_text);
}

void on_number_clicked(GtkButton *button, gpointer user_data) {
  append_character_to_display(button);
}

void on_operation_clicked(GtkButton *button, gpointer user_data) {
  append_character_to_display(button);

  const char *data = (const char *)user_data;
  operation = data[0]; // Correctly assign the operation character
}

void on_clear_clicked(GtkButton *button, gpointer user_data) {
  gtk_entry_set_text(GTK_ENTRY(display), "");
}

void on_equal_clicked(GtkButton *button, gpointer user_data) {
  const char *current_text = gtk_entry_get_text(GTK_ENTRY(display));
  printf("Current text: %s\n", current_text);

  int left_operand = 0, right_operand = 0;
  char operation_type;

  sscanf(current_text, "%d%c%d", &left_operand, &operation_type,
         &right_operand);

  int result = 0;

  switch (operation_type) {
  case '+':
    result = left_operand + right_operand;
    break;
  case '-':
    result = left_operand - right_operand;
    break;
  default:
    printf("Unsupported operation: %c\n", operation_type);
    return;
  }

  char result_text[256];
  snprintf(result_text, sizeof(result_text), "%d", result);
  gtk_entry_set_text(GTK_ENTRY(display), result_text);
}

void setup_window(int *argc, char ***argv) {
  gtk_init(argc, argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Simple Calculator");
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 300);

  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  display = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(vbox), display, FALSE, FALSE, 2);

  grid = gtk_grid_new();
  gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 2);
}

int main(int argc, char *argv[]) {
  setup_window(&argc, &argv);

  const char *buttons[4][4] = {{"7", "8", "9", "C"},
                               {"4", "5", "6", "+"},
                               {"1", "2", "3", "-"},
                               {"0", "=", NULL, NULL}};

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (buttons[i][j] != NULL) {
        GtkWidget *button = gtk_button_new_with_label(buttons[i][j]);
        gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1);

        if (g_strcmp0(buttons[i][j], "C") == 0) {

          g_signal_connect(button, "clicked", G_CALLBACK(on_clear_clicked),
                           NULL);
        } else if (g_strcmp0(buttons[i][j], "=") == 0) {

          g_signal_connect(button, "clicked", G_CALLBACK(on_equal_clicked),
                           (gpointer) "Equal button pressed");
        } else if (isdigit(buttons[i][j][0])) {

          g_signal_connect(button, "clicked", G_CALLBACK(on_number_clicked),
                           NULL);
        } else {

          g_signal_connect(button, "clicked", G_CALLBACK(on_operation_clicked),
                           (gpointer)buttons[i][j]);
        }
      }
    }
  }

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
