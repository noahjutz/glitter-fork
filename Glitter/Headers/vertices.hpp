#pragma once

float vertices[] = {
    // positions          // colors           // texture coords
    0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.6f, 0.6f, // top right
    0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.6f, 0.4f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.4f, 0.4f, // bottom left
    -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.4f, 0.6f, // top left
    0.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, //
    1.0f,  0.0,   0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, //
    1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, //
    0.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};

unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3, // second triangle
    4, 5, 7, // ...
    5, 6, 7  // ...
};
