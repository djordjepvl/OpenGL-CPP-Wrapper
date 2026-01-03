#include <texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const std::string &imagePath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType){
    
    stbi_set_flip_vertically_on_load(true);
    
    int imgWidth, imgHeight, imgChannelCnt;
    unsigned char* image = stbi_load(imagePath.c_str(), &imgWidth, &imgHeight, &imgChannelCnt, 0);
    if (!image) {
        std::cout << "Could not load " << imagePath << std::endl;
        //TODO: load a default texture here instead of returning
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