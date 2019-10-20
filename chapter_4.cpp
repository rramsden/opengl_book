#define GL_SILENCE_DEPRECATION

#include <iostream>
#include "Utils.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600
#define WINDOW_TITLE "Chapter 4"

GLFWwindow* window;
GLuint
  ProjectionMatrixUniformLocation,
  ViewMatrixUniformLocation,
  ModelMatrixUniformLocation,
  BufferIds[3] = { 0 },
  ShaderIds[3] = { 0 };

Matrix
  ProjectionMatrix,
  ViewMatrix,
  ModelMatrix;

float CubeRotation = 0;
clock_t LastTime = 0;

void CreateCube(void);
void DestroyCube(void);
void DrawCube(void);

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

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  ExitOnGLError("ERROR: Could not set OpenGL depth testing options");

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  ExitOnGLError("ERROR: Could not set OpenGL culling options");

  ModelMatrix = IDENTITY_MATRIX;
  ProjectionMatrix = IDENTITY_MATRIX;
  ViewMatrix = IDENTITY_MATRIX;
  TranslateMatrix(&ViewMatrix, 0, 0, -2);

  CreateCube();

  ResizeFunction(WIDTH, HEIGHT);

  while (!glfwWindowShouldClose(window))
  {
    RenderFunction();
    glfwPollEvents();
  }

  DestroyCube();

  glfwDestroyWindow(window);
  glfwTerminate();

  exit(EXIT_SUCCESS);
}

void RenderFunction() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  DrawCube();

  glfwSwapBuffers(window);
}

void CreateCube(void) {
  const Vertex VERTICES[8] = {
    { { -.5f, -.5f,  .5f, 1 }, { 0, 0, 1, 1 } },
    { { -.5f,  .5f,  .5f, 1 }, { 1, 0, 0, 1 } },
    { {  .5f,  .5f,  .5f, 1 }, { 0, 1, 0, 1 } },
    { {  .5f, -.5f,  .5f, 1 }, { 1, 1, 0, 1 } },
    { { -.5f, -.5f, -.5f, 1 }, { 1, 1, 1, 1 } },
    { { -.5f,  .5f, -.5f, 1 }, { 1, 0, 0, 1 } },
    { {  .5f,  .5f, -.5f, 1 }, { 1, 0, 1, 1 } },
    { {  .5f, -.5f, -.5f, 1 }, { 0, 0, 1, 1 } }
  };

  const GLuint INDICES[36] = {
    0, 2, 1, 0, 3, 2,
    4, 3, 0, 4, 7, 3,
    4, 1, 5, 4, 0, 1,
    3, 6, 2, 3, 7, 6,
    1, 6, 5, 1, 2, 6,
    7, 5, 6, 7, 4, 5
  };

  ShaderIds[0] = glCreateProgram();
  ExitOnGLError("ERROR: Could not create the shader program");

  ShaderIds[1] = LoadShader("SimpleShader.fragment.glsl", GL_FRAGMENT_SHADER);
  ShaderIds[2] = LoadShader("SimpleShader.vertex.glsl", GL_VERTEX_SHADER);
  glAttachShader(ShaderIds[0], ShaderIds[1]);
  glAttachShader(ShaderIds[0], ShaderIds[2]);

  glLinkProgram(ShaderIds[0]);
  ExitOnGLError("ERROR: Could not link the shader program");

  ModelMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ModelMatrix");
  ViewMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ViewMatrix");
  ProjectionMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ProjectionMatrix");
  ExitOnGLError("ERROR: Could not get the sahder uniform locations");

  glGenBuffers(2, &BufferIds[1]);
  ExitOnGLError("ERROR: Could not generate the buffer objects");

  glGenVertexArrays(1, &BufferIds[0]);
  ExitOnGLError("ERROR: Could not generate the VAO");
  glBindVertexArray(BufferIds[0]);
  ExitOnGLError("ERROR: Could not bind the VAO");

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  ExitOnGLError("ERROR: Could not enable vertex attributes");

  glBindBuffer(GL_ARRAY_BUFFER, BufferIds[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
  ExitOnGLError("ERROR: Could not bind the VBO to VAO");

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), (GLvoid*)0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), (GLvoid*)sizeof(VERTICES[0].Position));
  ExitOnGLError("ERROR: Could not set VAO attributes");

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIds[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);
  ExitOnGLError("ERROR: Could not bind the IBO to the VAO");

  glBindVertexArray(0);
}

void DestroyCube(void) {
  glDetachShader(ShaderIds[0], ShaderIds[1]);
  glDetachShader(ShaderIds[0], ShaderIds[2]);
  glDeleteShader(ShaderIds[1]);
  glDeleteShader(ShaderIds[2]);
  glDeleteProgram(ShaderIds[0]);
  ExitOnGLError("ERROR: Could not destroy the shaders");

  glDeleteBuffers(2, &BufferIds[1]);
  glDeleteVertexArrays(1, &BufferIds[0]);
  ExitOnGLError("ERROR: Could not destroy the buffer objects");
}

void DrawCube(void) {
  float CubeAngle;
  clock_t Now = clock();
  if (LastTime == 0)
    LastTime = Now;

  CubeRotation += 45.0f * ((float)(Now - LastTime) / CLOCKS_PER_SEC);
  CubeAngle = DegreesToRadians(CubeRotation);
  LastTime = Now;

  ModelMatrix = IDENTITY_MATRIX;
  RotateAboutY(&ModelMatrix, CubeAngle);
  RotateAboutX(&ModelMatrix, CubeAngle);

  glUseProgram(ShaderIds[0]);
  ExitOnGLError("ERROR: Could not use the shader program");

  glUniformMatrix4fv(ModelMatrixUniformLocation, 1, GL_FALSE, ModelMatrix.m);
  glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE, ViewMatrix.m);
  ExitOnGLError("ERROR: Could not set the shader uniforms");

  glBindVertexArray(BufferIds[0]);
  ExitOnGLError("ERROR: Could not bind the VAO for drawing purposes");

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLvoid*)0);
  ExitOnGLError("ERROR: Could not draw the cube");

  glBindVertexArray(0);
  glUseProgram(0);
}

void ResizeFunction(int width, int height) {
  cout << "Resize Viewport: " << width << ", " << height << endl;
  glViewport(0, 0, width, height);

  ProjectionMatrix =
    CreateProjectionMatrix(
      60,
      (float)WIDTH / HEIGHT,
      1.0f,
      100.0f
    );
  
  glUseProgram(ShaderIds[0]);
  glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, ProjectionMatrix.m);
  glUseProgram(0);
}

void Initialize(int argc, char* argv[]) {
  GLenum GlewInitResult;

  InitWindow(argc, argv);

  GlewInitResult = glewInit();

  if (GLEW_OK != GlewInitResult) {
    cout << "ERROR: " << glewGetErrorString(GlewInitResult) << endl;
    exit(EXIT_FAILURE);
  }

  // ResizeFunction(WIDTH, HEIGHT);

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
