#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;
layout(location = 2) in vec2 aTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec4 col;
out vec4 pos;
out vec2 tex;

void main() {
    col = vec4(aCol, 1.0);
    tex = aTex;

    pos = proj * view * model * vec4(aPos, 1.0);
    gl_Position = pos;
}
