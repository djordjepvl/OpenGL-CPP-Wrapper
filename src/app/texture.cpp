#include <texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const std::string& imagePath, TextureType texType, GLuint slot) {
    
    this->type = texType;
    this->slot = slot;
    
    if (texType == DIRECTORY) {
        std::cout << "Texture Error: Use Cubemap loader for DIRECTORY types." << std::endl;
        this->glType = GL_TEXTURE_CUBE_MAP;
        //TODO: fallback here
        return; 
    }

    this->glType = GL_TEXTURE_2D;

    stbi_set_flip_vertically_on_load(true);
    int imgWidth, imgHeight, imgChannelCnt;
    unsigned char* image = stbi_load(imagePath.c_str(), &imgWidth, &imgHeight, &imgChannelCnt, 0);

    if (!image || imgChannelCnt < 1 || imgChannelCnt > 4) {
        std::cout << "Texture: Failed to load " << imagePath << " or invalid channels (" << imgChannelCnt << ")." << std::endl;
        
        unsigned char fallback[] = { 255, 0, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 0, 255, 255 };
        glGenTextures(1, &id);
        glBindTexture(glType, id);
        glTexImage2D(glType, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, fallback);
        glGenerateMipmap(glType);
        return;
    }

    GLenum dataFormat;
    if (imgChannelCnt == 4) dataFormat = GL_RGBA;
    else if (imgChannelCnt == 3) dataFormat = GL_RGB;
    else dataFormat = GL_RED;

    GLint internalFormat = dataFormat;
    if (type == DIFFUSE) internalFormat = (imgChannelCnt == 4) ? GL_RGBA : GL_RGB;
    else if (type == SPECULAR) internalFormat = GL_RED; 
    else if (type == NORMAL) internalFormat = dataFormat; 

    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(glType, id);

    glTexParameteri(glType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(glType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTexImage2D(glType, 0, internalFormat, imgWidth, imgHeight, 0, dataFormat, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(glType);

    stbi_image_free(image);
    glBindTexture(glType, 0);
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