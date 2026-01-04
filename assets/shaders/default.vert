#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aColor;

out vec3 pos;
out vec3 normal;
out vec2 uv;
out vec3 color;

uniform mat4 model;
uniform mat4 camMatrix;

void main() {
    pos = vec3(model * vec4(aPos, 1.0f));
    normal = aNormal;
    uv = aUV;
    color = aColor;
    
    gl_Position = camMatrix * model * vec4(aPos, 1.0);
}