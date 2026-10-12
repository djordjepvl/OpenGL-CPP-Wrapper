#version 330 core

in vec3 pos;
in vec3 normal;
in vec2 uv;
in vec3 color;

out vec4 FragColor;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normal0;

void main() {
   FragColor = texture(diffuse0, uv);
}