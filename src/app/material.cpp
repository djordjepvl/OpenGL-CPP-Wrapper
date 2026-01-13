#include <material.h>

Material::Material(const std::map<int, std::shared_ptr<Texture>>& textures){
    for (auto const& [id, tex] : textures) {
        if (!tex) continue;
        if (tex->type == DIFFUSE) baseColorTexture = tex;
        else if (tex->type == SPECULAR) metallicRoughnessTexture = tex;
        else if (tex->type == NORMAL) normalTexture = tex;
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
    
    bindMap(baseColorTexture, "diffuse0");
    bindMap(metallicRoughnessTexture, "specular0");
    bindMap(normalTexture, "normal0");
}

void Material::Unbind(Shader *shader) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
}