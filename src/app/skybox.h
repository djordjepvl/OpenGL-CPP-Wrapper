#pragma once

#include <config.h>

#include <texture.h>

class Skybox {
    public:
        std::shared_ptr<Texture> cubemap;
        unsigned int VAO, VBO;

        Skybox(std::string folderPath);
        void Draw(Shader *skyboxShader, Camera *camera);
    private:
        void setupMesh();
};