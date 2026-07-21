#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;

uniform mat4 trans;

out vec4 col;

void main() {
    col = vec4(aCol, 1.0);
    gl_Position = trans * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
