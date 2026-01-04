#pragma once

#include <config.h>

#include <VAO.h>
#include <EBO.h>
#include <camera.h>
#include <texture.h>

class Mesh {
    public:
        std::vector <Vertex> vertices;
        std::vector <GLuint> indices;
        std::vector <Texture> texture;

        VAO *vao;

        Mesh(std::vector <Vertex> &vertices, std::vector <GLuint> &indices, std::vector <Texture> &texture);
        ~Mesh();
        void Draw(Shader *shader, Camera *camera);
};