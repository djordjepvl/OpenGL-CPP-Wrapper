#pragma once

#include <config.h>

#include <mesh.h>
#include <texture.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Object3D {
    public:
        glm::vec3 position = glm::vec3(0.0f);
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f);

        std::vector<Mesh*> meshes;
        Texture* atlasTexture = nullptr;

        Object3D(std::vector<Mesh*> &meshes, Texture *atlasTexture);
        Object3D(std::string &stbFilePath, Texture *atlasTexture);
        ~Object3D();

        void Draw(Shader *shader, Camera *camera);
        glm::mat4 getModelMatrix();
};