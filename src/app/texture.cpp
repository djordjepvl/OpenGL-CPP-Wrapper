#include <texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const std::string &imagePath, GLenum texType, GLenum slot, GLenum pixelType){
    
    stbi_set_flip_vertically_on_load(true);
    
    int imgWidth, imgHeight, imgChannelCnt;
    unsigned char* image = stbi_load(imagePath.c_str(), &imgWidth, &imgHeight, &imgChannelCnt, 0);
    GLenum format;
    if (imgChannelCnt == 1) format = GL_RED;
    else if (imgChannelCnt == 3) format = GL_RGB;
    else if (imgChannelCnt == 4) format = GL_RGBA;
    else {
        std::cout << "Texture: Could not load " << imagePath << "." << std::endl;
    
        // magenta checkerboard
        unsigned char fallback[] = {
            255, 0, 255, 255,   0, 0, 0, 255,
            0, 0, 0, 255,       255, 0, 255, 255
        };

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, fallback);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        return;
    }
    
    glGenTextures(1, &id);
    this->slot = slot;
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(texType, id);


    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(texType, 0, format, imgWidth, imgHeight, 0, format, pixelType, image);
    glGenerateMipmap(texType);

    stbi_image_free(image);
    glBindTexture(texType, 0);
}
Texture::~Texture(){
    glDeleteTextures(1, &id);
}

void Texture::texUnit(Shader* shader, const char* uniform, GLuint unit){
    shader->Use();
    glUniform1i(glGetUniformLocation(shader->id, uniform), unit);
}
void Texture::Bind(){
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
}
void Texture::Unbind(){
    glBindTexture(GL_TEXTURE_2D, 0);
}