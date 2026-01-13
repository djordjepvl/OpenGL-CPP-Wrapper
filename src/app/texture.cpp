#include <texture.h>

#include <stb_image.h>

void Texture::init(const unsigned char* data, int width, int height, int numChannels) {
    this->glType = GL_TEXTURE_2D;

    GLenum dataFormat = (numChannels == 4) ? GL_RGBA : (numChannels == 3) ? GL_RGB : GL_RED;
    GLint internalFormat = dataFormat;
    
    if (type == DIFFUSE) internalFormat = (numChannels == 4) ? GL_SRGB_ALPHA : GL_SRGB;
    else if (type == SPECULAR) internalFormat = GL_RGB;
    else if (type == NORMAL) internalFormat = dataFormat;

    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(glType, id);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(glType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(glType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(glType, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(glType);
    glBindTexture(glType, 0);
}

Texture::Texture(const unsigned char* data, int width, int height, int numChannels, TextureType texType, GLuint slot) {
    this->type = texType;
    init(data, width, height, numChannels);
}

Texture::Texture(const std::string& path, TextureType texType, GLuint slot) {
    this->type = texType;

    if (texType == DIRECTORY) {
        this->glType = GL_TEXTURE_CUBE_MAP;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);

        std::vector<std::string> faces = {
            "E.jpg", "W.jpg", "U.jpg", "D.jpg", "N.jpg", "S.jpg"
        };

        int w, h, c;
        stbi_set_flip_vertically_on_load(false);

        for (unsigned int i = 0; i < faces.size(); i++) {
            unsigned char* data = stbi_load((path + "/" + faces[i]).c_str(), &w, &h, &c, 0);
            if (data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
        }

        glTexParameteri(glType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(glType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(glType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(glType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(glType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        return; 
    }

    stbi_set_flip_vertically_on_load(true);
    int w, h, c;
    unsigned char* image = stbi_load(path.c_str(), &w, &h, &c, 0);

    if (image) {
        init(image, w, h, c);
        stbi_image_free(image);
    } else {
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

void Texture::assignToShader(Shader* shader, const std::string &uniform, GLuint slot){
    shader->Use();
    glUniform1i(glGetUniformLocation(shader->id, uniform.c_str()), slot);
}
void Texture::Bind(GLuint slot){
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(glType, id);
}
void Texture::Unbind(){
    glBindTexture(glType, 0);
}

std::shared_ptr<Texture> Texture::Create1x1(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    unsigned char data[] = { r, g, b, a };
    
    auto tex = std::make_shared<Texture>(); 
    
    glGenTextures(1, &tex->id);
    glBindTexture(GL_TEXTURE_2D, tex->id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);
    return tex;
}
void Texture::initDefaultTextures() {
    Texture::White = Texture::Create1x1(255, 255, 255, 255);
    Texture::Normal = Texture::Create1x1(128, 128, 255, 255);
    Texture::Black = Texture::Create1x1(0,0,0,255);
}