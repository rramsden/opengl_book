#define GL_SILENCE_DEPRECATION

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600
#define WINDOW_TITLE "Chapter 2"

GLFWwindow* window;

using namespace std;

void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction();

static void ErrorCallback(int /* error */, const char* description) {
    fputs(description, stderr);
}

static void KeyCallback(GLFWwindow* window, int key, int /* scancode */, int action, int /* mods */) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static void WindowSizeCallback(GLFWwindow* window, int width, int height) {
  ResizeFunction(width, height);
}

int main(int argc, char *argv[]) {
  Initialize(argc, argv);

  while (!glfwWindowShouldClose(window))
  {
    RenderFunction();
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  exit(EXIT_SUCCESS);
}

void RenderFunction() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Draw something on back buffer here */

  glfwSwapBuffers(window);
}

void ResizeFunction(int width, int height) {
  cout << "Resize Viewport: " << width << ", " << height << endl;
  glViewport(0, 0, width, height);
}

void Initialize(int argc, char* argv[]) {
  GLenum GlewInitResult;

  InitWindow(argc, argv);

  GlewInitResult = glewInit();

  if (GLEW_OK != GlewInitResult) {
    cout << "ERROR: " << glewGetErrorString(GlewInitResult) << endl;
    exit(EXIT_FAILURE);
  }

  ResizeFunction(WIDTH, HEIGHT);

  glfwSetWindowSizeCallback(window, WindowSizeCallback);
  
  cout << 
    "INFO: GL_RENDERER: " << glGetString(GL_RENDERER) << "\n" << 
    "      GL_VENDOR:   " << glGetString(GL_VENDOR) << "\n" <<
    "      GL_VERSION:  " << glGetString(GL_VERSION) << endl;
}

void InitWindow(int argc, char* argv[]) {
  glfwSetErrorCallback(ErrorCallback);

  if (!glfwInit())
      exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_TITLE, NULL, NULL);

  if (!window)
  {
      glfwTerminate();
      exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, KeyCallback);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}
