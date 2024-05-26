

#ifndef TEST_GTK_PLAYER_H
#define TEST_GTK_PLAYER_H


typedef struct {
    int standing;
    int walking;
    int running;
    int attacking1;
    int attacking2;
    int attacking3;
    int defending;
    int jumping;
    int hurting;
    int dying;
} ActionFrames;

typedef struct Actor{
    const char *name;
    GdkPixbuf *sprite_sheet;
    GtkImage *image;
    int current_frame;
    int current_action;
    ActionFrames action_frames;
    int x_position;
    int y_position;
    double sprite_width;
    double sprite_height;
    int health;
    int Attack_P;
    int Defense_d;
    int speed ;
    void (*attack)(struct Actor *self, struct Machine *opponent);
} Actor;


typedef struct Machine{
    GdkPixbuf *sprite_sheet;
    GtkImage *image;
    int current_frame;
    int current_action;
    struct {
        int standing;
        int walking;
        int attacking1;
        int hurting;
        int dying;
    } action_frames;
    int x_position;
    int y_position;
    double sprite_width;
    double sprite_height;
    int health;
    int attack_p ;
    int add_health ;
    int speed ;
    void (*attack)(struct Machine *self, struct Actor *opponent);
} Machine;


#endif //TEST_GTK_PLAYER_H
