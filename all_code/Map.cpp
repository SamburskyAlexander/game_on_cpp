#include "Map.h"

void Map::initialize(int w, int h, int W, int H, const char* file_name, float fraction) {
    full_w = W; // длина карты (число стб)
    full_h = H; // высота карты (число стр)
    std::string line;
    std::ifstream in(file_name);
    start_global_pos.x = -1;
    int curr_size = 0;
    int curr_y = 0;
    while (getline(in, line) && curr_size < W * H){
        int line_len = line.length();
        int i = 0;
        while (i < line_len && i < full_w && i + curr_size < W * H){
            if (line[i] == 'S') {
                line[i] = ' ';
                start_global_pos.x = i;
                start_global_pos.y = curr_y;
            }
            else if (line[i] >= '0' && line[i] <= '1' && start_global_pos.x == -1) {
                start_global_pos.x = i;
                start_global_pos.y = curr_y;
            }
            map_code.push_back(line[i]);
            i++;
        }
        while (i < full_w) {
            map_code.push_back(' ');
            i++;
        }
        if (line_len <= full_w) curr_size += line_len;
        else curr_size += full_w;
        curr_y += 1;
    }
    in.close();
    curr_size = map_code.size();
    for (int i = curr_size; i < W * H; ++i){
        map_code.push_back(' ');
    }
    this->fraction=fraction;
    
    mini_map = Tex_Pix(w, h);
    
    mini_size = w * fraction;
    mini_map.set_size(mini_size + 2, mini_size + 2);
    mini_map.set_position(w - mini_size / 2 + 2, h - mini_size / 2 - 19);
    
}

void Map::make_mini_map(){
    if (full_h <= full_w){
        mini_block_size = int(mini_map.size.w / full_w);
        mini_xorigin = 1;
        mini_yorigin = mini_map.size.w - mini_block_size - 1;
    } else {
        mini_block_size = int(mini_map.size.w / full_h);
        mini_xorigin = 0;
        mini_yorigin = mini_map.size.w - mini_block_size - 1;
    }
}

XY_int Map::get_mini_block_coords(int i) {
    XY_int p;
    p.x = mini_xorigin + (mini_block_size * (i % full_w));
    p.y = mini_yorigin - (mini_block_size * (int)(i / full_w));
    return p;
}

void Map::draw_mini_map() {
    int curr_i;
    mini_map.clear();
    for (int i = 0; i < full_h; ++i) {
        for (int j = 0; j < full_w; ++j) {
            curr_i = full_w * i + j;
            switch (map_code[curr_i]) {
                case ' ':
                    draw_mini_block(curr_i, Pixel3{0, 0, 0});
                    break;
                case '#':
                    draw_mini_block(curr_i, Pixel3{0, 63 / 4, 0});
                    break;
                case 'T':
                    draw_mini_block(curr_i, Pixel3{63 / 4, 0, 0});
                    break;
                case 'C':
                    draw_mini_block(curr_i, Pixel3{0, 63 / 4, 63 / 4});
                    break;
                case '.':
                    draw_mini_block(curr_i, Pixel3{63 / 4, 0, 63 / 4});
                    break;
                case '+':
                draw_mini_block(curr_i, Pixel3{63 / 4, 63 / 4, 63 / 4});
                break;
                default:
                    draw_mini_block(curr_i, Pixel3{0, 0, 63 / 4});
                    break;
            }
            if (map_code[curr_i] >= '0' && map_code[curr_i] <= '9')
                draw_mini_block(curr_i, Pixel3{0, 63, 63});
        }
    }
}

void Map::draw_hdd1() {
    mini_map.draw();
}

void Map::draw_mini_block(int i, const Pixel3& color) {
    XY_int coords = get_mini_block_coords(i);
    for (int i = 0; i < mini_block_size; ++i) {
        for (int j = 0; j < mini_block_size; ++j) {
            mini_map.put_pixel(coords.x + i, coords.y + j, color);
        }
    }
}


Map_Union::Map_Union(int w, int h, int W0, int H0, const char* big_map_file, int n_maps, int* W, int* H, const char** map_files, float fraction) {
    nmaps = n_maps;
    maps = new Map [nmaps + 1];
    maps[0].initialize(w, h, W0, H0, big_map_file, fraction);
    maps[0].make_mini_map();
    for (int i = 1; i < nmaps + 1; i++) {
        maps[i].initialize(w, h, W[i], H[i], map_files[i], fraction);
        maps[i].make_mini_map();
    }
}

Map& Map_Union::operator[](int i) {
    return maps[i];
}
