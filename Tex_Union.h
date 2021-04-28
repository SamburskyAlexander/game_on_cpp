#ifndef Tex_Union_hpp
#define Tex_Union_hpp

#include <iostream>
#include "Texture.h"

struct Tex_Union3 {
    Tex_Union3(int w, int h, unsigned int max_size, const char* a_path, bool auto_push = false);
    ~Tex_Union3() {delete [] all_tex; stbi_image_free(data); delete [] ind;}
    int get_size() {return size;}
    
    void push();
    
    void move(char c, unsigned int number, MovementDir dir, int path);
    void rotate(char c, unsigned int number, float rad);
    void set_position(char c, unsigned int number, int x, int y);
    void set_size(char c, unsigned int number, int w, int h);
    void draw(char c, unsigned int number);
    
    void move(unsigned int index, MovementDir dir, int path);
    void rotate(unsigned int index, float rad);
    void set_position(unsigned int index, int x, int y);
    void set_size(unsigned int index, int w, int h);
    void draw(unsigned int index);
    
    void move(MovementDir dir, int path);
    void rotate(float rad);
    void set_position(int x, int y);
    void set_size(int w, int h);
    void draw();
    
    unsigned& operator[](unsigned i);
    void put_pixel(int x, int y, const Pixel3& color);
    void reload();
    
private:
    Texture3* all_tex = nullptr;
    unsigned int size;
    unsigned int max_size;
    unsigned* ind = nullptr;
    WH_int window;
    
    int shader_program;
    Pixel3* data = nullptr;
    int width, height, nrChannels;
};





struct Tex_Union4 {
    Tex_Union4(int w, int h, unsigned int max_size, const char* a_path, bool auto_push = false);
    ~Tex_Union4() {delete [] all_tex; stbi_image_free(data); delete [] ind;}
    int get_size() {return size;}
    
    void push();
    
    void move(char c, unsigned int number, MovementDir dir, int path);
    void rotate(char c, unsigned int number, float rad);
    void set_position(char c, unsigned int number, int x, int y);
    void set_size(char c, unsigned int number, int w, int h);
    void draw(char c, unsigned int number);
    
    void move(unsigned int index, MovementDir dir, int path);
    void rotate(unsigned int index, float rad);
    void set_position(unsigned int index, int x, int y);
    void set_size(unsigned int index, int w, int h);
    void draw(unsigned int index);
    
    void move(MovementDir dir, int path);
    void rotate(float rad);
    void set_position(int x, int y);
    void set_size(int w, int h);
    void draw();
    
    unsigned& operator[](unsigned i);
    void put_pixel(int x, int y, const Pixel4& color);
    void reload();
        
private:
    Texture4* all_tex;
    unsigned int size;
    unsigned int max_size;
    unsigned* ind = nullptr;
    WH_int window;
    
    int shader_program;
    Pixel4* data = nullptr;
    int width, height, nrChannels;
};

#endif /* Tex_Union_hpp */
