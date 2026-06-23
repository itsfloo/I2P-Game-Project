#include <allegro5/allegro.h>
#include <string.h>
#include "menu_scene.h"
#include "game_scene.h"
#include "setting_scene.h"
#include "loading_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"
#include "shop_scene.h"
#include "score_scene.h"

static Button settingButton;
static Button shopButton;
static Button scoreButton;
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
static void draw_snowflakes(void) {
    for (int i = 0; i < MAX_SNOWFLAKES; i++) {
        al_draw_filled_circle(snowflakes[i].x, snowflakes[i].y, 2, al_map_rgb(255, 255, 255));
    }
}
static void init(void) {
    settingButton = button_create(SCREEN_W / 2 - 200, 600, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    shopButton = button_create(SCREEN_W / 2 - 200, 730, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    scoreButton = button_create(SCREEN_W / 2 - 200, 860, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    init_snowflakes();
    change_bgm("menu_bgm.ogg");
}

static void update(void) {
    update_button(&settingButton);
    update_button(&shopButton);
    update_button(&scoreButton);
    update_snowflakes();
    if (keyState[ALLEGRO_KEY_ENTER]) {
        change_scene(create_loading_scene());
    }
    if (settingButton.hovered && mouseState.buttons&1){
        change_scene(create_setting_scene());
    }
    if (shopButton.hovered && mouseState.buttons&1){
        change_scene(create_shop_scene());
    }
    if (scoreButton.hovered && mouseState.buttons&1){
        change_scene(create_score_scene());
    }
    
    /*
        [TODO HACKATHON 4-3] 
        
        Change scene to setting scene when the button is pressed
    */
}

static void draw(void) {
    draw_snowflakes();
    // Title Text
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(146, 161, 185),
        SCREEN_W / 2,
        275,
        ALLEGRO_ALIGN_CENTER,
        "NTHU-RPG ADVENTURE"
    );
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(199, 207, 221),
        SCREEN_W / 2,
        270,
        ALLEGRO_ALIGN_CENTER,
        "NTHU-RPG ADVENTURE"
    );

    al_draw_text(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        SCREEN_W / 2,
        550,
        ALLEGRO_ALIGN_CENTER,
        "PRESS [ENTER] TO PLAY"
    );

    // button
    draw_button(settingButton);
    // button text
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        600 + 28 + settingButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "SETTING"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        600 + 31 + settingButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "SETTING"
    );
    //draw playerbutton
    draw_button(shopButton);
        al_draw_text(
            P2_FONT,
            al_map_rgb(66, 76, 110),
            SCREEN_W / 2,
            730 + 28 + shopButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "SHOP"
        );
        al_draw_text(
            P2_FONT,
            al_map_rgb(225, 225, 225),
            SCREEN_W / 2,
            730 + 31 + shopButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "SHOP"
        );
    //draw scorebutton
    draw_button(scoreButton);
        al_draw_text(
            P2_FONT,
            al_map_rgb(66, 76, 110),
            SCREEN_W / 2,
            860 + 28 + scoreButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "SCORE"
        );
        al_draw_text(
            P2_FONT,
            al_map_rgb(225, 225, 225),
            SCREEN_W / 2,
            860 + 31 + scoreButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "SCORE"
        );

    ALLEGRO_BITMAP* nthu = al_load_bitmap("nthu.png");
    int nthu_w = al_get_bitmap_width(nthu);
    int nthu_h = al_get_bitmap_height(nthu);
    ALLEGRO_BITMAP* cover = al_load_bitmap("cover.png");
    int cover_w = al_get_bitmap_width(cover);
    int cover_h = al_get_bitmap_height(cover);
    al_draw_scaled_bitmap(cover, 0, 0, cover_w, cover_h, 340, 130, 250, 250, 0);
    al_draw_scaled_bitmap(nthu, 0, 0, nthu_w, nthu_h, 1300, 80, 400, 400, 0);
    ALLEGRO_BITMAP* panda = al_load_bitmap("panda_win.png");
    int panda_w = al_get_bitmap_width(panda);
    int panda_h = al_get_bitmap_height(panda);
    al_draw_scaled_bitmap(panda, 0, 0, panda_w, panda_h, (SCREEN_W/2)-150, 300, 250, 250, 0);
}

static void destroy(void) {
    destroy_button(&settingButton);
    destroy_button(&shopButton);
    destroy_button(&scoreButton);
}


Scene create_menu_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "menu";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
