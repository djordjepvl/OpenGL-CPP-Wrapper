#include <mesh.h>

Mesh::Mesh(std::vector <Vertex> &vertices, std::vector <GLuint> &indices) {
    this->vertices = vertices;
    this->indices = indices;
    
    vao = new VAO();
    vao->Bind();

    VBO *vbo = new VBO(vertices);
    EBO *ebo = new EBO(indices);
    
    vao->LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    vao->LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    vao->LinkAttrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
    vao->LinkAttrib(vbo, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)(8 * sizeof(float)));
    
    vao->Unbind();
    vbo->Unbind();
    ebo->Unbind();

    delete vbo;
    delete ebo;
}

Mesh::~Mesh() {
    delete vao;
}

void Mesh::Draw() {
    vao->Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}