#include "Texture.h"

void Texture::move(MovementDir dir, int path) {
    switch(dir) {
        case MovementDir::UP:
            path *= speed.y;
            trans = glm::translate(trans, glm::vec3(0.0f, 2*(float)path / window.h, 0.0f));
            pos.y += path;
            break;
        case MovementDir::DOWN:
            path *= speed.y;
            trans = glm::translate(trans, glm::vec3(0.0f, -2*(float)path / window.h, 0.0f));
            pos.y -= path;
            break;
        case MovementDir::LEFT:
            path *= speed.x;
            trans = glm::translate(trans, glm::vec3(-2*(float)path / window.w, 0.0f, 0.0f));
            pos.x -= path;
            break;
        case MovementDir::RIGHT:
            path *= speed.x;
            trans = glm::translate(trans, glm::vec3(2*(float)path / window.w, 0.0f, 0.0f));
            pos.x += path;
            break;
        default:
            break;
    }
}

void Texture::rotate(float rad) {
    trans = glm::rotate(trans, glm::radians(-rotor + rad), glm::vec3(0.0, 0.0, 1.0));
    rotor = rad;
}

void Texture::set_position(int x, int y) {
    move(MovementDir::RIGHT, -pos.x + x);
    move(MovementDir::UP, -pos.y + y);
    pos.x = x; pos.y = y;
}

void Texture::set_size(int w, int h) {
    trans = glm::scale(trans, glm::vec3((float)w / window.w, (float)h / window.h, 0.0));
    speed.x *= (float)window.w / w;
    speed.y *= (float)window.h / h;
}


void Texture::draw() {
    glBindTexture(GL_TEXTURE_2D, texture); //// !!!!
    //glUseProgram(shader_program);
    //unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
    //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

Texture::~Texture() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}



void Tex_Pix::put_pixel(int x, int y, const Pixel3& color) {
    pixels[size.w * y + x] = color;
}

Pixel3 Tex_Pix::get_pixel(int x, int y) {
    return pixels[size.w * y + x];
}

/*void Tex_Pix::set_size(int w, int h) {
    trans = glm::scale(trans, glm::vec3((float)w / window.w, (float)h / window.h, 0.0));
    speed.x *= (float)window.w / w;
    speed.y *= (float)window.h / h;
    //size.w = w;
    //size.h = h;
}*/

void Tex_Pix::draw() {
    glBindTexture(GL_TEXTURE_2D, texture);
    glUseProgram(shader_program);
    unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    glBindVertexArray(VAO);
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,size.w,size.h,GL_RGB,GL_UNSIGNED_BYTE,pixels);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBegin(GL_POLYGON);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(1.0, 0.0, 0.0);
        glVertex3f(1.0, 1.0, 0.0);
        glVertex3f(0.0, 1.0, 0.0);
    glEnd();
}
