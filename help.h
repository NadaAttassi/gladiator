
#ifndef TEST_GTK_HELP_H
#define TEST_GTK_HELP_H
#include <gtk/gtk.h>
#include "gtk_utils.h"

void display_help_dialog() {
    GtkWidget *dialog;
    FILE *file;
    char help_text[1000000];  // Assuming the help text won't exceed 1000 characters

    // Open the help.txt file for reading
    file = fopen("E:\\ILISI_s1\\GTK\\Gladiator\\help.txt", "r");
    if (file == NULL) {
        g_print("Error opening file!\n");
        return;
    }
    GtkWidget *scroll;
    char buffer[100];
    // Read the contents of the file into help_text
    help_text[0] = '\0';

    // Read the file line by line and append each line to help_text
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Concatenate the current line to help_text
        strcat(help_text, buffer);
    }

    // Close the file
    fclose(file);

    // Create a dialog with the help text as its content
    dialog = gtk_message_dialog_new(NULL,
                                    GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_INFO,
                                    GTK_BUTTONS_OK,
                                    "%s",
                                    help_text);

    // Set the dialog title
    gtk_window_set_title(GTK_WINDOW(dialog), "Help");

    // Run the dialog
    gtk_dialog_run(GTK_DIALOG(dialog));

    // Destroy the dialog
    gtk_widget_destroy(dialog);
}


#endif //TEST_GTK_HELP_H
