#define _USE_MATH_DEFINES

#include "shop_scene.h"
#include <allegro5/allegro.h>
#include <string.h>
#include "setting_scene.h"
#include "menu_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"
#include "game_scene.h"
#include "map.h"
#include "weapon.h"
#include <math.h>

static Button backButton;
static Button buysniperButton;
static Button buyhealthButton;
int healthpotion = 0;
int buyweapon = 0;
int buypotion = 0;
static int isFirstRun = 1;
int total_coin;

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
    buysniperButton = button_create(SCREEN_W / 2 - 300, 550, 200, 100, "UI_Button.png", "UI_Button_hovered.png");
    buyhealthButton = button_create(SCREEN_W / 2 + 100, 550, 200, 100, "UI_Button.png", "UI_Button_hovered.png");
    if(isFirstRun){
        total_coin = 25 + coins_obtained;
        isFirstRun = 0;
    }else{
        total_coin += coins_obtained;
    }
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
    update_button(&buysniperButton);
    update_button(&buyhealthButton);
    update_snowflakes();

    if (mouseState.buttons && backButton.hovered == true) {
        change_scene(create_menu_scene());
    }
    if (mouseState.buttons && buysniperButton.hovered == true) {
        if (total_coin >= 10){
            total_coin-=10;
            buyweapon = 2;
            printf("Weapon purchased! Total coins: %d\n", total_coin);
        } else {
            printf("Not enough coins to buy the weapon!\n");
        }
    }
    if (mouseState.buttons && buyhealthButton.hovered == true) {
        if (total_coin >= 15){
            total_coin -= 15;
            buypotion = 1;
            healthpotion += 1;
            printf("Health potion purchased! Total coins: %d\n", total_coin);
        } else {
            printf("Not enough coins to buy the weapon!\n");
        }
    }
}

static void draw(void) {
    draw_snowflakes();
    // button
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
    al_draw_text(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        255,
        120,
        ALLEGRO_ALIGN_CENTER,
        "HAPPY SHOPPING!"
    );
    draw_button(buysniperButton);
    //button text
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W/2 - 200,
        550 + 28 + buysniperButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BUY"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W/2 - 200,
        550 + 31 + buysniperButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BUY"
    );
    draw_button(buyhealthButton);
    //button text
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W/2 + 200,
        550 + 28 + buyhealthButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BUY"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W/2 + 200,
        550 + 31 + buyhealthButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BUY"
    );
    //coin
    ALLEGRO_BITMAP* coin_UI;
    coin_UI = al_load_bitmap("coin_icon.png");
    float desired_height = 24.0f;
    float coin_scale = 4 * (desired_height / al_get_bitmap_height(coin_UI));
    float coin_y_position = 80 + 70 + 20;

    al_draw_scaled_bitmap(
        coin_UI,
        0, 0,
        al_get_bitmap_width(coin_UI), al_get_bitmap_height(coin_UI),
        80, coin_y_position,
        al_get_bitmap_width(coin_UI) * coin_scale, al_get_bitmap_height(coin_UI) * coin_scale,
        0
    );

    // Position the coin text next to the coin icon
    float coin_text_x_position = 80 + (al_get_bitmap_width(coin_UI) * coin_scale) + 10;
    al_draw_textf(
        P1_FONT,
        al_map_rgb(255, 255, 255),
        coin_text_x_position, coin_y_position + (al_get_bitmap_height(coin_UI) * coin_scale / 2) - 12,
        ALLEGRO_ALIGN_LEFT,
        "%0.2d", total_coin
    );
    
    //sniper
    ALLEGRO_BITMAP* sniper = al_load_bitmap("sniper.png");
    int sniper_w = al_get_bitmap_width(sniper);
    int sniper_h = al_get_bitmap_height(sniper);
    al_draw_scaled_bitmap(sniper, 0, 0, sniper_w, sniper_h, (SCREEN_W/2)-300, 275, 300, 300, 0);
    //price sniper
    al_draw_scaled_bitmap(
        coin_UI,
        0, 0,
        al_get_bitmap_width(coin_UI), al_get_bitmap_height(coin_UI),
        (SCREEN_W/2)-275, 650,
        al_get_bitmap_width(coin_UI) * coin_scale, al_get_bitmap_height(coin_UI) * coin_scale,
        0
    );

    // Position the coin text next to the coin icon
    al_draw_textf(
        P1_FONT,
        al_map_rgb(255, 255, 255),
        (SCREEN_W/2)-200, 650 + (al_get_bitmap_height(coin_UI) * coin_scale / 2) - 12,
        ALLEGRO_ALIGN_LEFT,
        " %0.2d", 10
    );
    //healthpotion
    ALLEGRO_BITMAP* health= al_load_bitmap("healthpotion.png");
    int health_w = al_get_bitmap_width(health);
    int health_h = al_get_bitmap_height(health);
    al_draw_scaled_bitmap(health, 0, 0, health_w, health_h, (SCREEN_W/2)+50, 275, 300, 300, 0);
    //price healthpotion
    al_draw_scaled_bitmap(
        coin_UI,
        0, 0,
        al_get_bitmap_width(coin_UI), al_get_bitmap_height(coin_UI),
        (SCREEN_W/2)+100, 650,
        al_get_bitmap_width(coin_UI) * coin_scale, al_get_bitmap_height(coin_UI) * coin_scale,
        0
    );

    // Position the coin text next to the coin icon
    al_draw_textf(
        P1_FONT,
        al_map_rgb(255, 255, 255),
        (SCREEN_W/2)+175, 650 + (al_get_bitmap_height(coin_UI) * coin_scale / 2) - 12,
        ALLEGRO_ALIGN_LEFT,
        " %0.2d", 15
    );
}

static void destroy(void) {
    destroy_button(&backButton);
    destroy_button(&buysniperButton);
    destroy_button(&buyhealthButton);
}

Scene create_shop_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "shop";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}

