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
        Texture() = default;
        Texture(const unsigned char* data, int width, int height, int numChannels, TextureType texType, GLuint slot);
        Texture(const std::string &imagePath, TextureType texType, GLenum slot);
        ~Texture();
        void init(const unsigned char* data, int width, int height, int numChannels);
        void assignToShader(Shader* shader, const std::string &uniform, GLuint slot);
        void Bind(GLuint slot);
        void Unbind();

        static std::shared_ptr<Texture> White;   // for baseColor
        static std::shared_ptr<Texture> Normal;  // for normals (0.5, 0.5, 1.0)
        static std::shared_ptr<Texture> Black;   // for AO/Metallic/Roughness if needed
        static std::shared_ptr<Texture> Create1x1(unsigned char r, unsigned char g, unsigned char b, unsigned char a, TextureType type);
        static void initDefaultTextures();
};