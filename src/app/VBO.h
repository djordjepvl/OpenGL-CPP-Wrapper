#pragma once

#include <config.h>

class VBO {
    public:
        GLuint id;
        VBO(GLfloat* vertices, GLsizeiptr size);
        void Bind();
        void Unbind();
        void Delete();
};