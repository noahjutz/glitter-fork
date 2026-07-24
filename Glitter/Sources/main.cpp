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
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);

double xMouse = 0.0;
double yMouse = 0.0;
double yLast = 0.0;
double xLast = 0.0;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraDir = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float lastProcessInput = 0.0f;

int main() {

  // Load GLFW and Create a Window
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
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
  glfwSetCursorPosCallback(mWindow, mouse_callback);
  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glEnable(GL_DEPTH_TEST);
  glLineWidth(5);

  // Load mesh

  unsigned int VAO[2];
  glGenVertexArrays(2, VAO);
  unsigned int VBO[2];
  glGenBuffers(2, VBO);

  glBindVertexArray(VAO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(2);

  // Create shader program

  Shader p1 =
      Shader("Glitter/Shaders/default.vert", "Glitter/Shaders/textured.frag");

  p1.use();
  p1.setInt("texture1", 0);
  p1.setInt("texture2", 1);

  Shader p2 =
      Shader("Glitter/Shaders/default.vert", "Glitter/Shaders/default.frag");

  // Textures

  stbi_set_flip_vertically_on_load(true);

  unsigned int textures[2];
  glGenTextures(2, textures);

  // Texture 1: Container

  int width, height, nrChannels;
  unsigned char *data = stbi_load("Glitter/Resources/container.jpg", &width,
                                  &height, &nrChannels, 0);
  if (data == nullptr) {
    std::cout << "container.jpg not found" << std::endl;
    return 1;
  }

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  // Texture 2: Awesomeface

  data = stbi_load("Glitter/Resources/awesomeface.png", &width, &height,
                   &nrChannels, 0);
  if (data == nullptr) {
    std::cout << "awesomeface.png not found" << std::endl;
    return 1;
  }

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textures[1]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  // Transformation matrices

  glm::mat4 M(1.0f);
  M = glm::rotate(M, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));

  glm::mat4 V(1.0f);

  glm::mat4 P = glm::perspective(glm::radians(45.0f),
                                 (float)width / (float)height, 0.1f, 100.0f);

  // Rendering Loop
  while (glfwWindowShouldClose(mWindow) == false) {
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(mWindow, true);

    processInput(mWindow);

    V = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);

    float t = glfwGetTime();

    // Background Fill Color
    glClearColor(0.65f, 0.95f, 0.55f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    p1.use();
    p1.setMat("view", V);
    p1.setMat("proj", P);
    p1.setMat("model", M);

    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

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

void processInput(GLFWwindow *window) {
  const float cameraSpeed = 5;
  float t = glfwGetTime();
  float delta = t - lastProcessInput;
  lastProcessInput = t;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += delta * cameraSpeed * cameraDir;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= delta * cameraSpeed * cameraDir;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -=
        glm::normalize(glm::cross(cameraDir, cameraUp)) * delta * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos +=
        glm::normalize(glm::cross(cameraDir, cameraUp)) * delta * cameraSpeed;
}

void mouse_callback(GLFWwindow *, double xpos, double ypos) {
  const float sensitivity = 0.005f;

  double yDelta = yLast - ypos;
  double xDelta = xpos - xLast;
  yLast = ypos;
  xLast = xpos;

  xMouse = xMouse + xDelta * sensitivity;
  yMouse = std::clamp(yMouse + yDelta * sensitivity, -(M_PI_2 - 0.1),
                      (M_PI_2 - 0.1));

  cameraDir.x = cos(xMouse) * cos(yMouse);
  cameraDir.y = sin(yMouse);
  cameraDir.z = sin(xMouse) * cos(yMouse);
  cameraDir = normalize(cameraDir);
}
