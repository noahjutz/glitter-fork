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

int main(int argc, char *argv[]) {

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

  // Load vertices
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Load vertex shader

  const char *vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
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
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl << compileLog << std::endl;
  }

  // Rendering Loop
  while (glfwWindowShouldClose(mWindow) == false) {
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(mWindow, true);

    // Background Fill Color
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

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
