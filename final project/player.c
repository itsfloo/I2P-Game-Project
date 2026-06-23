#include "player.h"
#include "game.h"
#include "utility.h"
#include "map.h"

#include <math.h>

static bool isCollision(Player* player, Map* map);

Player create_player(char * path, int row, int col){
    Player player;
    memset(&player, 0, sizeof(player));
    
    player.coord = (Point){
        col * TILE_SIZE,
        row * TILE_SIZE
    };
    
    player.speed = 4;
    player.health = 100;
    player.animation_tick=0;
    player.dying_tick=0;
    player.status=PLAYER_IDLE;
    
    player.image = al_load_bitmap(path);
    if(!player.image){
        game_abort("Error Load Bitmap with path : %s", path);
    }
    
    return player;
}

void update_player(Player * player, Map* map){

    Point original = player->coord;
    
    // Knockback effect
    if(player->knockback_CD > 0){

        player->knockback_CD--;
        int next_x = player->coord.x + player->speed * cos(player->knockback_angle);
        int next_y = player->coord.y + player->speed * sin(player->knockback_angle);
        Point next;
        next = (Point){next_x, player->coord.y};
        
        if(!isCollision(player, map)){
            player->coord = next;
        }
        
        next = (Point){player->coord.x, next_y};
        if(!isCollision(player, map)){
            player->coord = next;
        }
    }
    if (keyState[ALLEGRO_KEY_W]){ //if the 'W' key is pressed
        player->coord.y -= player->speed; // subtracting the player->speed with player y-coordinate
        player->direction = UP; //maju
        player->status = PLAYER_WALKING;
    }
    if (keyState[ALLEGRO_KEY_S]){
        player->coord.y += player-> speed; //adding player speed with player y-coordinate
        player->direction = DOWN; //mundur
        player->status = PLAYER_WALKING;
    }
    /*
        [TODO HACKATHON 1-1] 
        
        Player Movement
        Adjust the movement by player->speed

        if (keyState[ALLEGRO_KEY_W]) {
            player->coord.y = ...
            player->direction = ...
        }
        if (keyState[ALLEGRO_KEY_S]) {
            player->coord.y = ...
            player->direction = ...
        }
    */
    
    // if Collide, snap to the grid to make it pixel perfect
    if(isCollision(player, map)){
        player->coord.y = round((float)original.y / (float)TILE_SIZE) * TILE_SIZE;
    }
    if (keyState[ALLEGRO_KEY_A]){
        player->coord.x -= player->speed; //value x jadi min, means move left
        player->direction = LEFT;
        player->status= PLAYER_WALKING;
    }
    if (keyState[ALLEGRO_KEY_D]){
        player->coord.x += player->speed; //value x become positive, means move right
        player->direction = RIGHT;
        player->status = PLAYER_WALKING;
    }
    /*
        [TODO HACKATHON 1-2] 
        
        Player Movement
        Add/Subtract the movement by player->speed

        if (keyState[ALLEGRO_KEY_A]) {
            player->coord.y = ...
            player->direction = ...
        }
        if (keyState[ALLEGRO_KEY_D]) {
            player->coord.y = ...
            player->direction = ...

    }
    */
    
    if(isCollision(player, map)){
        player->coord.x = round((float)original.x / (float)TILE_SIZE) * TILE_SIZE;
    }
    player->animation_tick++;
    if(player->animation_tick >= 64){
        player->animation_tick = 0;
    }
    if(player->status == PLAYER_DYING){
        player->dying_tick++;
        if(player->dying_tick >= 128){
            player->dying_tick = 96;
            return;
        }
    }
    /*
        [TODO Homework] DONE
        
        Calculate the animation tick to draw animation later
    */
};
static void init(void){
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
}
void draw_player(Player * player, Point cam){
    int dy = player->coord.y - cam.y; // destiny y axis
    int dx = player->coord.x - cam.x; // destiny x axis
    
    int flag = 0; // Change the flag to flip character
    if(player->status == PLAYER_DYING){
        if(player->dying_tick >= 128){
            player->status = PLAYER_DYING;
        }
        int tint_red = player->knockback_CD > 0? 0 : 255;
        int offset = player->dying_tick/30*32;
        al_draw_tinted_scaled_bitmap(player->image,al_map_rgb(255, tint_red, tint_red), offset, 64, 32, 32, dx, dy, TILE_SIZE, TILE_SIZE, flag);
    }
    else if(player->status == PLAYER_WALKING){
        if(player->animation_tick > 128){
            player->animation_tick = 0;
        }
        int tint_red = player->knockback_CD > 0? 0 : 225;
        int offset = player->animation_tick/32*32;
        al_draw_tinted_scaled_bitmap(player->image,al_map_rgb(255, tint_red, tint_red), offset, 32, 32, 32, dx, dy, TILE_SIZE, TILE_SIZE, flag);
    }
    else if (player->status == PLAYER_IDLE){
        if(player->animation_tick >= 32){
            player->animation_tick = 0;
        }
        int tint_red = player->knockback_CD > 0? 0 : 225;
        int offset = player->animation_tick/20*32;
        al_draw_tinted_scaled_bitmap(player->image,al_map_rgb(255, tint_red, tint_red), offset, 0, 32, 32, dx, dy, TILE_SIZE, TILE_SIZE, flag);
    }
    /*
        [TODO Homework] DONE
        
        Draw Animation of Dying, Walking //
    */
    
#ifdef DRAW_HITBOX
    al_draw_rectangle(
        dx, dy, dx + TILE_SIZE, dy + TILE_SIZE,
        al_map_rgb(255, 0, 0), 1
    );
#endif
}

void delete_player(Player * player){
    al_destroy_bitmap(player->image);
}

static bool isCollision(Player* player, Map* map){
//to check whether the player has collided (wall, enemy)
    
    //if the player is out of bounds, it returns true (collision)
    if (player->coord.x < 0 ||
        player->coord.y < 0 ||
        (player->coord.x + TILE_SIZE - 1) / TILE_SIZE >= map->col || //exceeds map width
        (player->coord.y + TILE_SIZE - 1) / TILE_SIZE >= map->row) //exceeds map height
        return true;
    
    //player's grid coordinates
    int playerXS = player->coord.x/TILE_SIZE; //player x-coordinate start
    int playerYS = player->coord.y/TILE_SIZE; //player y-coordinate start
    int playerXE = (player->coord.x + TILE_SIZE - 1) / TILE_SIZE; //x-end
    int playerYE = (player->coord.y + TILE_SIZE - 1) / TILE_SIZE; //y-end
    
    //kalau isWalkable function return true, yg berarti ga nabrak berarti return true di if ga ngelakuin apapun
    if(!isWalkable(map->map[playerYS][playerXS]))return true;
    if(!isWalkable(map->map[playerYS][playerXE]))return true;
    if(!isWalkable(map->map[playerYE][playerXS]))return true;
    if(!isWalkable(map->map[playerYE][playerXE]))return true;
    /*

        [TODO HACKATHON 2-1] 
     
    
        Check every corner of player if it's collide or not

        We check every corner if it's collide with the wall/hole or not
        You can activate DRAW_HITBOX in utility.h to see how the collision work

        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;

    */
    
    return false;
}

void hitPlayer(Player * player, Point enemy_coord, int damage){
    if(player->knockback_CD == 0){
        float dY = player->coord.y - enemy_coord.y;
        float dX = player->coord.x - enemy_coord.x;
        float angle = atan2(dY, dX);
        player->knockback_angle = angle;
        player->knockback_CD = 32;
        player->health -= damage;
        if(player->health<=0){
            player->health = 0;
            player->status = PLAYER_DYING;
        }

        /*
            [TODO Homework] DONE
            
            Game Logic when the player get hit or die

            player->knockback_angle = angle;
            player->knockback_CD = 32;

            player->health = ...
            if(player->health <= 0){

                player->health = 0;
                player->status = ...

                ...
            }
        */
    }
}
