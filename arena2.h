

#include "gtk_utils.h" // Include the utility functions header
#include "arena.h"
// Global variables
gint64 last_machine_attack_time = 0;
gint64 last_gladiator_attack_time = 0; // Track the last attack time of the gladiator
gboolean is_game_over = FALSE;
GtkWidget *machine_health_label;
GtkWidget *gladiator_health_label;
Actor gladiator ;
Machine machine;
int smart_flag = 0;  // flag to indicate smart machine
gchar* selected_arena = "resourses/arena5.png";


gboolean is_paused = FALSE;

// Global variable for the label widget
GtkWidget *time_label;
int is_backward = 0 ;


void gladiator_attack(Actor *self, Machine *opponent) {
    gint64 current_time = g_get_real_time();
    if (current_time - last_gladiator_attack_time >= 1 * G_TIME_SPAN_SECOND) { // 1 second cooldown
        if (self->current_action == 3 || self->current_action == 4 || self->current_action == 5) {
            if (abs(self->x_position - opponent->x_position) + 200 < self->sprite_width) {
                opponent->health -= 10;
                opponent->current_action = 3; // Hurting action
                opponent->current_frame = 0;
                opponent->x_position -= 10; // Move the machine backward when hit
                if (opponent->health <= 0) {
                    opponent->current_action = 4; // Dying action
                }
            }
        }
        last_gladiator_attack_time = current_time; // Update the last attack time
    }
}
gboolean update_animation(gpointer data) {
    if (is_paused || is_game_over) {
        return TRUE;
    }

    // Ensure the gladiator stops all actions if dead
    if (gladiator.health <= 0) {
        gladiator.current_action = 9; // Dying action
        if (gladiator.current_frame < gladiator.action_frames.dying - 1) {
            gladiator.current_frame++;
        }
        // Stay on the last frame of the dying animation
        if (gladiator.current_frame >= gladiator.action_frames.dying - 1) {
            gladiator.current_frame = gladiator.action_frames.dying - 1;
        }
        // Update the action of the machine to standing when gladiator dies
        machine.current_action = 0; // Standing action
    } else {
        update_health_label(gladiator.health, gladiator_health_label);

        int frame_count = 0;
        switch (gladiator.current_action) {
            case 0: frame_count = gladiator.action_frames.standing; break;
            case 1: frame_count = gladiator.action_frames.walking; break;
            case 2: frame_count = gladiator.action_frames.running; break;
            case 3: frame_count = gladiator.action_frames.attacking1; break;
            case 4: frame_count = gladiator.action_frames.attacking2; break;
            case 5: frame_count = gladiator.action_frames.attacking3; break;
            case 6: frame_count = gladiator.action_frames.defending; break;
            case 7: frame_count = gladiator.action_frames.jumping; break;
            case 8: frame_count = gladiator.action_frames.hurting; break;
            case 9: frame_count = gladiator.action_frames.dying; break;
        }

        int x = (gladiator.current_frame % frame_count) * gladiator.sprite_width;
        int y = gladiator.current_action * gladiator.sprite_height;
        GdkPixbuf *frame = gdk_pixbuf_new_subpixbuf(gladiator.sprite_sheet, x, y, gladiator.sprite_width, gladiator.sprite_height);
        gtk_image_set_from_pixbuf(gladiator.image, frame);
        gtk_fixed_move(GTK_FIXED(data), GTK_WIDGET(gladiator.image), gladiator.x_position, gladiator.y_position);

        // Check if gladiator is dying
        if (gladiator.current_action == 9 && gladiator.current_frame == gladiator.action_frames.dying - 1) {
            is_game_over = TRUE; // Set game over state
        }
        gladiator.current_frame++;

        if (gladiator.current_frame >= frame_count) {
            gladiator.current_frame = 0;
        }

        // Handle gladiator movement
        if (!(gladiator.x_position >= 1900 - gladiator.sprite_width)) {
            if (gladiator.current_action == 1) {
                gladiator.x_position += 5;
            } else if (gladiator.current_action == 2) {
                gladiator.x_position += 10;
            } else if (gladiator.current_action == 7) {
                if (is_backward && gladiator.x_position >= 17) {
                    gladiator.x_position -= 10;
                } else {
                    gladiator.x_position += 7;
                }
            }
        }
    }

    return TRUE;
}

//------------------------------------------------------- reguler machine --------------------

// Function to handle machine attack
void machine_attack(Machine *self, Actor *opponent) {
    if (self->health <= 0) {
        self->current_action = 4; // Ensure the machine is in the dying action if health is zero
        return;
    }
    if (self->current_action == 2) {
        if (abs(self->x_position - opponent->x_position) < self->sprite_width + 100) {
            int damage = self->attack_p;
            if (opponent->current_action == 6) { // Check if gladiator is defending
                damage -= opponent->Defense_d; // Reduce damage if defending
            }
            opponent->health -= damage;
            if (opponent->health <= 0) {
                opponent->current_action = 9; // Dying action
            } else {
                opponent->current_action = 8; // Hurting action
            }
            opponent->current_frame = 0;
        }
    }

    // Smart machine feature: health regeneration if gladiator is not attacking
    if (smart_flag && opponent->current_action < gladiator.action_frames.attacking1) {
        self->health += self->add_health; // Regenerate health
        if (self->health > 200) {
            self->health = 200; // Cap health at 200
        }
    }
}

gboolean update_machine_animation(gpointer data) {
    if (is_paused || is_game_over) return TRUE;

    Machine *actor = &machine;
    Actor *opponent = &gladiator;

    update_health_label(actor->health, machine_health_label);

    // Ensure the machine stops all actions if dead
    if (actor->health <= 0) {
        actor->current_action = 4; // Dying action
        if (actor->current_frame < actor->action_frames.dying - 1) {
            actor->current_frame++;
        }
        // Stay on the last frame of the dying animation
        if (actor->current_frame >= actor->action_frames.dying - 1) {
            actor->current_frame = actor->action_frames.dying - 1;
        }
    } else {
        // Calculate distance between machine and gladiator
        int distance = abs(actor->x_position - opponent->x_position);

        // Limit machine attacks to once every 3 seconds
        gint64 current_time = g_get_real_time();
        if (current_time - last_machine_attack_time >= 3 * G_TIME_SPAN_SECOND) {
            // If gladiator is within attack range, perform attack
            if (distance + 500 < actor->sprite_width) {
                actor->current_action = 2; // Attack action
                actor->current_frame = (actor->current_frame + 1) % actor->action_frames.attacking1;
                actor->attack(actor, opponent);
                last_machine_attack_time = current_time; // Update last attack time
            } else {
                // If not in range, move towards the gladiator
                if (actor->x_position < opponent->x_position) {
                    actor->x_position += 10; // Move right
                    actor->current_action = 1; // Walking right
                } else if (actor->x_position + 150 > opponent->x_position) {
                    actor->x_position -= 5; // Move left
                    actor->current_action = 1; // Walking left
                }
            }
        }
    }

    // Update the animation frame
    int frame_count = 0;
    switch (actor->current_action) {
        case 0: frame_count = actor->action_frames.standing; break;
        case 1: frame_count = actor->action_frames.walking; break;
        case 2: frame_count = actor->action_frames.attacking1; break;
        case 3: frame_count = actor->action_frames.hurting; break;
        case 4: frame_count = actor->action_frames.dying; break;
        default: return TRUE; // No action needed for other actions
    }

    int x = (actor->current_frame % frame_count) * actor->sprite_width;
    int y = actor->current_action * actor->sprite_height;
    GdkPixbuf *frame = gdk_pixbuf_new_subpixbuf(actor->sprite_sheet, x, y, actor->sprite_width, actor->sprite_height);
    gtk_image_set_from_pixbuf(actor->image, frame);
    gtk_fixed_move(GTK_FIXED(data), GTK_WIDGET(actor->image), actor->x_position, actor->y_position);

    if (actor->health > 0) { // Only increment the frame if the machine is not dead
        actor->current_frame++;
        if (actor->current_frame >= frame_count) {
            actor->current_frame = 0;
            if (actor->current_action == 3 && actor->health <= 0) {
                actor->current_action = 4; // Change to dying action if health is zero
            }
        }
    }

    return TRUE;
}


gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    switch (event->keyval) {
        case GDK_KEY_s:
            gladiator.current_action = 0; // Standing
            break;
        case GDK_KEY_w:
            gladiator.current_action = 1; // Walking
            break;
        case GDK_KEY_Right:
            gladiator.current_action = 2; // Running forward
            break;
        case GDK_KEY_a:
            gladiator.current_action = 3; // Attacking 1
            gladiator.attack(&gladiator, &machine);
            break;
        case GDK_KEY_f:
            gladiator.current_action = 4; // Attacking 2
            gladiator.attack(&gladiator, &machine);
            break;
        case GDK_KEY_g:
            gladiator.current_action = 5; // Attacking 3
            gladiator.attack(&gladiator, &machine);
            break;
        case GDK_KEY_d:
            gladiator.current_action = 6; // Defending
            break;
        case GDK_KEY_Up:
            gladiator.current_action = 7; // Jumping
            is_backward = 0;
            break;
        case GDK_KEY_Left:
            gladiator.current_action = 7; // Jumping backward
            is_backward = 1;
            break;
        default:
            return FALSE; // No action matched, do not reset the frame counter
    }

    // Reset the frame counter
    gladiator.current_frame = 0;

    // Return TRUE to keep the timeout running
    return TRUE;
}


// Function to update the health label
void update_health_label(int health, GtkWidget *label) {
    if(health < 0 ) health = 0 ;
    gchar *health_str = g_strdup_printf("%d", health);
    gtk_label_set_text(GTK_LABEL(label), health_str);
    g_free(health_str);
}

// Function to update the time label
gboolean update_time_label(gpointer user_data) {
    if (is_paused || is_game_over ) {
        return TRUE;
    }
    static int seconds = 0;

    // Calculate minutes and seconds
    int minutes = seconds / 60;
    int display_seconds = seconds % 60;

    // Create the time string
    char time_str[6];
    snprintf(time_str, sizeof(time_str), "%02d:%02d", minutes, display_seconds);

    // Update the label text
    gtk_label_set_text(GTK_LABEL(time_label), time_str);

    // Increment the seconds
    seconds++;

    // Return TRUE to keep the timeout running
    return TRUE;
}


gboolean reset_action(gpointer data);
void toggle_pause(GtkWidget *widget, gpointer data) {
    is_paused = !is_paused;

    if (is_paused) {
        gtk_button_set_label(GTK_BUTTON(widget), "Play");
    } else {
        gtk_button_set_label(GTK_BUTTON(widget), "Pause");
    }
}



void run_game(Actor *player , Machine *selected_machine , int is_smart ) {
    // Initialize GTK
    gtk_init(NULL, NULL);


    gladiator = *player;
    machine = *selected_machine ;
    // Load the sprite sheet image

printf(" \nsprite path %s",player->sprite_sheet);
    gladiator.sprite_sheet = gdk_pixbuf_new_from_file(player->sprite_sheet, NULL);

    machine.sprite_sheet = gdk_pixbuf_new_from_file(machine.sprite_sheet, NULL);


    // Create the main window
    GdkPixbuf *arena_pixbuf = gdk_pixbuf_new_from_file(selected_arena, NULL);
    gint arena_width = gdk_pixbuf_get_width(arena_pixbuf);
    gint arena_height = gdk_pixbuf_get_height(arena_pixbuf);
    GtkWidget *window = create_window("Sprite Animation", arena_width, arena_height);

    // Connect key press event to the window
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);

    // Create a fixed container to hold the background image and gladiator image
    GtkWidget *fixed = create_fixed();

    // Update the background image
    GtkWidget *arena_image = NULL;
    update_background_image(fixed, &arena_image, selected_arena);

    // Create an image widget to display the sprite animation
    gladiator.image = GTK_IMAGE(gtk_image_new());
    gtk_widget_set_size_request(GTK_WIDGET(gladiator.image), gladiator.sprite_width, gladiator.sprite_height);
    gtk_fixed_put(GTK_FIXED(fixed), GTK_WIDGET(gladiator.image),gladiator.x_position, 10);

    // Create an image widget to display the sprite animation
    machine.image = GTK_IMAGE(gtk_image_new());
    gtk_widget_set_size_request(GTK_WIDGET(machine.image), machine.sprite_width, machine.sprite_height);
    gtk_fixed_put(GTK_FIXED(fixed), GTK_WIDGET(machine.image), machine.x_position, 0);



    // Create image widgets for player 1 and player 2 life
  GtkWidget *    life_image_player1 = gtk_image_new_from_file("resourses/LifeBar.png");
   GtkWidget *  life_image_player2 = gtk_image_new_from_file("resourses/LifeBar.png");


GtkWidget *    time_image = gtk_image_new_from_file("resourses/clock.png");
    GtkWidget *table_time_image = gtk_image_new_from_file("resourses/table.png");


    // Add the life images to the fixed container
    gtk_fixed_put(GTK_FIXED(fixed), life_image_player1, 0, 0); // Adjust position as needed
    gtk_fixed_put(GTK_FIXED(fixed), life_image_player2, arena_width - 375, 0);
    gtk_fixed_put(GTK_FIXED(fixed), table_time_image, arena_width / 2 - 160, 20);
    gtk_fixed_put(GTK_FIXED(fixed), time_image, arena_width / 2 - 200, 0);

    // Create labels to display the health of both players
    gladiator_health_label = create_label("100", "Ranger Eastwood 24", "white");
    machine_health_label = create_label("100", "Ranger Eastwood 24", "white");

    gtk_fixed_put(GTK_FIXED(fixed), gladiator_health_label, 250, 30); // Adjust position as needed
    gtk_fixed_put(GTK_FIXED(fixed), machine_health_label, arena_width - 120, 30); // Adjust position as needed

    // Create a label for the text
    time_label = create_label("00:00", "Ranger Eastwood 24", "white");
    gtk_fixed_put(GTK_FIXED(fixed), time_label, arena_width / 2 - 90, 35);

    // Add the pause button
    GtkWidget *pause_button = create_button("Pause", "pause_button");
    gtk_fixed_put(GTK_FIXED(fixed), pause_button, arena_width / 3 + 20, 20);


    g_signal_connect(pause_button, "clicked", G_CALLBACK(toggle_pause), NULL);
    // Add the fixed container to the window
    gtk_container_add(GTK_CONTAINER(window), fixed);
 gtk_widget_show_all(window);

    // Start the animation
    g_timeout_add(100, update_animation, fixed);
    g_timeout_add(100, update_machine_animation, fixed);


    // Start the time label update every second
    g_timeout_add_seconds(1, update_time_label, NULL);

    play_sound("resourses/GameMusic.wav");  // Show the window


    // Start the GTK main loop
    gtk_main();

}


//gboolean check_game_over(gpointer data) {
//    if (is_game_over) {
//        if (gladiator.health <= 0) {
//            gtk_label_set_text(GTK_LABEL(data), "Machine Wins!");
//        } else if (machine.health <= 0) {
//            gtk_label_set_text(GTK_LABEL(data), "Gladiator Wins!");
//        }
//    }
//
//    return TRUE;
//}
