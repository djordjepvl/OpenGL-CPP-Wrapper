#include <material.h>

std::shared_ptr<Texture> White;
std::shared_ptr<Texture> Normal;
std::shared_ptr<Texture> Black;

Material::Material(
    const std::map<int, std::shared_ptr<Texture>>& textures,
    const glm::vec4& baseColorFactor,
    float metallicFactor,
    float roughnessFactor
){
    this->baseColorFactor = baseColorFactor;
    this->metallicFactor = metallicFactor;
    this->roughnessFactor = roughnessFactor;
    
    for (auto const& [id, tex] : textures) {
        if (!tex) continue;
        if (tex->type == DIFFUSE) baseColorTexture = tex;
        else if (tex->type == SPECULAR) metallicRoughnessTexture = tex;
        else if (tex->type == NORMAL) normalTexture = tex;
    }
}

void Material::Bind(Shader *shader){
    int currSlot = 0;
    auto bindMap = [&](std::shared_ptr<Texture> map, const std::string &str, std::shared_ptr<Texture> fallback) {
        std::shared_ptr<Texture> toBind = map ? map : fallback;
        
        if (toBind) {
            toBind->assignToShader(shader, str, currSlot);
            toBind->Bind(currSlot);
            currSlot++;
        }
    };
    
    bindMap(baseColorTexture, "uBaseColorTexture", Texture::White);
    bindMap(metallicRoughnessTexture, "uMetallicRoughnessTexture", Texture::White);
    bindMap(normalTexture, "uNormalTexture", Texture::Normal);
    
    shader->setVec4("uBaseColorFactor", baseColorFactor);
    shader->setFloat("uMetallicFactor", metallicFactor);
    shader->setFloat("uRoughnessFactor", roughnessFactor);
}

void Material::Unbind(Shader *shader) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
}