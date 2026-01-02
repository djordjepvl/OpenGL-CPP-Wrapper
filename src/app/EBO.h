#pragma once

#include <config.h>

class EBO {
    public:
        GLuint id;

        EBO(GLuint* indices, GLsizeiptr size);
        ~EBO();
        
        void Bind();
        void Unbind();
};