#pragma once

float vertices_tri1[] = {
    0.1f, 0.1f, 0.0f, 0.75f, 0.2f, 0.0f, 0.5f, 0.5f, 0.0f,
};

float vertices_tri2[] = {-0.1f, -0.1f, 0.0f, -0.1f, 0.1f,
                         0.0f,  0.1f,  0.0f, 0.0f};

float vertices_tri3[] = {
    // positions         // colors
    0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
    0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top
};

float texCoords[] = {
    0.0f, 0.0f, // lower-left corner
    1.0f, 0.0f, // lower-right corner
    0.5f, 1.0f  // top-center corner
};
