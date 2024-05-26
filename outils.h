
#include "gtk_utils.h"
#include <windows.h>
#include <mmsystem.h>
// Function to create a window
GtkWidget* create_window(const char *title, int width, int height) {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), title);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    return window;
}

// Function to create buttons
GtkWidget* create_button(const char *label, const char *css_name) {
    GtkWidget *button = gtk_button_new_with_label(label);
    gtk_widget_set_name(button, css_name);
    return button;
}

// Function to create labels
GtkWidget* create_label(const char *text, const char *font_desc_str, const char *color_str) {
    GtkWidget *label = gtk_label_new(text);
    PangoFontDescription *font_desc = pango_font_description_from_string(font_desc_str);
    gtk_widget_override_font(label, font_desc);
    GdkRGBA color;
    gdk_rgba_parse(&color, color_str);
    gtk_widget_override_color(label, GTK_STATE_FLAG_NORMAL, &color);
    return label;
}

// Function to set CSS styles
void set_css_styles() {
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);

    const char *css_data =
            " button {"
            "   border: 1px solid #7c7c7c;"
            "   border-radius: 15px;"
            "   font: Ranger Eastwood 36;"
            "   color: #543c1c;"
            "   background-color: #e4d3aa;"
            "   background-image: none;"
            "   min-width: 280px;"
            "   text-align: center;"
            "   padding: 10px 20px;"
            "}"
            " button:hover {"
            "   background-color: #fff7d7;"
            "   color: #ffffff;"
            "}"
            " button:hover:active {"
            "   background-color: #e8a154;"
            "   color: #ffffff;"
            "}"
            " button#custom-button-player1, button#custom-button-player2, button#custom-button-player3 {"
            "   border-color: #00ff00;"
            "   min-width: 150px;"
            "   padding: 5px 10px;"

            "}"
            " button#pause_button {"
            "   border: 5px solid #7c7c7c;"
            "   border-radius: 10%;"
            "   font: Ranger Eastwood 24;"
            "   color: #543c1c;"
            "   background-color: #e4d3aa;"
            "   background-image: none;"
            "   min-width: 55px;"
            "   min-height: 55px;"
            "   text-align: center;"
            "   padding: 2px 2px;"
            "}"
            ;

    gtk_css_provider_load_from_data(provider, css_data, -1, NULL);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

// Function to create a fixed container
GtkWidget *create_fixed() {
    return GTK_WIDGET(gtk_fixed_new());
}

// Function to update the background image
void update_background_image(GtkWidget *fixed, GtkWidget **background_image, const char *file_path) {
    if (*background_image) {
        gtk_widget_destroy(*background_image);
    }
    *background_image = gtk_image_new_from_file(file_path);
    gtk_fixed_put(GTK_FIXED(fixed), *background_image, 0, 0);
    gtk_widget_show(*background_image);
}


// Function to play sound
void play_sound(const char *sound_file) {
    PlaySound(sound_file, NULL, SND_FILENAME | SND_ASYNC);
}
