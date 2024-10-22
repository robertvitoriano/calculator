#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <gtk/gtk.h>
#include <iostream>
#include <vector>

class Calculator {
private:
  GtkWidget *display;
  GtkWidget *window;
  GtkWidget *grid;
  int finalResult;
  char currentOperation;

  struct OperationSet {
    char operation;
    int leftOperand;
    int rightOperand;
  };

public:
  Calculator() : finalResult(0), currentOperation('\0') {}

  void setupWindow(int *argc, char ***argv) {
    gtk_init(argc, argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Object-Oriented Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 300);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    display = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), display, FALSE, FALSE, 2);

    grid = gtk_grid_new();
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 2);

    setupButtons();

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();
  }

  static void onButtonClicked(GtkButton *button, gpointer userData) {
    Calculator *calculator = static_cast<Calculator *>(userData);
    const char *buttonText = gtk_button_get_label(button);

    if (isdigit(buttonText[0]) || strchr("+-*/", buttonText[0])) {
      calculator->appendCharacterToDisplay(buttonText);
    } else if (strcmp(buttonText, "=") == 0) {
      calculator->onEqualClicked();
    } else if (strcmp(buttonText, "C") == 0) {
      calculator->onClearClicked();
    }
  }

  void appendCharacterToDisplay(const char *buttonText) {
    const char *currentText = gtk_entry_get_text(GTK_ENTRY(display));
    char newText[256];
    snprintf(newText, sizeof(newText), "%s%s", currentText, buttonText);
    gtk_entry_set_text(GTK_ENTRY(display), newText);
  }

  void onClearClicked() { gtk_entry_set_text(GTK_ENTRY(display), ""); }

  std::vector<OperationSet> getOperationSets(const char *currentText) {

    std::vector<OperationSet> operationSets;

    int leftOperand = 0;
    int rightOperand = 0;
    char operationType = '\0';

    for (int i = 0; i < strlen(currentText); i++) {
      if (isdigit(currentText[i])) {
        int digit = currentText[i] - '0';
        if (operationType == '\0') {
          leftOperand = leftOperand * 10 + digit;
        } else {
          rightOperand = rightOperand * 10 + digit;
        }
      } else {
        if (operationType != '\0') {
          operationSets.push_back({operationType, leftOperand, rightOperand});
          leftOperand = 0;
          rightOperand = 0;
        }
        operationType = currentText[i];
      }
    }
    if (operationType != '\0') {
      operationSets.push_back({operationType, leftOperand, rightOperand});
    }
    return operationSets;
  }
  void onEqualClicked() {
    const char *currentText = gtk_entry_get_text(GTK_ENTRY(display));

    finalResult = 0;

    std::vector<OperationSet> operationSets = getOperationSets(currentText);

    for (const auto &opSet : operationSets) {
      finalResult = getOperationResult(opSet);
    }
    char resultText[256];
    snprintf(resultText, sizeof(resultText), "%d", finalResult);
    gtk_entry_set_text(GTK_ENTRY(display), resultText);
  }

  int getOperationResult(const OperationSet &operationSet) {
    switch (operationSet.operation) {
    case '+':
      finalResult += operationSet.leftOperand + operationSet.rightOperand;
      break;
    case '-':
      finalResult += operationSet.leftOperand - operationSet.rightOperand;
      break;
    case '*':

      if (operationSet.rightOperand == 0 && operationSet.leftOperand == 0) {
        finalResult *= 1;
        break;
      }
      if (operationSet.leftOperand == 0) {
        finalResult *= operationSet.rightOperand;
        break;
      }
      if (operationSet.rightOperand == 0) {
        finalResult *= operationSet.leftOperand;
        break;
      }
      finalResult = operationSet.leftOperand * operationSet.rightOperand;
      break;
    case '/':
      if (operationSet.rightOperand != 0) {
        finalResult = operationSet.leftOperand / operationSet.rightOperand;
      } else {
        std::cerr << "Error: Division by zero\n";
        return 0;
      }
      break;
    default:
      std::cerr << "Unsupported operation: " << operationSet.operation << "\n";
      return 0;
    }
    return finalResult;
  }

  void setupButtons() {
    const char *buttons[4][4] = {{"+", "-", "*", "C"},
                                 {"7", "8", "9", "/"},
                                 {"4", "5", "6", "="},
                                 {"1", "2", "3", "0"}};

    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        GtkWidget *button = gtk_button_new_with_label(buttons[i][j]);
        gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1);
        g_signal_connect(button, "clicked", G_CALLBACK(onButtonClicked), this);
      }
    }
  }
};

int main(int argc, char *argv[]) {
  Calculator calculator;
  calculator.setupWindow(&argc, &argv);
  return 0;
}
