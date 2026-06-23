#include <string.h>
#include "lose_scene.h"
#include "win_scene.h"
#include "game_scene.h"
#include "menu_scene.h"
#include "loading_scene.h"
#include "shop_scene.h"
#include "score_scene.h"
#include "game.h"
#include "player.h"
#include "map.h"
#include "enemy.h"
#include "weapon.h"
#include "UI.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <math.h>

Player player; // Player
Map map; // Map
enemyNode * enemyList; // Enemy List
BulletNode * bulletList; // Bullet List


// Weapon
Weapon weapon; 
int coins_obtained;

ALLEGRO_FONT *font;
ALLEGRO_BITMAP* health;
ALLEGRO_BITMAP* coin_UI;
static void init(void){
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    health = al_load_bitmap("heart.png");
    coin_UI = al_load_bitmap("coin_icon.png");
    initEnemy();
    
    map = create_map("map0.txt", 0);

    player = create_player("panda2.png", map.Spawn.x, map.Spawn.y);

    enemyList = createEnemyList();
    bulletList = createBulletList();

    weapon = create_weapon("guns.png", "yellow_bullet.png", 16, 8, 10);
    
    for(int i=0; i<map.EnemySpawnSize; i++){
        Enemy enemy = createEnemy(map.EnemySpawn[i].x, map.EnemySpawn[i].y, map.EnemyCode[i]);
        insertEnemyList(enemyList, enemy);
    }

    game_log("coord x:%d \n coords y:%d \n", map.Spawn.x, map.Spawn.y);
    change_bgm("game_bgm.ogg");
}
int countdown = 64;

static void update(void){
    if(keyState[ALLEGRO_KEY_1]){
        delete_weapon(&weapon);
        weapon = create_weapon("guns.png", "yellow_bullet.png", 16, 16, 10);
    }
    
    if (keyState[ALLEGRO_KEY_2] && buyweapon == 2){
        delete_weapon(&weapon);
        weapon = create_weapon("sniper.png", "yellow_bullet.png", 8, 30, 10);
        printf("Equipped sniper weapon!\n");
    }
    
    if (keyState[ALLEGRO_KEY_3] && buypotion == 1 &&player.health < 100 && healthpotion>0){
        player.health += 20;
        if(player.health > 100){
            player.health = 100;
        }
        healthpotion--;
        printf("Health potion used! Total potion = %d\n", healthpotion);
    }
    
    if(player.status == DEAD){
        countdown--;
        if(countdown == 0){
            change_scene(create_lose_scene());
            return;
        }
    }
    
    int totalenemy=2;
    enemyNode *cur = enemyList;
    while(cur!=NULL){
        if(cur->enemy.status==DEAD){
            totalenemy--;
            printf("total enemy = %d\n", totalenemy);
        }
        cur = cur->next;
    }
    if(totalenemy==0){
        change_scene(create_win_scene());
        return;
    }
    update_player(&player, &map);
    /*
        [TODO Homework] DONE
        
        Change the scene if winning/losing to win/lose scene
    */

    Point Camera;
    /*
        [TODO HACKATHON 1-3]
        
        Calculate the formula for the Camera
        Camera.x = ...
        Camera.y = ...

        Hint: Adjust it based on player position variable, then subtract it with half of the gameplay screen
    */
    Camera = (Point){0,0};
    //player position relative to the game world or map, not to the screen, so u must subtract it with screen
    Camera.x = player.coord.x - (SCREEN_W/2);
    Camera.y = player.coord.y - (SCREEN_H/2);
    updateEnemyList(enemyList, &map, &player);
    update_weapon(&weapon, bulletList, player.coord, Camera);
    updateBulletList(bulletList, enemyList, &map);
    update_map(&map, player.coord, &coins_obtained);
}

static void draw(void){
    Point Camera;
    /*
        [TODO HACKATHON 1-4] just copy from hackathon 1-3
        Calculate the formula for the Camera
    */
    Camera = (Point){0,0};
    Camera.x = player.coord.x - (SCREEN_W/2);
    Camera.y = player.coord.y - (SCREEN_H/2);
    // Draw
    draw_map(&map, Camera);
    drawEnemyList(enemyList, Camera);
    drawBulletList(bulletList, Camera);
    draw_player(&player, Camera);
    draw_weapon(&weapon, player.coord, Camera);
    
    int heartamount = player.health/20;
    for(int i = 0; i < heartamount; i++){
        al_draw_scaled_bitmap(health, 0, 0, 32, 32, 80 + (i * 70) + 5, 80, 70, 70, 0);
    }
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
        "%0.2d", coins_obtained
    );
    /*
        [TODO Homework] DONE
        Draw the UI of Health and Total Coins
    */
}

static void destroy(void){
    delete_player(&player);
    delete_weapon(&weapon);
    destroy_map(&map);
    destroyBulletList(bulletList);
    destroyEnemyList(enemyList);
    terminateEnemy();
}

Scene create_game_scene(void){
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    
    scene.name = "game";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;
    
    return scene;
}
