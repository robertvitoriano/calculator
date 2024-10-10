#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <gtk/gtk.h>
#include <vector> // Added include for vector

GtkWidget *display;
GtkWidget *window;
GtkWidget *grid;
char operation;

struct OperationSet {
  char operation;
  int leftOperand;
  int rightOperand;
};

void appendCharacterToDisplay(GtkButton *button) {
  const char *currentText = gtk_entry_get_text(GTK_ENTRY(display));
  const char *buttonText = gtk_button_get_label(button);

  char newText[256];
  snprintf(newText, sizeof(newText), "%s%s", currentText, buttonText);

  gtk_entry_set_text(GTK_ENTRY(display), newText);
}

void onNumberClicked(GtkButton *button, gpointer userData) {
  appendCharacterToDisplay(button);
}

void onOperationClicked(GtkButton *button, gpointer userData) {
  appendCharacterToDisplay(button);
  const char *data = (const char *)userData;
  operation = data[0];
}

void onClearClicked(GtkButton *button, gpointer userData) {
  gtk_entry_set_text(GTK_ENTRY(display), "");
}

int getOperationResult(const OperationSet *operationSet) {
  int result;
  switch (operationSet->operation) {
  case '+':
    result = operationSet->leftOperand + operationSet->rightOperand;
    break;
  case '-':
    result = operationSet->leftOperand - operationSet->rightOperand;
    break;
  case '*':
    result = operationSet->leftOperand * operationSet->rightOperand;
    break;
  case '/':
    if (operationSet->rightOperand != 0) {
      result = operationSet->leftOperand / operationSet->rightOperand;
    } else {
      printf("Error: Division by zero\n");
      return 0;
    }
    break;
  default:
    printf("Unsupported operation: %c\n", operationSet->operation);
    return 0;
  }

  return result;
}

void onEqualClicked(GtkButton *button, gpointer userData) {
  const char *currentText = gtk_entry_get_text(GTK_ENTRY(display));
  printf("Current text: %s\n", currentText);

  std::vector<OperationSet> operationSets;

  // Temporary variables to hold current operands and operation
  int leftOperand = 0;
  int rightOperand = 0;
  char operationType = '\0';

  // Iterate through currentText to parse operands and operators
  for (int i = 0; i < strlen(currentText); i++) {
    if (isdigit(currentText[i])) {
      // Construct the left operand if we haven't yet
      if (operationType == '\0') {
        leftOperand = leftOperand * 10 + (currentText[i] - '0');
      } else {
        // Construct the right operand after the operator
        rightOperand = rightOperand * 10 + (currentText[i] - '0');
      }
    } else {
      // When we encounter an operator, save the operation and reset
      // rightOperand
      if (operationType != '\0') {
        // Store the current operation in the set
        operationSets.push_back({operationType, leftOperand, rightOperand});
        printf("Left OPERAND %d, Right OPERAND %d, Operation %c\n", leftOperand,
               rightOperand, operationType);

        // Reset for next operation
        leftOperand = 0;
        rightOperand = 0;
      }
      operationType = currentText[i]; // Update operation
    }
  }

  // Push the last operation after the loop
  if (operationType != '\0') {
    operationSets.push_back({operationType, leftOperand, rightOperand});
    printf("Left OPERAND %d, Right OPERAND %d, Operation %c\n", leftOperand,
           rightOperand, operationType);
  }

  // Calculate results of the operations
  int finalResult = 0;
  for (const auto &opSet : operationSets) {
    finalResult = getOperationResult(&opSet); // Call to the updated function
  }

  // Update the display with the final result
  char resultText[256];
  snprintf(resultText, sizeof(resultText), "%d", finalResult);
  gtk_entry_set_text(GTK_ENTRY(display), resultText);
}

void setupWindow(int *argc, char ***argv) {
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
  setupWindow(&argc, &argv);

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
          g_signal_connect(button, "clicked", G_CALLBACK(onClearClicked), NULL);
        } else if (g_strcmp0(buttons[i][j], "=") == 0) {
          g_signal_connect(button, "clicked", G_CALLBACK(onEqualClicked),
                           (gpointer) "Equal button pressed");
        } else if (isdigit(buttons[i][j][0])) {
          g_signal_connect(button, "clicked", G_CALLBACK(onNumberClicked),
                           NULL);
        } else {
          g_signal_connect(button, "clicked", G_CALLBACK(onOperationClicked),
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
