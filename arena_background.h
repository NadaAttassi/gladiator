#ifndef TEST_GTK_ARENA_BACKGROUND_H
#define TEST_GTK_ARENA_BACKGROUND_H
#include <gtk/gtk.h>
#include "menuarena.h"

void on_images_start_button_clickedd(GtkWidget *widget, gpointer data);


void on_images_start_button_click(GtkWidget *button, gpointer data) {
    selected_arena = (gchar *)data; // Update the selected_arena variable with the new path
    g_print("Selected arena path: %s\n", selected_arena);
}

// Fonction principale
void create_images_window() {


    GdkScreen *screen = gdk_screen_get_default();
    gint screen_width = 1500;
    gint screen_height = 400;

    // Utilisez une taille l�g�rement inf�rieure � celle de l'�cran pour la fen�tre
    gint window_width = screen_width ;
    gint window_height = screen_height ;



    // Add background image
    GtkWidget *background_image = gtk_image_new_from_file("bggg1.png");
    GdkPixbuf *bg_pixbuf = gdk_pixbuf_new_from_file("bggg1.png", NULL);
    gint bg_width = gdk_pixbuf_get_width(bg_pixbuf);
    gint bg_height = gdk_pixbuf_get_height(bg_pixbuf);
    Fenetre *images_window = initialiser_win("Images Window", NULL, "Images_Window", NULL, bg_height , bg_width, 0, 0, 0, 0, TRUE);
    create_window_n(images_window);

    GtkWidget *fixed = gtk_fixed_new();
    add_child(images_window->window, fixed);

    GdkPixbuf *resized_t_pixbuf = gdk_pixbuf_scale_simple(bg_pixbuf, window_width, window_height, GDK_INTERP_BILINEAR);

    GtkWidget *t_image = gtk_image_new_from_pixbuf(resized_t_pixbuf);
    g_object_unref(bg_pixbuf); // Lib�re la m�moire du pixbuf original
    gtk_fixed_put(GTK_FIXED(fixed), background_image, 0, 0);


    GtkWidget *message_player1 = create_label("Choose your Arena", "DRAGON HUNTER 48", "white");
    gtk_fixed_put(GTK_FIXED(fixed), message_player1, 150, 80);

    GtkWidget *grid = gtk_grid_new();

    // Chargement des images pour les boutons
    GdkPixbuf *image1 = gdk_pixbuf_new_from_file("arena7_button.png", NULL);
    GdkPixbuf *glad2 = gdk_pixbuf_new_from_file("arena5_button.png", NULL);
    GdkPixbuf *glad3 = gdk_pixbuf_new_from_file("arena6_button.png", NULL);
    GdkPixbuf *glad4 = gdk_pixbuf_new_from_file("arena8_button.png", NULL);

    // Création des boutons avec vos images
    GtkWidget *button;
    GtkWidget *image_widget;
    // Bouton 1
    image_widget = gtk_image_new_from_pixbuf(image1);
    button = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(button), image_widget);
    g_signal_connect(button, "clicked", G_CALLBACK(on_images_start_button_click),"arena7.png");
    gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

    // Bouton 2
    image_widget = gtk_image_new_from_pixbuf(glad2);
    button = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(button), image_widget);
    g_signal_connect(button, "clicked", G_CALLBACK(on_images_start_button_click), "arena5.png");
    gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);

    // Bouton 3
    image_widget = gtk_image_new_from_pixbuf(glad3);
    button = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(button), image_widget);
    g_signal_connect(button, "clicked", G_CALLBACK(on_images_start_button_click), "arena6.png");
    gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 1, 1);

    // Bouton 4
    image_widget = gtk_image_new_from_pixbuf(glad4);
    button = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(button), image_widget);
    g_signal_connect(button, "clicked", G_CALLBACK(on_images_start_button_click), "arena8.png");
    gtk_grid_attach(GTK_GRID(grid), button, 1, 1, 1, 1);



    // Libérer la mémoire des images
    g_object_unref(image1);
    g_object_unref(glad2);
    g_object_unref(glad3);
    g_object_unref(glad4);

    gtk_fixed_put(GTK_FIXED(fixed), grid, 200, 200);

    // Ajoutez le bouton "Start"
    GtkWidget *button_start = gtk_button_new_with_label("Start");
    g_signal_connect(button_start, "clicked", G_CALLBACK(on_images_start_button_clickedd), NULL);
    gtk_fixed_put(GTK_FIXED(fixed), button_start, 950, 570);

    // Ajoutez le bouton "Exit"
    GtkWidget *button_exit = gtk_button_new_with_label("Exit");
    g_signal_connect(button_exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_fixed_put(GTK_FIXED(fixed), button_exit, 950,650);

    show_window(images_window);
}


#endif //TEST_GTK_ARENA_BACKGROUND_H
