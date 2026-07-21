#version 330 core

out vec4 FragColor;

in vec4 col;
in vec4 pos;
in vec2 tex;

uniform sampler2D uniTex;

void main() {
    FragColor = texture(uniTex, tex);
}
