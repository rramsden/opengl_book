#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600
#define WINDOW_TITLE "Chapter 1"

GLFWwindow* window;

using namespace std;

void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction();
void RenderFunction();

static void ErrorCallback(int /* error */, const char* description) {
    fputs(description, stderr);
}

static void KeyCallback(GLFWwindow* window, int key, int /* scancode */, int action, int /* mods */) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(int argc, char *argv[]) {
  Initialize(argc, argv);

  while (!glfwWindowShouldClose(window))
  {
    ResizeFunction();
    RenderFunction();
    glfwPollEvents(); // process callbacks
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  exit(EXIT_SUCCESS);
}

void RenderFunction() {
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glfwSwapBuffers(window);
}

void ResizeFunction() {
  glViewport(0, 0, WIDTH, HEIGHT);
}

void Initialize(int argc, char* argv[]) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  InitWindow(argc, argv);
  
  cout << 
    "INFO: GL_RENDERER: " << glGetString(GL_RENDERER) << "\n" << 
    "      GL_VENDOR:   " << glGetString(GL_VENDOR) << "\n" <<
    "      GL_VERSION:  " << glGetString(GL_VERSION) << endl;
}

void InitWindow(int argc, char* argv[]) {
  glfwSetErrorCallback(ErrorCallback);

  if (!glfwInit())
      exit(EXIT_FAILURE);

  window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_TITLE, NULL, NULL);

  if (!window)
  {
      glfwTerminate();
      exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, KeyCallback);
}
