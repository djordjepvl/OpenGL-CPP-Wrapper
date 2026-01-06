#pragma once

#include <config.h>

#include <VAO.h>
#include <EBO.h>
#include <material.h>

class Mesh {
    public:
        std::vector <Vertex> vertices;
        std::vector <GLuint> indices;
        std::shared_ptr<Material> material;

        VAO *vao;

        Mesh(std::vector <Vertex> &vertices, std::vector <GLuint> &indices, std::shared_ptr<Material> material = nullptr);
        ~Mesh();
        void Draw(Shader *shader);
};