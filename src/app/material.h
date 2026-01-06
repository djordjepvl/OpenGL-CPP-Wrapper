#pragma once

#include <config.h>
#include <texture.h>

class Material {
    public:
        std::shared_ptr<Texture> diffuseMap;
        
        Material(std::shared_ptr<Texture> diffuse);

        void Apply(Shader *shader);
};