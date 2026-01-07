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

uniform vec3 camPosition;

void main() {
   vec3 N = normalize(normal);
   vec3 L = normalize(-sunDir);
   vec3 V = normalize(camPosition - pos);
   vec3 H = normalize(L + V);
   
   vec4 tex = texture(diffuse0, uv);

   vec3 pbrData = texture(specular0, uv).rgb;
   float roughness = pbrData.g;
   float metalness = pbrData.b;
   

   float diff = max(dot(N, L), 0);

   float shininess = (1.0 - roughness) * 128.0; 
   float spec = pow(max(dot(N, H), 0.0), shininess);

   vec3 ambient = ambientColor * ambientIntensity;
   vec3 diffuse = sunColor * diff * sunIntensity;
   vec3 specular = sunColor * spec * 2;
   vec3 lighting = ambient + diffuse + specular;

   //FragColor = vec4(specular,1.0f);
   FragColor = vec4(tex.rgb * lighting, tex.a);
}