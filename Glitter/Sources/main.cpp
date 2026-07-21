// Local Headers
#include "glitter.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "shader.hpp"
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_tri3), vertices_tri3,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)0); // layout (loaction = 0) in vec3 aPos
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

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

  // Create shader program

  Shader p1 =
      Shader("Glitter/Shaders/myvert.vert", "Glitter/Shaders/myfrag.frag");

  glm::mat4 trans = glm::mat4(1.0);

  // Rendering Loop
  while (glfwWindowShouldClose(mWindow) == false) {
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(mWindow, true);

    float t = glfwGetTime();

    glm::mat4 R = glm::mat4_cast(glm::angleAxis(
        (1.0f / 10.0f) * t * glm::two_pi<float>(), glm::vec3(1.0, 0.0, 0.0)));

    R[3][0] = sin(t) / 2;
    R[3][1] = cos(t) / 2;

    // Background Fill Color
    glClearColor(0.65f, 0.95f, 0.55f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO1);
    p1.use();
    p1.setMat("trans", R);
    float offset = sin(t) * 0.5;
    p1.setFloat("uniOff", offset);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // glBindVertexArray(VAO2);
    // float r = (sin(t) + 1) / 2;
    // float g = (cos(t) + 1) / 2;
    // glDrawArrays(GL_TRIANGLES, 0, 3);

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
