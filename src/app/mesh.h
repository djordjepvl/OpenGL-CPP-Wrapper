#pragma once

#include <config.h>

#include <VAO.h>
#include <EBO.h>
#include <camera.h>

class Mesh {
    public:
        std::vector <Vertex> vertices;
        std::vector <GLuint> indices;

        VAO *vao;

        Mesh(std::vector <Vertex> &vertices, std::vector <GLuint> &indices);
        ~Mesh();
        void Draw();
};