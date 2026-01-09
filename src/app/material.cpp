#include <material.h>

Material::Material(const std::map<int, std::shared_ptr<Texture>>& textures){
    for (auto const& [id, tex] : textures) {
        if (!tex) continue;
        if (tex->type == DIFFUSE) diffuseMap = tex;
        else if (tex->type == SPECULAR) specularMap = tex;
        else if (tex->type == NORMAL) normalMap = tex;
    }
}

void Material::Bind(Shader *shader){
    int currSlot = 0;
    auto bindMap = [&](std::shared_ptr<Texture> map, const std::string &str) {
        if (!map) return;
        map->assignToShader(shader, str, currSlot);
        map->Bind(currSlot);
        currSlot++;
    };
    
    bindMap(diffuseMap, "diffuse0");
    bindMap(specularMap, "specular0");
    bindMap(normalMap, "normal0");
}

void Material::Unbind(Shader *shader) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
}