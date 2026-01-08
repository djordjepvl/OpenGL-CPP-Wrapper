#version 330 core

in vec3 pos;
in vec3 normal;
in vec2 uv;
in vec3 color;
in vec4 FragPosLightSpace;

out vec4 FragColor;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normal0;
uniform samplerCube skybox;
uniform sampler2D shadowMap;

uniform vec3 sunDir;
uniform vec3 sunColor;
uniform float sunIntensity;
uniform vec3 ambientColor;
uniform float ambientIntensity;

uniform vec3 camPosition;

float ShadowCalculation(vec4 FragPosLightSpace) {
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    float bias = 0.005;
    return currentDepth - bias > closestDepth  ? 1.0 : 0.0;
}

void main() {
   vec4 tex = texture(diffuse0, uv);
   vec3 pbrData = texture(specular0, uv).rgb;
   float ao = pbrData.r;
   float roughness = pbrData.g;
   float metalness = pbrData.b;

   vec3 N = normalize(normal);
   vec3 L = normalize(-sunDir);
   vec3 V = normalize(camPosition - pos);
   vec3 H = normalize(L + V);

   //reflection calc
   vec3 R = reflect(-V, N);

   vec3 albedo = tex.rgb;

   //fresnel
   float VoN = max(dot(V, N), 0.0);
   vec3 F0 = mix(vec3(0.04), albedo, metalness);
   vec3 F = F0 + (1.0 - F0) * pow(1.0 - VoN, 5.0);

   float diff = max(dot(N, L), 0.0);

   float shininess = mix(1.0, 256.0, pow(1.0 - roughness, 2.0));
   float spec = pow(max(dot(N, H), 0.0), shininess);

   vec3 kd = (1.0 - metalness) * albedo;

   vec3 ambient = ambientColor * ambientIntensity * kd  * ao * (1.0 - F0);
   vec3 diffuse = sunColor * diff * sunIntensity * kd * (1.0 - F);
   vec3 specular = sunColor * spec * sunIntensity * F;


   float mip = roughness * 8.0;
   vec3 envReflection = textureLod(skybox, R, mip).rgb;
   vec3 reflection = envReflection * F;

   float shadow = ShadowCalculation(FragPosLightSpace);
   vec3 directLighting = (diffuse + specular) * (1.0 - shadow);
   vec3 indirectLighting = ambient + reflection;

   FragColor = vec4(directLighting + indirectLighting, tex.a);
}