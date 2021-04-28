#ifndef MAIN_TEXTURE_H
#define MAIN_TEXTURE_H

//#include "Image.h"
#include <glad/glad.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

enum class MovementDir
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

struct Pixel4 {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

struct Pixel3 {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct WH_int {int w; int h;};
struct XY_float {float x; float y;};
struct XY_int {int x; int y;};

struct Texture {
    Texture() {};
    unsigned int get_VBO() const {return VBO;}
    unsigned int get_VAO() const {return VAO;}
    unsigned int get_EBO() const {return EBO;}
    ~Texture();
    
    void move(MovementDir dir, int path);
    void rotate(float rad);
    void set_position(int x, int y);
    void set_size(int w, int h);
    void draw();

protected:
    XY_int pos;
    float rotor = 0;
    WH_int window;
    
    unsigned int VBO, VAO, EBO, texture;
    glm::mat4 trans;
    XY_int speed;
    friend class Tex_Union;
};


struct Texture3: public Texture {
    void initialize(int w, int h, Pixel3* data, int width, int height, int nrChannels);
    
    friend class Tex_Union3;
};

struct Texture4: public Texture {
    void initialize(int w, int h, Pixel4* data, int width, int height, int nrChannels);
    
    friend class Tex_Union4;
};

struct Tex_Pix: public Texture {
    Tex_Pix(int w, int h);
    void put_pixel(int x, int y, const Pixel3& color);
    Pixel3 get_pixel(int x, int y);
    void clear() {
        for (int i = 0; i < size.w; ++i) {
        for (int j = 0; j < size.h; ++j) {
            put_pixel(i, j, Pixel3{0, 0, 0});
        }
        }
    }
    WH_int get_size() {return size;}
    void draw();

    unsigned int shader_program;
    WH_int size;
    Pixel3* pixels = nullptr;
    
    friend class Map;
};

#endif //MAIN_TEXTURE_H
