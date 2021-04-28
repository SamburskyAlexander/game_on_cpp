#include "Player.h"

Player::Player(int w, int h, int W0, int H0, const char* big_map_file, int nmaps, int* W, int* H, const char** map_files, float fraction):
        Map_Union(w, h, W0, H0, big_map_file, nmaps, W, H, map_files, fraction),
        Grass(w, h, 121, "./resources/Grass4.png", true),
        Walls(w, h, 121, "./resources/Stone.png", true),
        Lava(w, h, 121, "./resources/Lava.png", true),
        Water(w, h, 121, "./resources/Water.png", true),
        Automat(w, h, 121, "./resources/Automat.png", true),
        Hole(w, h, 121, "./resources/Hole.png", true),
        Dark_Water(w, h, 1, "./resources/Water2.png", true),
        Frames(w, h, 6, "./resources/Frame.png", true),
        Ship_Sign(w, h, 1, "./resources/Ship_Sign.png", true),
        Drake_Sign(w, h, 10, "./resources/Drake_Sign.png", true),
        Left_Door(w, h, 1, "./resources/Left.png", true),
        Right_Door(w, h, 1, "./resources/Right.png", true),
        Drake(w, h, 1, "./resources/Drake.png", true),
        winner(w, h, 1, "./resources/Winner.png", true),
        loser(w, h, 1, "./resources/Loser.png", true),
        Ship(w, h, 1, "./resources/Ship.png", true),
        Health(w, h, 1, "./resources/Health.png", true),
        AAA(w, h, 1, "./resources/aaa0.png", true),
        Yes(w, h, 1, "./resources/Yes.png", true),
        Coffee(w, h, 1, "./resources/Coffee.png", true),
        Home(w, h, 1, "./resources/Home.png", true),
        inventory(w, h) {
    window.w = w;
    window.h = h;
    map_pos = 1;
    global_pos.x = last_global_pos.x = maps[map_pos].start_global_pos.x;
    global_pos.y = last_global_pos.y = maps[map_pos].start_global_pos.y;
    this->fraction = fraction;
    field.h = h;
    field.w = w * (1 - this->fraction);
    local_pos.x = 0;
    local_pos.y = 0;
    macro_block_size = window.h / 10;
    directory = MovementDir::RIGHT;
    ship_directory = MovementDir::RIGHT;
    maps[map_pos].draw_mini_map();
    initialize();
    
    int mini_size = w * fraction;
    inventory.set_size(mini_size + 2, mini_size + 2);
    inventory.set_position(w - mini_size / 2 + 2, mini_size / 2 + 19);
    inventory.clear();
    draw_inventory();
}

void Player::initialize() {
    Drake.set_size(window.w / 10, -window.h / 10);
    Drake.set_position(field.w / 2 + 10, field.h / 2);
    Health.set_size(inventory.get_size().w * 0.9, -inventory.get_size().h / 10);
    Health.set_position(window.w * 9 / 10 - 20, field.h / 2 - 130);
    Coffee.set_size(inventory.get_size().w * 0.9, -inventory.get_size().h / 3);
    Coffee.set_position(window.w * 9 / 10 - 13, field.h / 2 - 550);
    AAA.set_size(inventory.get_size().w * 0.7, -200);
    AAA.set_position(window.w / 2 - 100, field.h / 2 + 130);
    Yes.set_size(inventory.get_size().w * 0.7, -200);
    Yes.set_position(window.w / 2 - 100, field.h / 2 + 130);
    Ship.set_size(window.w / 10, -window.h / 10);
    Ship_Sign.set_size(macro_block_size, -macro_block_size);
    Drake_Sign.set_size(macro_block_size, -macro_block_size);
    Left_Door.set_size(window.w / 2, -window.h);
    Left_Door.set_position(0 - window.w / 2, window.h / 2);
    Right_Door.set_size(window.w / 2, -window.h);
    Right_Door.set_position(window.w + window.w / 2, window.h / 2);
    winner.set_size(window.w, -window.h);
    loser.set_size(window.w, -window.h);
    Ship.set_position(field.w / 2 + 10, field.h / 2);
    Grass.set_size(macro_block_size, -macro_block_size);
    Home.set_size(macro_block_size, -macro_block_size);
    Hole.set_size(macro_block_size, -macro_block_size);
    Automat.set_size(macro_block_size / 2, -macro_block_size);
    Walls.set_size(macro_block_size, -macro_block_size);
    Water.set_size(macro_block_size, macro_block_size);
    Lava.set_size(macro_block_size, macro_block_size);
    Frames.set_size(0, window.w / 30, window.h);
    Frames.set_position(0, window.w / 60, window.h / 2);
    Frames.set_size(1, window.w / 30, window.h);
    Frames.set_position(1, window.w - window.w / 60, window.h / 2);
    Frames.set_size(2, window.w / 30, window.h);
    Frames.set_position(2, field.w - window.w / 20, window.h / 2);
    Frames.set_size(3, window.w, window.w / 30);
    Frames.set_position(3, window.w / 2, window.h - window.w / 60);
    Frames.set_size(4, window.w, window.w / 30);
    Frames.set_position(4, window.w / 2, window.w / 60);
    Frames.set_size(5, window.w * fraction + 10, window.w / 30);
    Frames.set_position(5, window.w * (2 - fraction) / 2, field.h / 2);
}

void Player::change_map(int i) {
    if (just_tr) return;
    maps[map_pos].start_global_pos.x = global_pos.x;
    maps[map_pos].start_global_pos.y = global_pos.y;
    maps[map_pos].comp = true;
    tr_iter = -26;
    fut_map = i;
    //maps[map_pos].draw_mini_map();
}

void Player::change_global_pos(MovementDir dir) {
    if (dir == MovementDir::UP && global_pos.y != 0 && gp(gp_index() - maps[map_pos].full_w) != '#') {
        global_pos.y--;
        local_pos.y += -macro_block_size + move_speed;
    }
    else if (dir == MovementDir::DOWN && global_pos.y != maps[map_pos].full_h - 1 && gp(gp_index() + maps[map_pos].full_w) != '#') {
            global_pos.y++;
            local_pos.y += macro_block_size - move_speed;
    }
    if (dir == MovementDir::LEFT && global_pos.x != 0 && gp(gp_index() - 1) != '#') {
        global_pos.x--;
        local_pos.x += macro_block_size - move_speed;
    }
    else if (dir == MovementDir::RIGHT && global_pos.x != maps[map_pos].full_w - 1 && gp(gp_index() + 1) != '#') {
        global_pos.x++;
        local_pos.x += -macro_block_size + move_speed;
    }
}

void Player::forward_change_global_pos(MovementDir dir) {
    float mn = 75.0 / macro_block_size;
    float mny = 45.0 / macro_block_size;
    if (dir == MovementDir::UP && global_pos.y != 0 && gp(gp_index() - maps[map_pos].full_w) != '#') {
        if ((local_pos.x < -mn * macro_block_size && gp(gp_index() - maps[map_pos].full_w - 1) != '#') ||
            (local_pos.x > mn * macro_block_size && gp(gp_index() - maps[map_pos].full_w + 1) != '#') ||
            (local_pos.x >= -mn * macro_block_size && local_pos.x <= mn * macro_block_size))
        local_pos.y += move_speed;
    }
    else if (dir == MovementDir::DOWN && global_pos.y != maps[map_pos].full_h - 1 && gp(gp_index() + maps[map_pos].full_w) != '#') {
        if ((local_pos.x < -mn * macro_block_size && gp(gp_index() + maps[map_pos].full_w - 1) != '#') ||
            (local_pos.x > mn * macro_block_size && gp(gp_index() + maps[map_pos].full_w + 1) != '#') ||
            (local_pos.x >= -mn * macro_block_size && local_pos.x <= mn * macro_block_size))
        local_pos.y -= move_speed;
    }
    if (dir == MovementDir::LEFT && global_pos.x != 0 && gp(gp_index() - 1) != '#') {
        if ((local_pos.y < -mny * macro_block_size && gp(gp_index() + maps[map_pos].full_w - 1) != '#') ||
            (local_pos.y > mny * macro_block_size && gp(gp_index() - maps[map_pos].full_w - 1) != '#') ||
            (local_pos.y >= -mny * macro_block_size && local_pos.y <= mny * macro_block_size))
        local_pos.x -= move_speed;
    }
    else if (dir == MovementDir::RIGHT && global_pos.x != maps[map_pos].full_w - 1 && gp(gp_index() + 1) != '#') {
        if ((local_pos.y < -mny * macro_block_size && gp(gp_index() + maps[map_pos].full_w + 1) != '#') ||
            (local_pos.y > mny * macro_block_size && gp(gp_index() - maps[map_pos].full_w + 1) != '#') ||
            (local_pos.y >= -mny * macro_block_size && local_pos.y <= mny * macro_block_size))
        local_pos.x += move_speed;
    }
}

void Player::change_local_pos(MovementDir dir) { // !!!
    just_tr = false;
    switch(dir) {
        case MovementDir::UP:
        Ship.rotate(270);
        if (local_pos.y <= macro_block_size * 0.1) local_pos.y += move_speed;
        else if (local_pos.y <= macro_block_size * 0.7) forward_change_global_pos(dir);
        else change_global_pos(dir);
        break;
    case MovementDir::DOWN://
        Ship.rotate(90);
        if (local_pos.y >= -macro_block_size * 0.1) local_pos.y -= move_speed;
        else if (local_pos.y >= -macro_block_size * 0.7) forward_change_global_pos(dir);
        else change_global_pos(dir);
        break;
    case MovementDir::LEFT:
        if (directory == MovementDir::RIGHT) {
            Drake.set_size(-window.w, window.h);
            directory = MovementDir::LEFT;
        }
        Ship.rotate(180);
        if (local_pos.x >= -macro_block_size * 0.3) local_pos.x -= move_speed;
        else if (local_pos.x >= -macro_block_size * 0.5) forward_change_global_pos(dir);
        else change_global_pos(dir);
        break;
    case MovementDir::RIGHT:
        Ship.rotate(0);
        if (directory == MovementDir::LEFT) {
            Drake.set_size(-window.w, window.h);
            directory = MovementDir::RIGHT;
        }
        if (local_pos.x <= macro_block_size * 0.3) local_pos.x += move_speed;
        else if (local_pos.x <= macro_block_size * 0.5) forward_change_global_pos(dir);
        else change_global_pos(dir);
        break;
    default:
        break;
    }
}

void Player::draw() {
    Ship_Sign.set_position(-10000, -10000);
    Home.set_position(-10000, -10000);
    //Drake_Sign.set_position(-10000, -10000);
    int k1 = 0;
    int k2 = 0;
    int k3 = 0;
    int k4 = 0;
    int k5 = 0;
    int k6 = 0;
    for (int i = -5; i < 6; ++i) {
        for (int j = -5; j < 6; ++j) {
            int ij_index = gp_index() % maps[map_pos].full_w + j;
            if (ij_index >= 0 && ij_index < maps[map_pos].full_w) {
                int a = gp(gp_index() + maps[map_pos].full_w * i + j);
                switch(a) {
                    case ' ':
                        if (where == 'f') {
                            Grass.set_position(11 * (i + 5) + (j + 5), field.w / 2 + macro_block_size * j - local_pos.x, field.h / 2 - macro_block_size * i - local_pos.y);
                            Grass[k1++] = 11 * (i + 5) + (j + 5);
                        } else {
                            Water.set_position(11 * (i + 5) + (j + 5), field.w / 2 + macro_block_size * j - local_pos.x, field.h / 2 - macro_block_size * i - local_pos.y);
                            Water[k1++] = 11 * (i + 5) + (j + 5);
                        }
                        break;
                    case '#':
                        Walls.set_position(11 * (i + 5) + (j + 5), field.w / 2 + macro_block_size * j - local_pos.x, field.h / 2 - macro_block_size * i - local_pos.y);
                        Walls[k2++] = 11 * (i + 5) + (j + 5);
                        break;
                    case 'T':
                        Lava.set_position(11 * (i + 5) + (j + 5), field.w / 2 + macro_block_size * j - local_pos.x, field.h / 2 - macro_block_size * i - local_pos.y);
                        Lava[k3++] = 11 * (i + 5) + (j + 5);
                        break;
                    case '0':
                    Ship_Sign.set_position(field.w / 2 + macro_block_size * j - local_pos.x, field.h / 2 - macro_block_size * i - local_pos.y);
                    break;
                    case '.':
                    Hole.set_position(11 * (i + 5) + (j + 5), field.w / 2 + macro_block_size * j - local_pos.x, field.h / 2 - macro_block_size * i - local_pos.y);
                    Hole[k6++] = 11 * (i + 5) + (j + 5);
                    break;
                    case 'C':
                    Grass.set_position(11 * (i + 5) + (j + 5), field.w / 2 + macro_block_size * j - local_pos.x, field.h / 2 - macro_block_size * i - local_pos.y);
                    Grass[k1++] = 11 * (i + 5) + (j + 5);
                    Automat.set_position(11 * (i + 5) + (j + 5), field.w / 2 + macro_block_size * j - local_pos.x, field.h / 2 - macro_block_size * i - local_pos.y);
                    Automat[k5++] = 11 * (i + 5) + (j + 5);
                    break;
                    case '+':
                    Home.set_position(field.w / 2 + macro_block_size * j - local_pos.x, field.h / 2 - macro_block_size * i - local_pos.y);
                    break;
                }
                if (a >= '1' && a <= '9') {
                    Drake_Sign.set_position(a - '0', field.w / 2 + macro_block_size * j - local_pos.x, field.h / 2 - macro_block_size * i - local_pos.y);
                    Drake_Sign[k4++] = a - '0';
                }
            }
        }
    }
    Dark_Water.draw();
    if (where == 'f') Grass.draw(0, k1);
    else Water.draw(0, k1);
    Hole.draw(0, k6);
    Lava.draw(0, k3);
    Automat.draw(0, k5);
    Home.draw();
    Walls.draw(0, k2);
    Ship_Sign.draw();
    Drake_Sign.draw(0, k4);
    if (where == 'f') Drake.draw();
    else Ship.draw();
    Frames.draw();
    if (gp(gp_index()) == 'T' || gp(gp_index()) == '.') AAA.draw();
    if (gp(gp_index()) == 'C') Yes.draw();
    //Right_Door.draw();
}

void Player::draw_mini() {
    //maps[map_pos].draw_mini_map();
    draw_hdd();
    Health.draw();
    Coffee.draw();
    switch (gp(gpl_index())) {
    case ' ':
        maps[map_pos].draw_mini_block(gpl_index(), Pixel3{0, 0, 0});
        break;
    case 'T':
        maps[map_pos].draw_mini_block(gpl_index(), Pixel3{255 / 4, 0, 0});
        break;
    case 'E':
        maps[map_pos].draw_mini_block(gpl_index(), Pixel3{0, 255 / 4, 255 / 4});
        break;
    default:
        maps[map_pos].draw_mini_block(gpl_index(), Pixel3{0, 0, 0});
        break;
    }
    int i_min = (global_pos.x >= 3) ? -3 : (global_pos.x >= 2) ? -2 : (global_pos.x >= 1) ? -1 : 0;
    int i_max = (global_pos.x < maps[map_pos].full_w - 3) ? 3 : (global_pos.x < maps[map_pos].full_w - 2) ? 2 : (global_pos.x < maps[map_pos].full_w - 1) ? 1 : 0;
    int j_min = (global_pos.y >= 3) ? -3 : (global_pos.y >= 2) ? -2 : (global_pos.y >= 1) ? -1 : 0;
    int j_max = (global_pos.y < maps[map_pos].full_h - 3) ? 3 : (global_pos.y < maps[map_pos].full_h - 2) ? 2 : (global_pos.y < maps[map_pos].full_h - 1) ? 1 : 0;
    for (int i = i_min; i < i_max + 1; i++)
        for (int j = j_min; j < j_max + 1; j++) {
            uint8_t power;
            if (i == i_min || i == i_max || j == j_min || j == j_max) {
                power = 255 / 3;
            } else {
                int coef = i * i + j * j + 1;
                power = 200 - coef * 10;
            }
            int a = gp(gp_index() + maps[map_pos].full_w * j + i);
            switch (a) {
            case ' ':
                maps[map_pos].draw_mini_block(gp_index() + maps[map_pos].full_w * j + i, Pixel3{power, power, power});
                break;
            case 'T':
                maps[map_pos].draw_mini_block(gp_index() + maps[map_pos].full_w * j + i, Pixel3{power, 0, 0});
                break;
            case '#':
                    maps[map_pos].draw_mini_block(gp_index() + maps[map_pos].full_w * j + i, Pixel3{0, power, 0});
                    break;
            case 'C':
                    maps[map_pos].draw_mini_block(gp_index() + maps[map_pos].full_w * j + i, Pixel3{0, power, power});
                    break;
            case '.':
                    maps[map_pos].draw_mini_block(gp_index() + maps[map_pos].full_w * j + i, Pixel3{power, 0, power});
                    break;
            case '+':
                    maps[map_pos].draw_mini_block(gp_index() + maps[map_pos].full_w * j + i, Pixel3{power, power, 0});
                    break;
            default:
                maps[map_pos].draw_mini_block(gp_index() + maps[map_pos].full_w * j + i, Pixel3{0, 0, 0});
                break;
            }
            if (a >= '0' && a <= '9')
                maps[map_pos].draw_mini_block(gp_index() + maps[map_pos].full_w * j + i, Pixel3{0, (uint8_t)(255), (uint8_t)(255)});
        }
            maps[map_pos].draw_mini_block(gp_index(), Pixel3{255, 255, 255});
}


void Player::tr() {
    if (tr_iter == 0) {
        map_pos = fut_map;
        if (fut_map == 0) where = 's';
        else where = 'f';
        local_pos.x = local_pos.y = 0;
        global_pos.x = maps[map_pos].start_global_pos.x;
        global_pos.y = maps[map_pos].start_global_pos.y;
        if (!maps[map_pos].comp)
            maps[map_pos].draw_mini_map();
        just_tr = true;
        
    }
    Left_Door.set_position(-window.w / 2 / 676 * tr_iter * tr_iter +  window.w / 4, window.h / 2);
    Right_Door.set_position(window.w - (-window.w / 2 / 676 * tr_iter * tr_iter +  window.w / 4), window.h / 2);
    draw();
    draw_mini();
    Left_Door.draw();
    Right_Door.draw();
    tr_iter++;
}


void Player::draw_hdd() {
    maps[map_pos].mini_map.draw();
    draw_health();
    draw_coffee();
    inventory.draw();
}

void Player::draw_health() {
    for (int i = 0.1 * inventory.get_size().w; i < 0.1 * inventory.get_size().w + 0.8 * inventory.get_size().w * health / 255; i++)
    for (int j = 0.7 * inventory.get_size().h; j < 0.8 * inventory.get_size().h; j++)
        inventory.put_pixel(i, j, Pixel3{(uint8_t)(255 - health), (uint8_t)health, 0});
    for (int i = 0.1 * inventory.get_size().w + 0.8 * inventory.get_size().w * health / 255; i < 0.9 * inventory.get_size().w; i++)
    for (int j = 0.7 * inventory.get_size().h; j < 0.8 * inventory.get_size().h; j++)
        inventory.put_pixel(i, j, Pixel3{0, 0, 0});
}

void Player::draw_coffee() {
    for (int i = 0.1 * inventory.get_size().w; i < 0.1 * inventory.get_size().w + 0.8 * inventory.get_size().w * coffee / 255; i++)
    for (int j = 0.1 * inventory.get_size().h; j < 0.2 * inventory.get_size().h; j++)
        inventory.put_pixel(i, j, Pixel3{(uint8_t)((-1 + 150.0/255)*coffee + 255), (uint8_t)((-1 + 75.0/255)*coffee + 255), (uint8_t)((-1)*coffee + 255)});
    for (int i = 0.1 * inventory.get_size().w + 0.8 * inventory.get_size().w * coffee / 255; i < 0.9 * inventory.get_size().w; i++)
    for (int j = 0.1 * inventory.get_size().h; j < 0.2 * inventory.get_size().h; j++)
        inventory.put_pixel(i, j, Pixel3{0, 0, 0});
}

void Player::draw_inventory() {
    for (int i = 0.1 * inventory.get_size().w - 35; i < 0.1 * inventory.get_size().w + 0.8 * inventory.get_size().w * health / 255 + 35; i++)
    for (int j = 0.8 * inventory.get_size().h + 30; j < 0.8 * inventory.get_size().h + 35; j++)
        inventory.put_pixel(i, j, Pixel3{255, 255, 255});
    
    for (int i = 0.1 * inventory.get_size().w - 35; i < 0.1 * inventory.get_size().w + 0.8 * inventory.get_size().w * health / 255 + 35; i++)
    for (int j = 0.7 * inventory.get_size().h - 35; j < 0.7 * inventory.get_size().h - 30; j++)
        inventory.put_pixel(i, j, Pixel3{255, 255, 255});
    
    for (int i = 0.1 * inventory.get_size().w - 35; i < 0.1 * inventory.get_size().w - 30; i++)
    for (int j = 0.7 * inventory.get_size().h - 35; j < 0.8 * inventory.get_size().h + 35; j++)
        inventory.put_pixel(i, j, Pixel3{255, 255, 255});
    
    for (int i = 0.9 * inventory.get_size().w + 30; i < 0.9 * inventory.get_size().w + 35; i++)
    for (int j = 0.7 * inventory.get_size().h - 35; j < 0.8 * inventory.get_size().h + 35; j++)
        inventory.put_pixel(i, j, Pixel3{255, 255, 255});
    
    
    
    for (int i = 0.1 * inventory.get_size().w - 35; i < 0.1 * inventory.get_size().w + 0.8 * inventory.get_size().w * health / 255 + 35; i++)
    for (int j = 0.2 * inventory.get_size().h + 30; j < 0.2 * inventory.get_size().h + 35; j++)
        inventory.put_pixel(i, j, Pixel3{255, 255, 255});
    
    for (int i = 0.1 * inventory.get_size().w - 35; i < 0.1 * inventory.get_size().w + 0.8 * inventory.get_size().w * health / 255 + 35; i++)
    for (int j = 0.1 * inventory.get_size().h - 35; j < 0.1 * inventory.get_size().h - 30; j++)
        inventory.put_pixel(i, j, Pixel3{255, 255, 255});
    
    for (int i = 0.1 * inventory.get_size().w - 35; i < 0.1 * inventory.get_size().w - 30; i++)
    for (int j = 0.1 * inventory.get_size().h - 35; j < 0.2 * inventory.get_size().h + 35; j++)
        inventory.put_pixel(i, j, Pixel3{255, 255, 255});
    
    for (int i = 0.9 * inventory.get_size().w + 30; i < 0.9 * inventory.get_size().w + 35; i++)
    for (int j = 0.1 * inventory.get_size().h - 35; j < 0.2 * inventory.get_size().h + 35; j++)
        inventory.put_pixel(i, j, Pixel3{255, 255, 255});
}
