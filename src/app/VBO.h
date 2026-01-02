#pragma once

#include <config.h>

class VBO {
    public:
        GLuint id;
        
        VBO(GLfloat* vertices, GLsizeiptr size);
        ~VBO();

        void Bind();
        void Unbind();
};