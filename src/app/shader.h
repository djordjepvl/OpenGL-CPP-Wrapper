#pragma once

#include <config.h>

#include <camera.h>

class Shader
{
    public:
        GLuint id;
        Shader(const std::string &vertexFile, const std::string &fragmentFile);
        ~Shader();
        void Use();
        void setMat4(const char* uniform, glm::mat4 matrix);
};
