#pragma once

#include <config.h>

class EBO {
    public:
        GLuint id;
        EBO(GLuint* indices, GLsizeiptr size);
        void Bind();
        void Unbind();
        void Delete();
};