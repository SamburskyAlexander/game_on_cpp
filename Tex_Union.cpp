#include "Tex_Union.h"
#include <iostream>

const char* asst1 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"    gl_Position = transform * vec4(aPos, 1.0);\n"
"    ourColor = aColor;\n"
"    TexCoord = aTexCoord;\n"
"}\n\0";

const char* asst2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D texture1;\n"
"void main() {\n"
"    vec4 texColor = texture(texture1, TexCoord);\n"
"    if(texColor.a < 0.1)\n"
"        discard;\n"
"    FragColor = texColor;\n"
"}\n\0";

//constexpr Pixel backgroundColor0{200, 0, 0, 0};

Tex_Union3::Tex_Union3(int w, int h, unsigned int max_size, const char* a_path, bool auto_push) {
    this->max_size = max_size;
    all_tex = new Texture3 [max_size];
    size = 0;
    window.w = w; window.h = h;
    
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &asst1, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &asst2, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertexShader);
    glAttachShader(shader_program, fragmentShader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    data = (Pixel3*)stbi_load(a_path, &width, &height, &nrChannels, 0);
    
    ind = new unsigned [max_size];
    if (auto_push) {
        for (int i = 0; i < max_size; ++i) {
            push();
        }
    }
}

void Tex_Union3::push() {
    if (size < max_size) {
        ind[size] = size;
        all_tex[size++].initialize(window.w, window.h, data, width, height, nrChannels);
    } else std::cout << "> Tex_Union is full\n";
}

void Tex_Union3::move(char c, unsigned int number, MovementDir dir, int path) {
    for (unsigned int i = 0; i < number; ++i) {
        all_tex[ind[i]].move(dir, path);
    }
}

void Tex_Union3::rotate(char c, unsigned int number, float rad) {
    for (unsigned int i = 0; i < number; ++i) {
        all_tex[ind[i]].rotate(rad);
    }
}

void Tex_Union3::set_position(char c, unsigned int number, int x, int y) {
    for (unsigned int i = 0; i < number; ++i) {
        all_tex[ind[i]].set_position(x, y);
    }
}

void Tex_Union3::set_size(char c, unsigned int number, int w, int h) {
    for (unsigned int i = 0; i < number; ++i) {
        all_tex[ind[i]].set_size(w, h);
    }
}

void Tex_Union3::draw(char c, unsigned int number) {
    glUseProgram(shader_program);
    unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
    for (unsigned int i = 0; i < number; ++i) {
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(all_tex[ind[i]].trans));
        all_tex[ind[i]].draw();
    }
}



void Tex_Union3::move(unsigned int index, MovementDir dir, int path) {
    all_tex[index].move(dir, path);
}

void Tex_Union3::rotate(unsigned int index, float rad) {
    all_tex[index].rotate(rad);
}

void Tex_Union3::set_position(unsigned int index, int x, int y) {
    all_tex[index].set_position(x, y);
}

void Tex_Union3::set_size(unsigned int index, int w, int h) {
    all_tex[index].set_size(w, h);
}

void Tex_Union3::draw(unsigned int index) {
    glUseProgram(shader_program);
    unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(all_tex[index].trans));
    all_tex[index].draw();
}



void Tex_Union3::move(MovementDir dir, int path) {
    for (unsigned int i = 0; i < size; ++i) {
        all_tex[i].move(dir, path);
    }
}

void Tex_Union3::rotate(float rad) {
    for (unsigned int i = 0; i < size; ++i) {
        all_tex[i].rotate(rad);
    }
}

void Tex_Union3::set_position(int x, int y) {
    for (unsigned int i = 0; i < size; ++i) {
        all_tex[i].set_position(x, y);
    }
}

void Tex_Union3::set_size(int w, int h) {
    for (unsigned int i = 0; i < size; ++i) {
        all_tex[i].set_size(w, h);
    }
}

void Tex_Union3::draw() {
    glUseProgram(shader_program);
    unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
    for (unsigned int i = 0; i < size; ++i) {
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(all_tex[i].trans));
        all_tex[i].draw();
    }
}

unsigned& Tex_Union3::operator[](unsigned i) {
    return ind[i];
}

void Tex_Union3::put_pixel(int x, int y, const Pixel3& color) {
    data[width * y + x] = color;
}

void Tex_Union3::reload() {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}


// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

Tex_Union4::Tex_Union4(int w, int h, unsigned int max_size, const char* a_path, bool auto_push){
    this->max_size = max_size;
    all_tex = new Texture4 [max_size];
    size = 0;
    window.w = w; window.h = h;
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &asst1, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &asst2, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertexShader);
    glAttachShader(shader_program, fragmentShader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    data = (Pixel4*)stbi_load(a_path, &width, &height, &nrChannels, 0);
    ind = new unsigned [max_size];
    if (auto_push) {
        for (int i = 0; i < max_size; ++i) {
            push();
        }
    }
}

void Tex_Union4::push() {
    if (size < max_size) all_tex[size++].initialize(window.w, window.h, data, width, height, nrChannels);
    else std::cout << "> Tex_Union is full\n";
}

void Tex_Union4::move(char c, unsigned int number, MovementDir dir, int path) {
    for (unsigned int i = 0; i < number; ++i) {
        all_tex[ind[i]].move(dir, path);
    }
}

void Tex_Union4::rotate(char c, unsigned int number, float rad) {
    for (unsigned int i = 0; i < number; ++i) {
        all_tex[ind[i]].rotate(rad);
    }
}


void Tex_Union4::set_position(char c, unsigned int number, int x, int y) {
    for (unsigned int i = 0; i < number; ++i) {
        all_tex[ind[i]].set_position(x, y);
    }
}

void Tex_Union4::set_size(char c, unsigned int number, int w, int h) {
    for (unsigned int i = 0; i < number; ++i) {
        all_tex[ind[i]].set_size(w, h);
    }
}

void Tex_Union4::draw(char c, unsigned int number) {
    glUseProgram(shader_program);
    unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
    for (unsigned int i = 0; i < number; ++i) {
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(all_tex[ind[i]].trans));
        all_tex[ind[i]].draw();
    }
}



void Tex_Union4::move(unsigned int index, MovementDir dir, int path) {
    all_tex[index].move(dir, path);
}

void Tex_Union4::rotate(unsigned int index, float rad) {
    all_tex[index].rotate(rad);
}

void Tex_Union4::set_position(unsigned int index, int x, int y) {
        all_tex[index].set_position(x, y);
}

void Tex_Union4::set_size(unsigned int index, int w, int h) {
    all_tex[index].set_size(w, h);
}

void Tex_Union4::draw(unsigned int index) {
    glUseProgram(shader_program);
    unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(all_tex[index].trans));
    all_tex[index].draw();
}



void Tex_Union4::move(MovementDir dir, int path) {
    for (unsigned int i = 0; i < size; ++i) {
        all_tex[i].move(dir, path);
    }
}

void Tex_Union4::rotate(float rad) {
    for (unsigned int i = 0; i < size; ++i) {
        all_tex[i].rotate(rad);
    }
}

void Tex_Union4::set_position(int x, int y) {
    for (unsigned int i = 0; i < size; ++i) {
        all_tex[i].set_position(x, y);
    }
}

void Tex_Union4::set_size(int w, int h) {
    for (unsigned int i = 0; i < size; ++i) {
        all_tex[i].set_size(w, h);
    }
}

void Tex_Union4::draw() {
    glUseProgram(shader_program);
    unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
    for (unsigned int i = 0; i < size; ++i) {
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(all_tex[i].trans));
        all_tex[i].draw();
    }
}

unsigned& Tex_Union4::operator[](unsigned i) {
    return ind[i];
}

void Tex_Union4::put_pixel(int x, int y, const Pixel4& color) {
    data[width * y + x] = color;
}

void Tex_Union4::reload() {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}
