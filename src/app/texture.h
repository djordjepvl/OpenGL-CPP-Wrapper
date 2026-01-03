#pragma once

#include <config.h>

#include <shader.h>

class Texture
{
    public:
        GLuint id;
        GLenum type;
        GLuint slot;
        Texture::Texture(const std::string &imagePath, GLenum texType, GLenum slot, GLenum pixelType);
        ~Texture();
        void texUnit(Shader* shader, const char* uniform, GLuint unit);
        void Bind();
        void Unbind();
};