#include <texture.h>

#include <stb_image.h>

// 1. The Helper (The only place where OpenGL calls live)
void Texture::init(const unsigned char* data, int width, int height, int numChannels) {
    this->glType = GL_TEXTURE_2D;

    GLenum dataFormat = (numChannels == 4) ? GL_RGBA : (numChannels == 3) ? GL_RGB : GL_RED;
    GLint internalFormat = dataFormat;
    
    if (type == DIFFUSE) internalFormat = (numChannels == 4) ? GL_RGBA : GL_RGB;
    else if (type == SPECULAR) internalFormat = GL_RED;
    else if (type == NORMAL) internalFormat = dataFormat;

    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(glType, id);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Fixes skewed GLB textures

    glTexParameteri(glType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(glType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTexImage2D(glType, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(glType);
    glBindTexture(glType, 0);
}

// 2. Memory Constructor
Texture::Texture(const unsigned char* data, int width, int height, int numChannels, TextureType texType, GLuint slot) {
    this->type = texType;
    this->slot = slot;
    init(data, width, height, numChannels);
}

// 3. File Constructor
Texture::Texture(const std::string& imagePath, TextureType texType, GLuint slot) {
    this->type = texType;
    this->slot = slot;

    if (texType == DIRECTORY) {
        this->glType = GL_TEXTURE_CUBE_MAP;
        return; 
    }

    stbi_set_flip_vertically_on_load(true);
    int w, h, c;
    unsigned char* image = stbi_load(imagePath.c_str(), &w, &h, &c, 0);

    if (image) {
        init(image, w, h, c);
        stbi_image_free(image);
    } else {
        // Fallback logic...
        unsigned char fallback[] = { 255, 0, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 0, 255, 255 };
        this->glType = GL_TEXTURE_2D;
        glGenTextures(1, &id);
        glBindTexture(glType, id);
        glTexImage2D(glType, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, fallback);
        glGenerateMipmap(glType);
    }
}
Texture::~Texture(){
    glDeleteTextures(1, &id);
}

void Texture::assignToShader(Shader* shader, const char* uniform){
    shader->Use();
    glUniform1i(glGetUniformLocation(shader->id, uniform), slot);
}
void Texture::Bind(){
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(glType, id);
}
void Texture::Unbind(){
    glBindTexture(glType, 0);
}