#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include "Tex_Union.h"
#include "Map.h"

struct Player: public Map_Union {
    Player(int w, int h, int W0, int H0, const char* big_map_file, int nmaps, int* W, int* H, const char** map_files, float fraction=0.3);
    void initialize();
    
    int gp_index() {return maps[map_pos].full_w * global_pos.y + global_pos.x;}
    int gpl_index() {return maps[map_pos].full_w * last_global_pos.y + last_global_pos.x;}
    char gp(int i) {
        if (i >= 0 && i < maps[map_pos].full_w * maps[map_pos].full_h) return maps[map_pos].map_code[i];
        else return '!';
    }
    void change_map(int i);
    void global_synh() {last_global_pos = global_pos;}
    void change_global_pos(MovementDir dir);
    void forward_change_global_pos(MovementDir dir);
    void change_local_pos(MovementDir dir);
    void draw();
    void draw_mini();
    void draw_hdd();
    int on_tr() {
        int a = gp(gp_index());
        if (a >= '0' && a <= '9') return a - '0';
        else return -1;
    }
    void tr();
    bool if_tr() {return tr_iter != 27;}
    void checking() {
        if (gp(gp_index()) == 'T' && health > 0) {health--;}
        if (gp(gp_index()) == '.') {if (health > 15) health -= 5; else if (health > 0) health--;}
        if (health == 0) game_mode = '-';
        if (gp(gp_index()) == '+') game_mode = '+';
    }
    void making() {
        if (game_mode == '-') loser.draw();
        if (game_mode == '+') winner.draw();
    }
    // // // // // // // // // // // // // // // // //
    void draw_inventory();
    void draw_health();
    void draw_coffee();
    void coffee_to_health() {if (coffee > 0 && health < 255) {coffee--; health++;}}
    bool on_coffee() {return gp(gp_index()) == 'C';}
    void inc_coffee() {if (coffee < 255) coffee++;}

private:
    int block_size;
    int macro_block_size;
    float fraction;
    WH_int window;
    WH_int field;
    XY_int local_pos;
    XY_int global_pos;
    XY_int last_global_pos;
    int map_pos;
    int move_speed = 15;
    char where = 'f';
    bool just_tr = true;
    int tr_iter = 27;
    int fut_map;
    int health = 255;
    int coffee = 0;
    char game_mode = 'g';
    
    
    Tex_Union4 Grass;
    Tex_Union4 Walls;
    Tex_Union4 Lava;
    Tex_Union4 Water;
    Tex_Union4 Automat;
    Tex_Union4 Hole;
    Tex_Union4 Dark_Water;
    Tex_Union4 Drake;
    Tex_Union4 Ship;
    Tex_Union4 Ship_Sign;
    Tex_Union4 Drake_Sign;
    Tex_Union4 Left_Door;
    Tex_Union4 Right_Door;
    Tex_Union3 winner;
    Tex_Union3 loser;
    Tex_Union3 Frames;
    Tex_Union4 Health;
    Tex_Union4 AAA;
    Tex_Union4 Yes;
    Tex_Union4 Coffee;
    Tex_Union4 Home;
    
    Tex_Pix inventory;
    MovementDir directory;
    MovementDir ship_directory;
        
    friend class Macro;
};

#endif //MAIN_PLAYER_H
