#pragma once

#include <config.h>
#include <VBO.h>

class VAO {
    public:
        GLuint id;
        VAO();
        void LinkVBO(VBO VBO, GLuint layout);
        void Bind();
        void Unbind();
        void Delete();
};