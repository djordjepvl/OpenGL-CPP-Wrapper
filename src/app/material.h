#pragma once

#include <config.h>
#include <texture.h>
#include <map>

class Material {
    public:
        std::shared_ptr<Texture> baseColorTexture, metallicRoughnessTexture, normalTexture, occlusionTexture;
        glm::vec4 baseColorFactor = glm::vec4(1.0f);
        float metallicFactor = 1.0f;
        float roughnessFactor = 1.0f;
        
        Material(const std::map<int, std::shared_ptr<Texture>>& textures);

        void Bind(Shader *shader);
        void Unbind(Shader *shader);
};