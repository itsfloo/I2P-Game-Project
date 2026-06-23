#include <allegro5/allegro.h>
#include "setting_scene.h"
#include "menu_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"

static Button backButton;
static ALLEGRO_BITMAP* sliderImage;
static ALLEGRO_BITMAP* sliderHoveredImage;
static RecArea bgmSlider;
static RecArea sfxSlider;
static float bgmSliderPos;
static float sfxSliderPos;

static void init(void) {
    backButton = button_create(SCREEN_W / 2 - 200, 750, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    sliderImage = al_load_bitmap("slider.png");
    sliderHoveredImage = al_load_bitmap("slider_hovered.png");
    if (!sliderImage || !sliderHoveredImage) {
        game_abort("Failed to load slider images");
    }

    bgmSlider = (RecArea){ SCREEN_W / 2 - 300, 300, 500, 30 };
    sfxSlider = (RecArea){ SCREEN_W / 2 - 300, 500, 500, 30 };

    // Initialize slider positions based on current volume levels
    bgmSliderPos = bgmSlider.x + (BGM_VOLUME * bgmSlider.w);
    sfxSliderPos = sfxSlider.x + (SFX_VOLUME * sfxSlider.w);
}

static void update(void) {

    update_button(&backButton);

    if (mouseState.buttons && backButton.hovered == true) {
        change_scene(create_menu_scene());
    }
    
    if (mouseState.buttons) {
        Point mouse = { mouseState.x, mouseState.y };

        if (mouse.x > bgmSlider.x && mouse.x < bgmSlider.x + bgmSlider.w &&
            mouse.y > bgmSlider.y && mouse.y < bgmSlider.y + bgmSlider.h) {
            bgmSliderPos = mouse.x;
            BGM_VOLUME = (bgmSliderPos - bgmSlider.x) / bgmSlider.w;
            al_set_sample_instance_gain(BGM_INSTANCE, BGM_VOLUME);

        }

        if (mouse.x > sfxSlider.x && mouse.x < sfxSlider.x + sfxSlider.w &&
            mouse.y > sfxSlider.y && mouse.y < sfxSlider.y + sfxSlider.h) {
            sfxSliderPos = mouse.x;
            SFX_VOLUME = (sfxSliderPos - sfxSlider.x) / sfxSlider.w;
        }
    }
}

static void draw(void) {
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
    // Draw BGM slider
    al_draw_scaled_bitmap(
        sliderImage,
        0, 0,
        al_get_bitmap_width(sliderImage),
        al_get_bitmap_height(sliderImage),
        bgmSlider.x, bgmSlider.y,
        bgmSlider.w, bgmSlider.h,
        0
    );
    al_draw_scaled_bitmap(
        sliderHoveredImage,
        0, 0,
        al_get_bitmap_width(sliderHoveredImage),
        al_get_bitmap_height(sliderHoveredImage),
        bgmSliderPos - 10, bgmSlider.y - 5,
        20, 30,
        0
    );
    al_draw_text(P2_FONT, al_map_rgb(255, 255, 255), SCREEN_W / 2, bgmSlider.y - 60, ALLEGRO_ALIGN_CENTER, "BGM Volume");

    // Draw SFX slider
    al_draw_scaled_bitmap(
        sliderImage,
        0, 0,
        al_get_bitmap_width(sliderImage),
        al_get_bitmap_height(sliderImage),
        sfxSlider.x, sfxSlider.y,
        sfxSlider.w, sfxSlider.h,
        0
    );
    al_draw_scaled_bitmap(
        sliderHoveredImage,
        0, 0,
        al_get_bitmap_width(sliderHoveredImage),
        al_get_bitmap_height(sliderHoveredImage),
        sfxSliderPos - 10, sfxSlider.y - 5,
        20, 30,
        0
    );
    al_draw_text(P2_FONT, al_map_rgb(255, 255, 255), SCREEN_W / 2, sfxSlider.y - 60, ALLEGRO_ALIGN_CENTER, "SFX Volume");
}

static void destroy(void) {
    destroy_button(&backButton);
    al_destroy_bitmap(sliderImage);
    al_destroy_bitmap(sliderHoveredImage);
}

Scene create_setting_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "setting";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}

