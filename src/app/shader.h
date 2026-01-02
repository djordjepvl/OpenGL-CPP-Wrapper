#pragma once

#include <config.h>

class Shader
{
    public:
        GLuint id;
        Shader(const std::string &vertexFile, const std::string &fragmentFile);
        void Use();
        void Delete();
};
