// Local Headers
#include "glitter.hpp"
#include "vertices.hpp"

// System Headers
#include <GLFW/glfw3.h>
#include <glad/glad.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main() {

  // Load GLFW and Create a Window
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

  // Check for Valid Context
  if (mWindow == nullptr) {
    fprintf(stderr, "Failed to Create OpenGL Context");
    return EXIT_FAILURE;
  }

  // Create Context and Load OpenGL Functions
  glfwMakeContextCurrent(mWindow);
  gladLoadGL();
  fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

  glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);

  // Load triangle 1

  unsigned int VAO1;
  glGenVertexArrays(1, &VAO1);
  glBindVertexArray(VAO1);

  unsigned int VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_tri1), vertices_tri1,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        (void *)0); // layout (loaction = 0) in vec3 aPos
  glEnableVertexAttribArray(0);

  // Load triangle 2

  unsigned int VAO2;
  glGenVertexArrays(1, &VAO2);
  glBindVertexArray(VAO2);

  unsigned int VBO2;
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_tri2), vertices_tri2,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Create vertex shader

  const char *vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "uniform float uniOff;\n"
      "out vec4 vertexColor;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = vec4(aPos.x + uniOff, aPos.y, aPos.z, 1.0);\n"
      "   vertexColor = vec4(1.0, 0.5, 0.5, 1.0);\n"
      "}\0";

  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  int success;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char compileLog[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, compileLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl
              << compileLog << std::endl;
  }

  // Create shader program 1

  const char *fragmentShader1Source = "#version 330 core\n"
                                      "out vec4 FragColor;\n"
                                      "in vec4 vertexColor;\n"
                                      "void main() {\n"
                                      "FragColor = vertexColor;\n"
                                      "}\0";

  unsigned int fragmentShader1;
  fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader1, 1, &fragmentShader1Source, NULL);
  glCompileShader(fragmentShader1);

  glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
  if (!success) {
    char compileLog[512];
    glGetShaderInfoLog(fragmentShader1, 512, NULL, compileLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl
              << compileLog << std::endl;
  }

  unsigned int shaderProgram1;
  shaderProgram1 = glCreateProgram();
  glAttachShader(shaderProgram1, vertexShader);
  glAttachShader(shaderProgram1, fragmentShader1);
  glLinkProgram(shaderProgram1);

  glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
  if (!success) {
    char linkLog[512];
    glGetProgramInfoLog(shaderProgram1, 512, NULL, linkLog);
    std::cout << "ERROR::PROGRAM::COMPILATION_FAILED" << std::endl
              << linkLog << std::endl;
  }

  // Create shader program 2

  const char *fragmentShader2Source = "#version 330 core\n"
                                      "out vec4 FragColor;\n"
                                      "uniform vec4 uniCol;\n"
                                      "void main() {\n"
                                      "FragColor = uniCol;\n"
                                      "}\0";

  unsigned int fragmentShader2;
  fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader2, 1, &fragmentShader2Source, NULL);
  glCompileShader(fragmentShader2);

  glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
  if (!success) {
    char compileLog[512];
    glGetShaderInfoLog(fragmentShader2, 512, NULL, compileLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl
              << compileLog << std::endl;
  }

  unsigned int shaderProgram2;
  shaderProgram2 = glCreateProgram();
  glAttachShader(shaderProgram2, vertexShader);
  glAttachShader(shaderProgram2, fragmentShader2);
  glLinkProgram(shaderProgram2);

  glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
  if (!success) {
    char linkLog[512];
    glGetProgramInfoLog(shaderProgram2, 512, NULL, linkLog);
    std::cout << "ERROR::PROGRAM::COMPILATION_FAILED" << std::endl
              << linkLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader1);
  glDeleteShader(fragmentShader2);

  // Rendering Loop
  while (glfwWindowShouldClose(mWindow) == false) {
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(mWindow, true);

    float t = glfwGetTime();

    // Background Fill Color
    glClearColor(0.65f, 0.95f, 0.55f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO1);
    glUseProgram(shaderProgram1);

    float offset = sin(t);
    int vertexOffsetLocation = glGetUniformLocation(shaderProgram1, "uniOff");
    glUniform1f(vertexOffsetLocation, offset);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(VAO2);
    glUseProgram(shaderProgram2);

    // Uniform fg color

    float r = (sin(t) + 1) / 2;
    float g = (cos(t) + 1) / 2;
    int vertexColorLocation = glGetUniformLocation(shaderProgram2, "uniCol");
    glUniform4f(vertexColorLocation, r, g, 0.0f, 0.0f);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Flip Buffers and Draw
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
  }
  glfwTerminate();
  return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow *, int width, int height) {
  glViewport(0, 0, width, height);
}
