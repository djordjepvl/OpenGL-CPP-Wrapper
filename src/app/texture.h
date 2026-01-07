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
        void init(const unsigned char* data, int width, int height, int numChannels);
        Texture(const unsigned char* data, int width, int height, int numChannels, TextureType texType, GLuint slot);
        Texture(const std::string &imagePath, TextureType texType, GLenum slot);
        ~Texture();
        void Texture::assignToShader(Shader* shader, const std::string &uniform, GLuint slot);
        void Bind(GLuint slot);
        void Unbind();
};