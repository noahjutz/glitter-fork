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
glm::mat4 myLookAt(glm::vec3 pos, glm::vec3 g, glm::vec3 t);

double xMouse = 0.0;
double yMouse = 0.0;
double yLast = 0.0;
double xLast = 0.0;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraDir = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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

  // Load VAOs

  unsigned int VAO[2];
  glGenVertexArrays(2, VAO);
  unsigned int VBO[2];
  glGenBuffers(2, VBO);

  // VAO 1: Cube

  glBindVertexArray(VAO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(2);

  // VAO 2: Light Source Cube

  glBindVertexArray(VAO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Create shader program

  Shader p_material =
      Shader("Glitter/Shaders/default.vert", "Glitter/Shaders/textured.frag");

  p_material.use();
  p_material.setInt("texture1", 0);
  p_material.setInt("texture2", 1);
  p_material.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.3f));
  p_material.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

  Shader p_light = Shader("Glitter/Shaders/default.vert",
                          "Glitter/Shaders/light_source.frag");

  p_light.use();
  p_light.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
  p_material.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

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

  // View, Projection

  glm::mat4 V(1.0f);

  glm::mat4 P = glm::perspective(glm::radians(45.0f),
                                 (float)width / (float)height, 0.1f, 100.0f);

  // Object

  glm::mat4 M(1.0f);
  M = glm::scale(M, glm::vec3(35.0f));

  // Light source

  glm::mat4 M_L(1.0f);
  M_L = glm::translate(M_L, lightPos);
  M_L = glm::scale(M_L, glm::vec3(0.2f));

  // Rendering Loop
  while (glfwWindowShouldClose(mWindow) == false) {
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(mWindow, true);

    processInput(mWindow);

    V = myLookAt(cameraPos, cameraPos + cameraDir, cameraUp);

    float t = glfwGetTime();

    // Background Fill Color
    glClearColor(0.65f, 0.95f, 0.55f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    p_material.use();
    p_material.setMat("view", V);
    p_material.setMat("proj", P);
    p_material.setMat("model", M);
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    p_light.use();
    p_light.setMat("view", V);
    p_light.setMat("proj", P);
    p_light.setMat("model", M_L);
    glBindVertexArray(VAO[1]);
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

  glm::vec3 sideways = glm::normalize(glm::cross(cameraDir, cameraUp));

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos +=
        delta * cameraSpeed * glm::vec3(cameraDir.x, 0.0f, cameraDir.z);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -=
        delta * cameraSpeed * glm::vec3(cameraDir.x, 0.0f, cameraDir.z);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -= delta * cameraSpeed * glm::vec3(sideways.x, 0.0f, sideways.z);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos += delta * cameraSpeed * glm::vec3(sideways.x, 0.0f, sideways.z);
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

glm::mat4 myLookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 t) {
  glm::vec3 g = -glm::normalize(target - pos);
  glm::vec3 r = glm::normalize(glm::cross(t, g));
  glm::vec3 y = glm::cross(g, r);

  glm::mat4 V = glm::mat4(glm::transpose(glm::mat3(r, y, g)));

  glm::mat4 T = glm::translate(glm::mat4(1.0f), -pos);

  return V * T;
}
