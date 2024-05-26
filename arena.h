
#ifndef TEST_GTK_ARENA_H
#define TEST_GTK_ARENA_H
#include <gtk/gtk.h>
#include "player.h"
#include "arena2.h"


extern GtkWidget *life_image_player1;
extern GtkWidget *life_image_player2;
extern GtkWidget *time_image;
extern GtkWidget *table_time_image;




extern Actor gladiator;
extern Machine machine;

extern gchar* selected_arena;

gboolean update_animation(gpointer data);
gboolean update_machine_animation(gpointer data);
gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data);
void run_game(Actor *player , Machine *machine , int isSmart );
void gladiator_attack(Actor *self, Machine *opponent);
void machine_attack(Machine *self, Actor *opponent);
void update_health_label(int health ,GtkWidget *health_label);



#endif //TEST_GTK_ARENA_H
