#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "utility.h"

const int FPS = 60; 
const int SCREEN_W = 1920;
const int SCREEN_H = 1080;
const int RESERVE_SAMPLES = 10;
const char* GAME_TITLE = "NTHU-RPG Adventure";
const char* log_file = "log.txt";
const char* font_file = "Minecraft.ttf";
const int TILE_SIZE = 64;

char player_name[30] = {0};
int name = 0;

ALLEGRO_SAMPLE_INSTANCE* BGM_INSTANCE = NULL;
float SFX_VOLUME = 0.5f;
float BGM_VOLUME = 0.5f;

ALLEGRO_FONT* TITLE_FONT;
ALLEGRO_FONT* WINLOSE_FONT;
ALLEGRO_FONT* P1_FONT;
ALLEGRO_FONT* P2_FONT;
ALLEGRO_FONT* P3_FONT;
ALLEGRO_SAMPLE* BGM = NULL;

void change_bgm(char* audio_path) {
    if (BGM_INSTANCE != NULL) {
        al_destroy_sample_instance(BGM_INSTANCE);
        BGM_INSTANCE = NULL;
    }

    if (BGM != NULL) {
        al_destroy_sample(BGM);
        BGM = NULL;
    }

    BGM = al_load_sample(audio_path);
    if (!BGM) {
        game_log("No BGM File found [%s], no sound will be played", audio_path);
        return;
    }

    BGM_INSTANCE = al_create_sample_instance(BGM);
    al_attach_sample_instance_to_mixer(BGM_INSTANCE, al_get_default_mixer());

    // Reapply the saved BGM volume
    al_set_sample_instance_gain(BGM_INSTANCE, BGM_VOLUME);

    al_play_sample_instance(BGM_INSTANCE);
}

void init_Util(void){
    TITLE_FONT = al_load_font("Minecraft.ttf", 64, 0);
    if(!TITLE_FONT){
        game_abort("Failed to load Title Font");
    }
    WINLOSE_FONT = al_load_font("Minecraft.ttf", 150, 0);
    if(!WINLOSE_FONT){
        game_abort("Failed to load Title Font");
    }
    P1_FONT = al_load_font(font_file, 48, 0);
    if(!P1_FONT){
        game_abort("Failed to load P1 Font");
    }
    
    P2_FONT = al_load_font(font_file, 36, 0);
    if(!P2_FONT){
        game_abort("Failed to load P2 Font");
    }
    
    P3_FONT = al_load_font(font_file, 24, 0);
    if(!P3_FONT){
        game_abort("Failed to load P3 Font");
    }
}

void destroy_init(void) {
    al_destroy_font(TITLE_FONT);
    al_destroy_font(WINLOSE_FONT);
    al_destroy_font(P1_FONT);
    al_destroy_font(P2_FONT);
    al_destroy_font(P3_FONT);
}

/*
    [GAME LOG]

    You can disable the log by remove the "#define LOG_ENABLE" in utility.h
    It's not recommended if you are still in progress on making this game
    You may do it after finishing it for performance boost purpose
*/
void print_log(const char * msg, va_list arg, logtype type){
#ifdef LOG_ENABLE
    // Open File
    static bool firstOpen = true;
    FILE* f;

    if (firstOpen) {
        f = fopen(log_file, "w");
        firstOpen = false;
    }
    else {
        f = fopen(log_file, "a");
    }
    
    switch(type){
        case Log:
            printf("[LOG] ");
            if(f != NULL)
                fprintf(f, "[LOG] ");
            break;
        case Error:
            printf("[Error] ");
            if(f != NULL)
                fprintf(f, "[Error] ");
            break;
        case Warning:
            printf("[Warning] ");
            if(f != NULL)
                fprintf(f, "[Warning] ");
        default:
            printf("[???] ");
            if(f != NULL)
                fprintf(f, "[???] ");
            break;
    }
    
    vprintf(msg, arg);
    printf("\n");
    if(f != NULL){
        vfprintf(f, msg, arg);
        fprintf(f, "\n");
        fclose(f);
    }

#endif
}

void game_log(const char * msg, ...){
#ifdef LOG_ENABLE
    va_list args;
    va_start(args, msg);
    print_log(msg, args, Log);
    va_end(args);
#endif
}

void game_abort(const char * msg, ...){
#ifdef LOG_ENABLE
    va_list args;
    va_start(args, msg);
    print_log(msg, args, Error);
    va_end(args);
#endif
    // Abort the game if there is an error
    exit(1);
}

// To print with a Warning Sign
void game_warning(const char * msg, ...){
#ifdef LOG_ENABLE
    va_list args;
    va_start(args, msg);
    print_log(msg, args, Warning);
    va_end(args);
#endif
}
static void init(void){
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
}
