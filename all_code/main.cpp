#include "common.h"
//#include "Texture.h"
#include "Tex_Union.h"
#include "Image.h"
#include "Player.h"
#include "Map.h"
//#include "Macro.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

constexpr GLsizei WINDOW_WIDTH = 3000; //
constexpr GLsizei WINDOW_HEIGHT = 2100; //

struct InputState
{
  bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
  GLfloat lastX = 400, lastY = 300; //исходное положение мыши
  bool firstMouse = true;
  bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
  bool capturedMouseJustNow = false;
} static Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
  case GLFW_KEY_1:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  case GLFW_KEY_2:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
	default:
		if (action == GLFW_PRESS)
      Input.keys[key] = true;
		else if (action == GLFW_RELEASE)
      Input.keys[key] = false;
	}
}


void processPlayerMovement(Player& player) {
    player.global_synh();
    if (Input.keys[GLFW_KEY_W]) {
        player.change_local_pos(MovementDir::UP);
    } else if (Input.keys[GLFW_KEY_S]) {
        player.change_local_pos(MovementDir::DOWN);
    }
    if (Input.keys[GLFW_KEY_A]) {
        player.change_local_pos(MovementDir::LEFT);
    } else if (Input.keys[GLFW_KEY_D]) {
        player.change_local_pos(MovementDir::RIGHT);
    }
    int a;
    if (Input.keys[GLFW_KEY_Q] && (a=player.on_tr()) != -1) player.change_map(a);
    if (Input.keys[GLFW_KEY_SPACE] && player.on_coffee()) player.inc_coffee();
    if (Input.keys[GLFW_KEY_E]) player.coffee_to_health();
}
void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    Input.captureMouse = !Input.captureMouse;

  if (Input.captureMouse)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Input.capturedMouseJustNow = true;
  }
  else
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
  if (Input.firstMouse)
  {
    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
    Input.firstMouse = false;
  }

  GLfloat xoffset = float(xpos) - Input.lastX;
  GLfloat yoffset = Input.lastY - float(ypos);

  Input.lastX = float(xpos);
  Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
  // ...
}


int initGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  std::cout << "Controls: "<< std::endl;
  std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
  std::cout << "W, A, S, D - movement  "<< std::endl;
  std::cout << "press ESC to exit" << std::endl;
  std::cout << "press ESC to exit" << std::endl;

	return 0;
}



int main(int argc, char** argv) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // раскомментируйте эту строку, если используете macOS
    #endif
     
    #ifdef __APPLE__
        GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, "task1 base project", nullptr, nullptr);
    #else
        GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, "task1 base project", nullptr, nullptr);
    #endif
    
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback        (window, OnKeyboardPressed);
    glfwSetCursorPosCallback  (window, OnMouseMove);
    glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
    glfwSetScrollCallback     (window, OnMouseScroll);
     
    if(initGL() != 0) return -1;
    GLenum gl_error = glGetError();
    while (gl_error != GL_NO_ERROR) gl_error = glGetError();
    
    
    
    
    int nmaps = 3;
    const char** a = new const char* [nmaps];
    a[1] = new char[20];
    a[1] = "./Map1.txt";
    a[2] = new char[20];
    a[2] = "./Map2.txt";
    a[3] = new char[20];
    a[3] = "./Map3.txt";
    int* w = new int [nmaps];
    int* h = new int [nmaps];
    for (int i = 0; i < nmaps + 1; ++i) {
        w[i] = 20;
        h[i] = 20;
    }
    h[2] = 80;
    w[3] = 40;
    h[3] = 40;
    
    const char* b = new char[20];
    b = "./Map0.txt";
    Player player(WINDOW_WIDTH, WINDOW_HEIGHT, 20, 20, b, nmaps, w, h, a, 0.3);
    //Tex_Union3 AAA(WINDOW_WIDTH, WINDOW_HEIGHT, 1, "./resources/Grass.png", true);
    //AAA.set_size(100, 100);
    
    #ifdef __APPLE__
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
    #else
        glViewport(0, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);  GL_CHECK_ERRORS;
    #endif
    
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;

    // Цикл рендеринга
    while (!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if (!player.if_tr()) {
            processPlayerMovement(player);
            player.checking();
            player.draw();
            player.draw_mini();
        } else {
            player.tr();
        }
        player.making();
        
        //AAA.set_position(2*Input.lastX, WINDOW_HEIGHT -2*Input.lastY);//
        //AAA.draw();
        glfwSwapBuffers(window);
        //glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
