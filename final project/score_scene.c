#include "score_scene.h"
#include <allegro5/allegro.h>
#include "setting_scene.h"
#include "menu_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"
#include "lose_scene.h"
#include "loading_scene.h"


static Button backButton;
typedef struct Snowflake {
    float x, y;
    float speed;
} Snowflake;

#define MAX_SNOWFLAKES 100
static Snowflake snowflakes[MAX_SNOWFLAKES];
static void init_snowflakes(void) {
    for (int i = 0; i < MAX_SNOWFLAKES; i++) {
        snowflakes[i].x = rand() % SCREEN_W;
        snowflakes[i].y = rand() % SCREEN_H;
        snowflakes[i].speed = (rand() % 50) / 10.0f + 1.0f; // Speed range 1.0 to 6.0
    }
}
static void update_snowflakes(void) {
    for (int i = 0; i < MAX_SNOWFLAKES; i++) {
        snowflakes[i].y += snowflakes[i].speed;
        if (snowflakes[i].y > SCREEN_H) {
            snowflakes[i].y = 0;
            snowflakes[i].x = rand() % SCREEN_W;
        }
    }
}

static void init(void) {
    backButton = button_create(SCREEN_W / 2 - 200, 750, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    init_snowflakes();
    change_bgm("menu_bgm.ogg");
}
static void draw_snowflakes(void) {
    for (int i = 0; i < MAX_SNOWFLAKES; i++) {
        al_draw_filled_circle(snowflakes[i].x, snowflakes[i].y, 2, al_map_rgb(255, 255, 255));
    }
}
static void update(void) {

    update_button(&backButton);

    if (mouseState.buttons && backButton.hovered == true) {
        change_scene(create_menu_scene());
    }
    update_snowflakes();
}

static void draw(void) {
    draw_snowflakes();
    // button
    al_draw_text(
        WINLOSE_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        100,
        ALLEGRO_ALIGN_CENTER,
        "LEADERBOARD"
    );
    draw_button(backButton);
    //button text
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        750 + 28 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        750 + 31 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );
}

static void destroy(void) {
    destroy_button(&backButton);
}

Scene create_score_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "score";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}

