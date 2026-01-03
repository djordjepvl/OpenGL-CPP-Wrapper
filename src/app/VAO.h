#pragma once

#include <config.h>
#include <VBO.h>

class VAO {
    public:
        GLuint id;

        VAO();
        ~VAO();
        
        void LinkAttrib(VBO *VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
        void Bind();
        void Unbind();
};