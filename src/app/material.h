#pragma once

#include <config.h>
#include <texture.h>
#include <map>

class Material {
    public:
        std::shared_ptr<Texture> diffuseMap, specularMap, normalMap;
        
        Material(const std::map<int, std::shared_ptr<Texture>>& textures);

        void Apply(Shader *shader);
};