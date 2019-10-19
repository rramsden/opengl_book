#define GL_SILENCE_DEPRECATION

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#define WIDTH 600
#define HEIGHT 600
#define WINDOW_TITLE "Chapter 3"

typedef struct {
  float XYZW[4];
  float RGBA[4];
} Vertex;

GLFWwindow* window;
GLuint
  VertexShaderId,
  FragmentShaderId,
  ProgramId,
  VaoId,
  VboId,
  BufferId,
  IndexBufferId[2],
  ActiveIndexBuffer = 0;

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

    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
      ActiveIndexBuffer = (ActiveIndexBuffer == 1 ? 0 : 1);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[ActiveIndexBuffer]);
    }
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

  if (ActiveIndexBuffer == 0) {
    glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_BYTE, NULL);
  } else {
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, NULL);
  }

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
  Vertex Verticies[] = {
    {{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
    // Top
    {{-0.2f, 0.8f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{0.2f, 0.8f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    {{0.0f, 0.8f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}},
    {{0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    // Bottom
    {{-0.2f, -0.8f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    {{0.2f, -0.8f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{0.0f, -0.8f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}},
    {{0.0f, -1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    // Left
    {{-0.8f, -0.2f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{-0.8f, 0.2f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    {{-0.8f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}},
    {{-1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    // Right
    {{0.8f, -0.2f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    {{0.8f, 0.2f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{0.8f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}},
    {{1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}
  };

  GLubyte Indices[] = {
    // Top
    0, 1, 3,
    0, 3, 2,
    3, 1, 4,
    3, 4, 2,
    // Bottom
    0, 5, 7,
    0, 7, 6,
    7, 5, 8,
    7, 8, 6,
    // Left
    0, 9, 11,
    0, 11, 10,
    11, 9, 12,
    11, 12, 10,
    // Right
    0, 13, 15,
    0, 15, 14,
    15, 13, 16,
    15, 16, 14
  };

  GLubyte AlternativeIndices[] = {
    // Outer square border:
    3, 4, 16,
    3, 15, 16,
    15, 16, 8,
    15, 7, 8,
    7, 8, 12,
    7, 11, 12,
    11, 12, 4,
    11, 3, 4,

    // Inner square
    0, 11, 3,
    0, 3, 15,
    0, 15, 7,
    0, 7, 11
  };

  GLenum ErrorCheckValue = glGetError();
  const size_t BufferSize = sizeof(Verticies);
  const size_t VertexSize = sizeof(Verticies[0]);
  const size_t RgbOffset = sizeof(Verticies[0].XYZW);

  glGenBuffers(1, &VboId);

  glGenVertexArrays(1, &VaoId);
  glBindVertexArray(VaoId);

  glBindBuffer(GL_ARRAY_BUFFER, VboId);
  glBufferData(GL_ARRAY_BUFFER, BufferSize, Verticies, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VertexSize, 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)RgbOffset);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glGenBuffers(2, IndexBufferId);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(AlternativeIndices), AlternativeIndices, GL_STATIC_DRAW);

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
  glDeleteBuffers(1, &VboId);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(2, IndexBufferId);

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