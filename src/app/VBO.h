#pragma once

#include <config.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 UV;
    glm::vec3 color;
};

class VBO {
    public:
        GLuint id;
        
        VBO(std::vector<Vertex> &vertices);
        ~VBO();

        void Bind();
        void Unbind();
};