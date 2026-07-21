#pragma once

float vertices_tri1[] = {
    0.1f, 0.1f, 0.0f, 0.75f, 0.2f, 0.0f, 0.5f, 0.5f, 0.0f,
};

float vertices_tri2[] = {-0.1f, -0.1f, 0.0f, -0.1f, 0.1f,
                         0.0f,  0.1f,  0.0f, 0.0f};

float vertices[] = {
    // positions          // colors           // texture coords
    0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
    0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
};
