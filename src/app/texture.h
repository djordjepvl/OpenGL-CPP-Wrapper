#pragma once

#include <config.h>

#include <shader.h>

enum TextureType {
    DIFFUSE,
    SPECULAR,
    NORMAL,
    DIRECTORY
};

class Texture
{
    public:
        GLuint id;
        TextureType type;
        GLenum glType;
        GLuint slot;
        Texture::Texture(const std::string &imagePath, TextureType texType, GLenum slot);
        ~Texture();
        void Texture::assignToShader(Shader* shader, const char* uniform);
        void Bind();
        void Unbind();
};