#version 330 core

in vec3 pos;
in vec3 normal;
in vec2 uv;
in vec3 color;

out vec4 FragColor;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normal0;

uniform vec3 sunDir;
uniform vec3 sunColor;
uniform float sunIntensity;
uniform vec3 ambientColor;
uniform float ambientIntensity;

void main() {
   vec3 N = normalize(normal);
   vec3 L = normalize(-sunDir);
   vec4 tex = texture(diffuse0, uv);
   
   float brightness = max(dot(N, L), 0) *sunIntensity;
   vec3 lighting = ambientColor * ambientIntensity + (sunColor * brightness);
   FragColor = vec4(tex.rgb * lighting, tex.a);
}