#version 330 core

out vec4 FragColor;

in vec4 col;
in vec4 pos;
in vec2 tex;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
    vec4 texColor = mix(texture(texture1, tex), texture(texture2, vec2(-tex.x, tex.y)), 0.0);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec4 res = vec4(ambient, 1.0) * texColor;
    FragColor = res;
}
