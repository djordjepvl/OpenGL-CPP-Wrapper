#pragma once

#include <config.h>

class EBO {
    public:
        GLuint id;

        EBO(std::vector<GLuint> &indices);
        ~EBO();
        
        void Bind();
        void Unbind();
};