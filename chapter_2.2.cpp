#define GL_SILENCE_DEPRECATION

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600
#define WINDOW_TITLE "Chapter 2.2"

GLFWwindow* window;
GLuint
  VertexShaderId,
  FragmentShaderId,
  ProgramId,
  VaoId,
  VboId,
  ColorBufferId;

void Cleanup(void);
void CreateVBO(void);
void DestroyVBO(void);
void CreateShaders(void);
void DestroyShaders(void);

const GLchar* VertexShader = R"STR(
  #version 400

  layout(location=0) in vec4 in_Position;
  layout(location=1) in vec4 in_Color;
  out vec4 ex_Color;

  void main(void) {
    gl_Position = in_Position;
    ex_Color = in_Color;
  }
)STR";

const GLchar* FragmentShader = R"STR(
  #version 400

  in vec4 ex_Color;
  out vec4 out_Color;

  void main(void) {
    out_Color = ex_Color;
  }
)STR";

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

  CreateShaders();
  CreateVBO();

  while (!glfwWindowShouldClose(window))
  {
    RenderFunction();
    glfwPollEvents();
  }

  Cleanup();

  glfwDestroyWindow(window);
  glfwTerminate();

  exit(EXIT_SUCCESS);
}

void Cleanup(void) {
  DestroyShaders();
  DestroyVBO();
}

void RenderFunction() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDrawArrays(GL_TRIANGLES, 0, 6);

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

void CreateVBO(void) {
  GLfloat Verticies[] = {
    -0.8f,  0.8f, 0.0f, 1.0f,
    0.8f,  0.8f, 0.0f, 1.0f,
    -0.8f, -0.8f, 0.0f, 1.0f,

    -0.8f, -0.8f, 0.0f, 1.0f,
    0.8f,  0.8f, 0.0f, 1.0f,
    0.8f, -0.8f, 0.0f, 1.0f
  };

  GLfloat Colors[] = {
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,

    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f
  };

  GLenum ErrorCheckValue = glGetError();

  glGenVertexArrays(1, &VaoId);
  glBindVertexArray(VaoId);

  glGenBuffers(1, &VboId);
  glBindBuffer(GL_ARRAY_BUFFER, VboId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Verticies), Verticies, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &ColorBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);

  ErrorCheckValue = glGetError();
  if (ErrorCheckValue != GL_NO_ERROR) {
    fprintf(
      stderr,
      "ERROR: Could not create a VBO: %s \n",
      gluErrorString(ErrorCheckValue)
    );

    exit(-1);
  }
}

void CreateShaders(void) {
  GLenum ErrorCheckValue = glGetError();

  VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(VertexShaderId, 1, &VertexShader, NULL);
  glCompileShader(VertexShaderId);

  FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(FragmentShaderId, 1, &FragmentShader, NULL);
  glCompileShader(FragmentShaderId);

  ProgramId = glCreateProgram();
    glAttachShader(ProgramId, VertexShaderId);
    glAttachShader(ProgramId, FragmentShaderId);
  glLinkProgram(ProgramId);
  glUseProgram(ProgramId);

  ErrorCheckValue = glGetError();
  if (ErrorCheckValue != GL_NO_ERROR) {
    fprintf(
      stderr,
      "ERROR: Could not create the shaders: %s \n",
      gluErrorString(ErrorCheckValue)
    );

    exit(-1);
  }
}

void DestroyShaders(void) {
  GLenum ErrorCheckValue = glGetError();

  glUseProgram(0);

  glDetachShader(ProgramId, VertexShaderId);
  glDetachShader(ProgramId, FragmentShaderId);

  glDeleteShader(FragmentShaderId);
  glDeleteShader(VertexShaderId);

  glDeleteProgram(ProgramId);

  ErrorCheckValue = glGetError();
  if (ErrorCheckValue != GL_NO_ERROR) {
    fprintf(
      stderr,
      "ERROR: Could not destroy the shaders: %s \n",
      gluErrorString(ErrorCheckValue)
    );

    exit(-1);
  }
}

void DestroyVBO(void) {
  GLenum ErrorCheckValue = glGetError();
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &ColorBufferId);
  glDeleteBuffers(1, &VboId);

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &VaoId);

  ErrorCheckValue = glGetError();
  if (ErrorCheckValue != GL_NO_ERROR) {
    fprintf(
      stderr,
      "ERROR: Could not destroy the VBO: %s \n",
      gluErrorString(ErrorCheckValue)
    );

    exit(-1);
  }
}