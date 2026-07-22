#version 330 core

out vec4 FragColor;

in vec4 col;
in vec4 pos;
in vec2 tex;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    FragColor = texture(texture1, tex);
}
