#include <gtk/gtk.h>
#include "arena.h"
#include "gtk_utils.h"
#include "help.h"
#include "window.h"
#include "nada/windowboxscroll.h"
#include "nada/Gridheader.h"
#include "nada/label.h"
#include "createChara.h"
#include "arena_background.h"
#include "outils.h"
GtkWidget *life_image_player1; // Image widget for player 1 life
GtkWidget *life_image_player2; // Image widget for player 2 life
GtkWidget *time_image;
GtkWidget *table_time_image;

GtkWidget *button_start;
GtkWidget *button_create;
GtkWidget *button_exit;
GtkWidget *button_help;



int selected_avatar ;
// Global variable to store the selected player
Actor *selected_player = NULL;
Machine *selected_machine = NULL ;
int isSmart = 1 ;





//----------------------------------------- create caracter full code --------------------------------

// Fonction de vérification des attributs du joueur avant de créer la fenêtre des images
void check_player_attributes(GtkWidget *button, gpointer data) {
    // Afficher les valeurs des attributs pour déboguer
    printf("\ncc Health: %d, Attack_P: %d, Defense_d: %d, Speed: %d\n",
           player->health, player->Attack_P, player->Defense_d, player->speed);

    // Vérifiez si le joueur a été initialisé et que ses attributs sont > 0
    if (player == NULL || player->health == 0 || player->Attack_P == 0 || player->Defense_d == 0 || player->speed == 0 ) {
        // Afficher une boîte de dialogue pour informer l'utilisateur
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Les attributs du joueur doivent etre superieurs a zero !");
        // Définir la position de la boîte de dialogue au centre de l'écran
        gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return; // Arrêter l'exécution de la fonction si les attributs ne sont pas valides
    }

    // Si les attributs sont valides, appelez la fonction create_images_window
    create_images_window();
}


void ok_button_clicked_create(GtkWidget *widget, gpointer data) {
    CallbackData *callback_data = (CallbackData *)data;
    GtkWidget *entry = callback_data->entry;
    Window *myWindow = callback_data->window;

    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    printf("Texte de l'entrée : %s\n", text);

    // Ensure player is allocated
    if (player == NULL) {
        player = (Actor *)malloc(sizeof(Actor));
        if (player == NULL) {
            fprintf(stderr, "\nFailed to allocate memory for player");
            exit(1);
        }
        player->name = NULL;
        player->health = 0;
        player->Attack_P = 0;
        player->Defense_d = 0;
        player->speed = 0;
    }

    // Free the existing name if necessary
    if (player->name != NULL) {
        free(player->name);
    }

    // Duplicate the entry text
    player->name = strdup(text);
    if (player->name == NULL) {
        fprintf(stderr, "\nFailed to allocate memory for player name");
        exit(1);
    }

    // Initialize player attributes based on sliders
    player->health = (int)gtk_range_get_value(GTK_RANGE(options.sliders[2]));
    player->Attack_P = (int)gtk_range_get_value(GTK_RANGE(options.sliders[1]));
    player->Defense_d = (int)gtk_range_get_value(GTK_RANGE(options.sliders[0]));
    player->speed = (int)gtk_range_get_value(GTK_RANGE(options.sliders[3]));



    GdkScreen *screen = gdk_screen_get_default();
    gint screen_width = gdk_screen_get_width(screen);
    gint screen_height = gdk_screen_get_height(screen);

    gchar* title = "Nouvelle fenêtre";
    gchar* icon = NULL;
    gint width = screen_width;
    gint height = screen_height;
    gint position = 1;
    gint x = 0;
    gint y = 0;
    gint resizable = 1;

    Window* myNewWindow = initializeWindow("f2", title, icon, height, width, position, x, y, resizable, NULL, "E:\\ILISI_s1\\GTK\\Gladiator\\resourses\\temple.jpg", screen_width, screen_height);
    createWindow(myNewWindow);
    gtk_window_set_position(GTK_WINDOW(myNewWindow->window), GTK_WIN_POS_CENTER);

    Grille* myGrid = init_Grille("maGrille", width, height, 5, 0, NULL, 0, 0);
    create_Grille(myGrid);
    gtk_container_add(GTK_CONTAINER(myNewWindow->window), myGrid->Grille);

    LabelObj* titre = init_label("CREATE YOUR Player", "lbl", 0, 1, 0, 0, 0, 0);
    create_label22(titre);
    ajout_au_Grille(myGrid->Grille, titre->label, 0, 0);

    Box *left_box = initializeBox("left-box", 'v', 1, 0, "#FAFAD2", 0.7);
    createBox(left_box);
    gtk_widget_set_size_request(left_box->box, 200, 600);
    ajout_au_Grille(myGrid->Grille, left_box->box, 0, 1);

    LabelObj* myLabel = init_label(text, "nom", FALSE, FALSE, 100, 20, 0, 12);
    create_label22(myLabel);

    GtkWidget *edit_name_button = gtk_button_new_with_label("Modifier le nom");
    noms = g_malloc(sizeof(Noms));
    noms->ancienNom = g_strdup(text);
    g_signal_connect(edit_name_button, "clicked", G_CALLBACK(modifier), myLabel->label);

    LabelObj* label_Defense = init_label("Defense", "lelabel", FALSE, FALSE, 100, 20, 0, 12);
    create_label22(label_Defense);
    LabelObj* label_Attack = init_label("Attaque", "lelabel", FALSE, FALSE, 100, 20, 0, 12);
    create_label22(label_Attack);
    LabelObj* label_PV = init_label("Vitalite", "lelabel", FALSE, FALSE, 100, 20, 0, 12);
    create_label22(label_PV);
    LabelObj* label_speed = init_label("Vitesse", "lelabel", FALSE, FALSE, 100, 20, 0, 12);
    create_label22(label_speed);


    options.remaining_points = TOTAL_POINTS;
    options.remaining_points_label = gtk_label_new("Remaining Points: 200");
    gtk_widget_set_name(GTK_WIDGET(options.remaining_points_label), "remaining-points-label");

    addToBox(left_box->box, options.remaining_points_label, 0, 1, 1, 1);
    options.sliders[0] = create_slider(&options);
    options.sliders[1] = create_slider(&options);
    options.sliders[2] = create_slider(&options);
    options.sliders[3] = create_slider(&options);

    addToBox(left_box->box, options.sliders[0], 0, FALSE, FALSE, FALSE);
    addToBox(left_box->box, label_Defense->label, 0, FALSE, FALSE, FALSE);
    addToBox(left_box->box, options.sliders[1], 0, 1, 1, 1);
    addToBox(left_box->box, label_Attack->label, 0, FALSE, FALSE, FALSE);
    addToBox(left_box->box, options.sliders[2], 0, 1, 1, 1);
    addToBox(left_box->box, label_PV->label, 0, FALSE, FALSE, FALSE);
    addToBox(left_box->box, options.sliders[3], 0, 1, 1, 1);
    addToBox(left_box->box, label_speed->label, 0, FALSE, FALSE, FALSE);

    GtkWidget *edit_name_button1 = gtk_button_new_with_label("Edit Name Character");
    g_signal_connect(edit_name_button1, "clicked", G_CALLBACK(modifier), myLabel->label);

    addToBox(left_box->box, edit_name_button1, 0, FALSE, FALSE, 0);
    addToBox(left_box->box, myLabel->label, 0, 1, 1, 1);

    // Create the photo area and add it to the grid
    photo_area = initializeBox("photo-area", 'v', 1, 0, "", 1);
    createBox(photo_area);
    gtk_widget_set_size_request(photo_area->box, 800, 300);
    const gchar *photo_path = "nada/knight.png"; // Replace with the path to your default photo
    current_photo_widget = create_or_update_photo_widget(photo_path);
    addToBox(photo_area->box, current_photo_widget, 0, 1, 1, 1);

    Actor players[] = {
        {
            .name = "Light Mage",
            .sprite_sheet = "Light_Mage_Spritelist.png",
            .image = "nada/mage.png",
            .current_frame = 0,
            .current_action = 0,
            .action_frames = {7, 7, 8, 10, 4, 12, 13, 8, 3, 5},
            .x_position = 10,
            .y_position = 450,
            .sprite_width = 3584.0 / 15,
            .sprite_height = 239,
            .health =0 ,
            .Attack_P =0,
            .Defense_d = 0,
            .speed =0,
            .attack = gladiator_attack
        },
        {
            .name = "Wanderer Magician",
            .sprite_sheet = "Wanderer_Magican_Spritelist.png",
            .image ="nada/magician.png",
            .current_frame = 0,
            .current_action = 0,
            .action_frames = {8, 7, 8, 7, 9, 15, 6, 8, 4, 4},
            .x_position = 10,
            .y_position = 10,
            .sprite_width = 3584.0 / 16,
            .sprite_height = 225,
            .health = 0,
            .Attack_P = 0,
            .Defense_d = 0,
            .speed =0,
            .attack = gladiator_attack
        },
        {
            .name = "Samurai Commander",
            .sprite_sheet = "Samurai_Commander_Spritelist.png",
            .image = "nada/commander.png",
            .current_frame = 0,
            .current_action = 0,
            .action_frames = {4, 4, 8, 6, 9, 14, 10, 8, 6, 6},
            .x_position = 10,
            .y_position = 10,
            .sprite_width = 3584.0 / 15,
            .sprite_height = 239,
            .health = 0,
            .Attack_P = 0,
            .Defense_d = 0,
            .speed =  0,
            .attack = gladiator_attack
        },
        {
            .name = "Crimson Warrior",
            .sprite_sheet = "Samurai_Archer_Spritelist.png",
            .image = "nada/samurai.png",
            .current_frame = 0,
            .current_action = 0,
            .action_frames = {6, 6, 6, 6, 6, 8, 8, 6, 6, 6},
            .x_position = 10,
            .y_position = 10,
            .sprite_width = 3584.0 / 12,
            .sprite_height = 286,
            .health = 0,
            .Attack_P = 0,
            .Defense_d = 0,
            .speed =  0,
            .attack = gladiator_attack
        }
    };
    // Create the Player thumbnails and add them to the grid
    GtkWidget *thumbnails_container = create_Player_thumbnails(players, 4);
    gtk_widget_set_name(thumbnails_container, "thumbnails-container");


    // Création d'un bouton simple
    ButtonSimple* play_btn = init_button_simple("start", "Play",NULL, 10,10, "arial", "black","", FALSE, TRUE, 1600, 400);
    creer_button_Simple(play_btn);

    // Création du conteneur grid
    GtkWidget *container = gtk_grid_new();
    gtk_widget_set_name(container, "container");
    // Ajout de photo_area au conteneur grid
    gtk_grid_attach(GTK_GRID(container), photo_area->box, 0, 0, 1, 1); // position (0,0), spanning 1 cell

    // Ajout des vignettes des joueurs au conteneur grid
    gtk_grid_attach(GTK_GRID(container), thumbnails_container, 1, 0, 1, 1); // position (1,0), spanning 1 cell

    // Ajout du bouton au conteneur grid
    gtk_grid_attach(GTK_GRID(container), play_btn->button, 1, 1, 1, 1); // position (1,1), spanning 1 cell

    // Ajout du conteneur grid à la grille
    ajout_au_Grille(myGrid->Grille, container, 2, 1);

    // Assurez-vous de libérer la mémoire si nécessaire


    //    g_signal_connect(simpleButton->button, "clicked", G_CALLBACK(ok_button_clicked), callback_data);
    load_css2();
    // Connectez d'abord le signal "clicked" du bouton à une fonction de vérification
    g_signal_connect(play_btn->button, "clicked", G_CALLBACK(check_player_attributes), NULL);


    // Show the new window
    gtk_widget_show_all(myNewWindow->window);

    gtk_widget_destroy(myWindow->window);

    // Start a new GTK main loop for the new window
    gtk_main();
}

int create_charachter() {


    // Création de la fenêtre principale
    gchar* title = "Nom du personnage";
    gchar* icon = NULL;
    gint height = 200;
    gint width = 400;
    gint position = 1; // Position centrale
    gint x = 0;
    gint y = 0;
    gint resizable = 0; // Redimensionnable

    // Initialisation de la fenêtre principale
    Window* myWindow = initializeWindow("f1",title, icon, height, width, position, x, y, resizable, NULL, "image.jpeg", 100, 100);
    createWindow(myWindow);
    // Positionner la fenêtre au centre de l'écran
    gtk_window_set_position(GTK_WINDOW(myWindow->window), GTK_WIN_POS_CENTER);

    // Laisser les icônes désactivées
    gtk_window_set_icon(GTK_WINDOW(myWindow->window), NULL);
    // Rendre la fenêtre modale
    gtk_window_set_modal(GTK_WINDOW(myWindow->window), TRUE);

    Grille* myGrid = init_Grille("maGrille", 400, 200, 5, 0, NULL, 100, 100);
    create_Grille(myGrid);
    gtk_container_add(GTK_CONTAINER(myWindow->window), myGrid->Grille);



    // Création et initialisation du label
    LabelObj* myLabel = init_label("Veuillez choisir le nom du personnage", "mon_label", FALSE, FALSE, 100, 20, 0, 10);
    create_label22(myLabel);

    // Appliquer le style au label
    apply_label_style(myLabel->label);

    ajout_au_Grille(myGrid->Grille,myLabel->label,myLabel->x,myLabel->y);
    //image

    imageObj* img=init_img("nada/golden.png",400,400);
    create_image(img);
    ajout_au_Grille(myGrid->Grille,img->image,0,0);
    //ENTRY

    EntryObj* entry= init_entry ("Nom du personnage...",NULL, 1,0,10,40,0,14);
    create_entry(entry);
    ajout_au_Grille(myGrid->Grille,entry->entry ,entry->x,entry->y);
    // Chemin vers l'icône
    // const gchar *icon_path = "icon.png";
    CallbackData *callback_data = g_new(CallbackData, 1);
    callback_data->entry = entry->entry;
    callback_data->window = myWindow;

    // Création d'un bouton simple
    ButtonSimple* simpleButton = init_button_simple("mon_bouton", "OK",NULL, 10,10, "Ranger Eastwood 24", "","white", FALSE, TRUE, 0, 190);
    creer_button_Simple(simpleButton);
    // ajout_au_Grille(myGrid->Grille,simpleButton->button,simpleButton->x,simpleButton->y);

    ajout_au_Grille(myGrid->Grille, simpleButton->button,simpleButton->x,simpleButton->y);

    g_signal_connect(simpleButton->button, "clicked", G_CALLBACK(ok_button_clicked_create), callback_data);

    //*******************************
    // Connexion de l'événement de fermeture de la fenêtre principale
    g_signal_connect(myWindow->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Affichage de la fenêtre principale et démarrage de la boucle GTK
    gtk_widget_show_all(myWindow->window);
    //
    // gtk_widget_show(myDialog->dialog);

    gtk_main();
    // Libération de la mémoire allouée
    if (myWindow) {
        g_free(myWindow->title);
        g_free(myWindow->icon);
        free(myWindow);
    }

    if (simpleButton) {
        g_free(simpleButton->name);
        g_free(simpleButton->label);
        free(simpleButton);
    }

    // Libération de la mémoire allouée pour le label
    if (myLabel) {
        if (myLabel->text)
            g_free(myLabel->text);
        if (myLabel->name)
            g_free(myLabel->name);
        free(myLabel);
    }
    if (myGrid) {
        g_free(myGrid->name);
        //if (myGrid->bgCLor.couleur)
        // g_free(myGrid->bgCLor.couleur);
        if (myGrid->imageBg.image)
            g_free(myGrid->imageBg.image);
        free(myGrid);
    }

    return 0;
}


void on_button_clicked(GtkWidget *button, gpointer data) {
    const char *button_label = gtk_button_get_label(GTK_BUTTON(button));
    if (g_strcmp0(button_label, "Start") == 0) {
        if (selected_player && selected_machine) {
            g_print("Starting the game with %s\n", selected_player->name);
            //run_game(selected_player);  // Call run_game with the selected player
            create_images_window(selected_player);
        } else {
            g_print("No player or machine selected!\n");
        }
    } else if (g_strcmp0(button_label, "CREATE GLADIATOR") == 0) {
        g_print("Create Gladiator clicked\n");
        create_charachter();
        // Code to create a new gladiator
    } else if (g_strcmp0(button_label, "EXIT") == 0) {
        g_print("Exit clicked\n");
        gtk_main_quit();
    }  else if (g_strcmp0(button_label, "HELP") == 0) {
        // Code to display help dialog
        printf("ok a khalid hhhh");
        display_help_dialog();
    }
}

void on_select_AVATER_clicked(GtkWidget *button, gpointer data) {
    const char *button_label = gtk_button_get_label(GTK_BUTTON(button));
    if (g_strcmp0(button_label, "Select 1") == 0) {
        selected_avatar = 1 ;

    } else if (g_strcmp0(button_label, "Select 1") == 0) {

        selected_avatar = 2 ;
    }
}
// Callback for the start button in the images window
void on_images_start_button_clickedd(GtkWidget *button_label, gpointer data) {
    // Add the logic you want to execute when the start button in the images window is clicked
    if(player) selected_player=player;
    if (selected_player) {
        g_print("Starting the game with %s\n", selected_player->name);
        run_game(selected_player , selected_machine , isSmart); // Appelez run_game avec le joueur sélectionné
    } else {
        g_print("No player selected!\n");


    }
}
void play_audio(const char *file_path) {
    // Open the audio file
    if (PlaySound(file_path, NULL, SND_FILENAME | SND_ASYNC) == 0) {
        // Error handling if the audio file fails to play
        printf("Error playing audio: %d\n", GetLastError());
    }
}

GtkWidget *player_table; // Add this global variable
GtkWidget *machine_table;
// Callback for button click events
void on_select_button_clicked(GtkWidget *button, Actor *data) {
    selected_player = data;
    g_print("Selected player: %s\n", data->name);  // Print the selected player name for verification
    // Update the player_machine_table_image with the selected player's image
    GdkPixbuf *player_image = gdk_pixbuf_new_from_file(data->image, NULL);
    gtk_image_set_from_pixbuf(GTK_IMAGE(player_table), player_image) ;
}

void on_select_machine_button_clicked(GtkWidget *button, Machine *data) {
    selected_machine = data;
    g_print("Selected machine: %s\n", data->image );  // Print the selected player name for verification
    // Update the player_machine_table_image with the selected player's image
    GdkPixbuf *machine_image = gdk_pixbuf_new_from_file(data->image, NULL);
    gtk_image_set_from_pixbuf(GTK_IMAGE(machine_table), machine_image) ;
}


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the character selection window
    GtkWidget *window = create_window("Character Selection", 800, 600);

    // Create a fixed container
    GtkWidget *fixed = create_fixed();
    GtkWidget *background_image = NULL;
    gtk_container_add(GTK_CONTAINER(window), fixed);

    // Set the initial background image
    update_background_image(fixed, &background_image, "resourses/Menu.jpg");



    // Get the width and height of the background image
    GdkPixbuf *background_image_pixbuf = gdk_pixbuf_new_from_file("E:\\ILISI_s1\\GTK\\Gladiator\\resourses\\Menu.jpg", NULL);
    gint background_image_width = gdk_pixbuf_get_width(background_image_pixbuf);
    gint background_image_height = gdk_pixbuf_get_height(background_image_pixbuf);

    // Create player data
    Actor players[3] = {
            {
                    .name = "Samuri",
                    .sprite_sheet = "resourses/Samurai_Spritelist.png",
                    .image = "resourses/vector1.png",
                    .current_frame = 0,
                    .current_action = 0,
                    .action_frames = {
                            .standing = 6,
                            .walking = 9,
                            .running = 8,
                            .attacking1 = 4,
                            .attacking2 = 5,
                            .attacking3 = 4,
                            .defending = 2,
                            .jumping = 8,
                            .hurting = 3,
                            .dying = 6
                    },
                    .x_position = 10,
                    .y_position = 450,
                    .sprite_width = 256,
                    .sprite_height = 256,
                    .health = 200,
                    .Attack_P = 10 ,
                    .Defense_d = 8 ,
                    .speed = 11 ,
                    .attack = gladiator_attack
            },
            {
                    .name = "Fire vizard",
                    .sprite_sheet = "resourses/Fire_Mage_Spritelist.png",
                    .image = "resourses/vector2.png",
                    .current_frame = 0,
                    .current_action = 0,
                    .action_frames = {
                            .standing = 7,
                            .walking = 6,
                            .running = 8,
                            .attacking1 = 4,
                            .attacking2 = 4,
                            .attacking3 = 8,
                            .defending = 14,
                            .jumping = 9,
                            .hurting = 3,
                            .dying = 6
                    },
                    .x_position = 10,
                    .y_position = 450,
                    .sprite_width = 3584/15,
                    .sprite_height = 239,
                    .health = 300,
                    .Attack_P = 18 ,
                    .Defense_d = 0 ,
                    .speed = 9 ,
                    .attack = gladiator_attack
            },
            {
                    .name = "Knight",
                    .sprite_sheet = "resourses/knight_1_spritelist.png",
                    .image = "resourses/vector3.png",
                    .current_frame = 0,
                    .current_action = 0,
                    .action_frames = {
                            .standing = 1,
                            .walking = 7,
                            .running = 6,
                            .attacking1 = 5,
                            .attacking2 = 4,
                            .attacking3 = 4,
                            .defending = 1,
                            .jumping = 2,
                            .hurting = 6,
                            .dying = 6
                    },
                    .x_position = 10,
                    .y_position = 420,
                    .sprite_width = 2816/10,
                    .sprite_height = 2816/10  + 1,
                    .health = 150,
                    .Attack_P = 20 ,
                    .Defense_d = 10 ,
                    .speed = 10,
                    .attack = gladiator_attack
            },
    };

    Machine machines[2] = {
             {
                .sprite_sheet = "resourses/machine1.png",
                .image = "resourses/demon_idle_1.png",
                .current_frame = 0,
                .current_action = 0,
                .action_frames = {
                        .standing = 5,
                        .walking = 12,
                        .attacking1 = 15,
                        .hurting = 5,
                        .dying = 22
                },
                .x_position = 1900 - (12672/22),
                .y_position = 380,
                .sprite_width = (12672/22),
                .sprite_height = (1600/5),
                .health = 200,
                .attack_p = 15,
                .add_health = 0,
                .speed = 7,
                .attack = machine_attack
            },
             {
                .sprite_sheet = "resourses/Bringer-of-Death-SpritSheet.png",
                .image = "resourses/Bringer-of-Death_Idle_1.png",
                .current_frame = 0,
                .current_action = 0,
                .action_frames = {
                        .standing = 8,
                        .walking = 8,
                        .attacking1 = 8,
                        .hurting = 8,
                        .dying = 7
                },
                .x_position = 1900 - 420,
                .y_position = 430,
                .sprite_width = 420,
                .sprite_height = 280.4,
                .health = 200,
                .attack_p = 13,
                .add_health = 2,
                .speed = 9,
                .attack = machine_attack
            },

    };
    // Create the player selection UI
    GtkWidget *list_cards = gtk_image_new_from_file("resourses/list_card.png");
    gtk_fixed_put(GTK_FIXED(fixed), list_cards, 0, background_image_height / 10);


    GtkWidget *player1 = gtk_image_new_from_file(players[0].image);
    GtkWidget *player2 = gtk_image_new_from_file(players[1].image);
    GtkWidget *player3 = gtk_image_new_from_file(players[2].image);

    gtk_fixed_put(GTK_FIXED(fixed), player1, 140, background_image_height / 3);
    gtk_fixed_put(GTK_FIXED(fixed), player2, background_image_width / 6 + 140, background_image_height / 3 );
    gtk_fixed_put(GTK_FIXED(fixed), player3, background_image_width / 3 + 140, background_image_height / 3 );



    GtkWidget *card_player1 = gtk_image_new_from_file("resourses/Civilian_card.png");
    GtkWidget *card_player2 = gtk_image_new_from_file("resourses/Civilian_card.png");
    GtkWidget *card_player3 = gtk_image_new_from_file("resourses/Civilian_card.png");

    gtk_fixed_put(GTK_FIXED(fixed), card_player1, 100, background_image_height / 3 - 20);
    gtk_fixed_put(GTK_FIXED(fixed), card_player2, background_image_width / 6 + 100, background_image_height / 3 - 20);
    gtk_fixed_put(GTK_FIXED(fixed), card_player3, background_image_width / 3 + 100, background_image_height / 3 - 20);

    GtkWidget *player1_message = gtk_image_new_from_file("resourses/message.png");
    GtkWidget *player2_message = gtk_image_new_from_file("resourses/message.png");
    GtkWidget *player3_message = gtk_image_new_from_file("resourses/message.png");

    gtk_fixed_put(GTK_FIXED(fixed), player1_message, 80, background_image_height / 4);
    gtk_fixed_put(GTK_FIXED(fixed), player2_message, background_image_width / 6 + 60, background_image_height / 4);
    gtk_fixed_put(GTK_FIXED(fixed), player3_message, background_image_width / 3 + 50, background_image_height / 4);


    GtkWidget *button_select_player1 = create_button("select", "custom-button-player1");
    GtkWidget *button_select_player2 = create_button("select", "custom-button-player2");
    GtkWidget *button_select_player3 = create_button("select", "custom-button-player3");

    gtk_fixed_put(GTK_FIXED(fixed), button_select_player1, 150, background_image_height - 280);
    gtk_fixed_put(GTK_FIXED(fixed), button_select_player2, background_image_width / 6 + 150, background_image_height - 280);
    gtk_fixed_put(GTK_FIXED(fixed), button_select_player3, background_image_width / 3 + 150, background_image_height - 280);

    g_signal_connect(button_select_player1, "clicked", G_CALLBACK(on_select_button_clicked), &players[0]);
    g_signal_connect(button_select_player2, "clicked", G_CALLBACK(on_select_button_clicked), &players[1]);
    g_signal_connect(button_select_player3, "clicked", G_CALLBACK(on_select_button_clicked), &players[2]);



    //---------------------- selected payer and machine -------------------------

    GtkWidget *player_machine_table = gtk_image_new_from_file("resourses/bg.png");
    gtk_fixed_put(GTK_FIXED(fixed), player_machine_table, background_image_width /2 + 200 , 20);
    GtkWidget *vs = gtk_image_new_from_file("resourses/vs.png");
    gtk_fixed_put(GTK_FIXED(fixed), vs, background_image_width - 460 , 150);

    player_table = gtk_image_new_from_file(NULL);
    gtk_fixed_put(GTK_FIXED(fixed), player_table, background_image_width /2 + 250 ,50);

    machine_table = gtk_image_new_from_file(NULL);
    gtk_fixed_put(GTK_FIXED(fixed), machine_table, background_image_width - 300 ,50);


    GtkWidget *machine_label = create_label("Select the machine", "Ranger Eastwood 32", "white");
    gtk_fixed_put(GTK_FIXED(fixed), machine_label, background_image_width /1.5 + 50  ,400);

    // select machine buttons
    GtkWidget *button_select_regular_machine = create_button("regular machine ", "custom-button-player1");
    GtkWidget *button_select_smart_machine = create_button("smart machine", "custom-button-player2");

    gtk_fixed_put(GTK_FIXED(fixed), button_select_regular_machine,  background_image_width /1.5 - 50, 500);
    gtk_fixed_put(GTK_FIXED(fixed), button_select_smart_machine,  background_image_width /1.5 + 270, 500);

    g_signal_connect(button_select_regular_machine, "clicked", G_CALLBACK(on_select_machine_button_clicked), &machines[0]);
    g_signal_connect(button_select_smart_machine, "clicked", G_CALLBACK(on_select_machine_button_clicked), &machines[1]);
    //----------------------------------------------------------------------------


    GtkWidget *message_player1 = create_label("Our destiny \n awaits us", "DRAGON HUNTER 12", "black");
    GtkWidget *message_player2 = create_label("Courage will \n guide us.", "DRAGON HUNTER 12", "black");
    GtkWidget *message_player3 = create_label("Faith leads \n the way.", "DRAGON HUNTER 12", "black");

    gtk_fixed_put(GTK_FIXED(fixed), message_player1, 100, background_image_height / 4 + 50);
    gtk_fixed_put(GTK_FIXED(fixed), message_player2, background_image_width / 6 + 90, background_image_height / 4 + 50);
    gtk_fixed_put(GTK_FIXED(fixed), message_player3, background_image_width / 3 + 80, background_image_height / 4 + 50);

    GtkWidget *label_player1 = create_label("Nom: Samuri\nPV: 100\nAttaque: 50\nDefense: 30\nSpecial Capacity: ", "DRAGON HUNTER 12", "black");
    GtkWidget *label_player2 = create_label("Nom: Fire vizard\nPV: 120\nAttaque: 40\nDefense: 40\nSpecial Capacity: ", "DRAGON HUNTER 12", "black");
    GtkWidget *label_player3 = create_label("Nom: Knight\nPV: 80\nAttaque: 60\nDefense: 20\nSpecial Capacity: ", "DRAGON HUNTER 12", "black");

    gtk_fixed_put(GTK_FIXED(fixed), label_player1, 150, background_image_height / 3 + 270);
    gtk_fixed_put(GTK_FIXED(fixed), label_player2, background_image_width / 6 + 150, background_image_height / 3 + 270);
    gtk_fixed_put(GTK_FIXED(fixed), label_player3, background_image_width / 3 + 150, background_image_height / 3 + 270);

    button_start = create_button("Start", NULL);
    button_create = create_button("CREATE GLADIATOR", NULL);

    button_exit = create_button("EXIT", NULL);
    button_help = create_button("HELP", NULL);

    gtk_fixed_put(GTK_FIXED(fixed), button_start, background_image_width - 350, background_image_height - 400);
    gtk_fixed_put(GTK_FIXED(fixed), button_create,1000,19);
    gtk_fixed_put(GTK_FIXED(fixed), button_help, background_image_width - 350, background_image_height - 250);
    gtk_fixed_put(GTK_FIXED(fixed), button_exit, background_image_width - 350, background_image_height - 175);

    g_signal_connect(button_start, "clicked", G_CALLBACK(on_button_clicked), NULL);
    g_signal_connect(button_create, "clicked", G_CALLBACK(on_button_clicked), NULL);
    g_signal_connect(button_exit, "clicked", G_CALLBACK(on_button_clicked), NULL);
    g_signal_connect(button_help, "clicked", G_CALLBACK(on_button_clicked), NULL);

    GtkWidget *label = create_label("Select your Vector", "Ranger Eastwood 48", "black");
    gtk_fixed_put(GTK_FIXED(fixed), label, background_image_width / 7, background_image_height / 5);

    set_css_styles();
    gtk_widget_show_all(window);

    play_sound("resourses/GameMusic.wav");
    gtk_main();

    return 0;
}
