#include "win_scene.h"
#include <allegro5/allegro.h>
#include <string.h>
#include "menu_scene.h"
#include "loading_scene.h"
#include "game_scene.h"
#include "setting_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"

static int timer_countdown;
static ALLEGRO_BITMAP* player_bitmap;
static Button backButton;

static void init(void) {
    timer_countdown = 60;

    player_bitmap = al_load_bitmap("panda2.png");
    if (!player_bitmap) {
        game_abort("Failed to load player bitmap");
    }

    change_bgm("None");
    backButton = button_create(SCREEN_W / 2 - 200, 750, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
}

static void update(void) {
    update_button(&backButton);
    if (mouseState.buttons && backButton.hovered == true) {
        change_scene(create_menu_scene());
    }
}

static void draw(void) {

    al_draw_text(
        WINLOSE_FONT,
        al_map_rgb(146, 161, 185),
        SCREEN_W / 2,
        SCREEN_H / 2 - 50,
        ALLEGRO_ALIGN_CENTER,
        "You Win!"
    );

    ALLEGRO_BITMAP* pwin = al_load_bitmap("panda_win.png");
    int pwin_w = al_get_bitmap_width(pwin);
    int pwin_h = al_get_bitmap_height(pwin);
    al_draw_scaled_bitmap(pwin, 0, 0, pwin_w, pwin_h, 340, 130, 250, 250, 0);
    al_draw_scaled_bitmap(pwin, 0, 0, pwin_w, pwin_h, 1300, 80, 400, 400, 0);

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
}

static void destroy(void) {
    al_destroy_bitmap(player_bitmap);
    destroy_button(&backButton);
}


Scene create_win_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "win";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
