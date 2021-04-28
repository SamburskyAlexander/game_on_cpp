#ifndef Map_h
#define Map_h

#include "Image.h"
#include "Texture.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct Map{
    Map(int w=1, int h=1): mini_map(w, h){};
    void initialize(int w, int h, int W, int H, const char* file_name, float fraction=0.3);
    void make_mini_map();
    XY_int get_mini_block_coords(int i);
    void draw_mini_map();
    void draw_hdd1();
    void draw_mini_block(int i, const Pixel3& color);

private:
    float fraction;
    int full_w;
    int full_h;
    int mini_block_size;
    int mini_xorigin;
    int mini_yorigin;
    XY_int start_global_pos;
    std::vector<char> map_code;
    int mini_size;
    Tex_Pix mini_map;
    bool comp = false;
    
    friend class Player;
    friend class Map_Union;
};

struct Map_Union {
    Map_Union(int w, int h, int W0, int H0, const char* big_map_file, int maps, int* W, int* H, const char** map_files, float fraction=0.3);
    Map& operator[](int i);

protected:
    Map* maps = nullptr;
    int nmaps;
    
    friend class Player;
};


#endif
