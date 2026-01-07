#include <material.h>

Material::Material(const std::map<int, std::shared_ptr<Texture>>& textures){
    for (auto const& [id, tex] : textures) {
        if (!tex) continue;
        if (tex->type == DIFFUSE) diffuseMap = tex;
        else if (tex->type == SPECULAR) specularMap = tex;
        else if (tex->type == NORMAL) normalMap = tex;
    }
}

void Material::Apply(Shader *shader){
    int currSlot = 0;
    auto bindMap = [&](std::shared_ptr<Texture> map, const std::string &str) {
        if (!map) return;
        map->assignToShader(shader, str, currSlot);
        map->Bind(currSlot);
        currSlot++;
    };
    
    bindMap(diffuseMap, "diffuse0");
}