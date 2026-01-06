#include <material.h>

Material::Material(std::shared_ptr<Texture> diffuse){
    diffuseMap = diffuse;
}

void Material::Apply(Shader *shader){
    if (diffuseMap) {
        diffuseMap->assignToShader(shader, "diffuse0");
        diffuseMap->Bind();
    }
}