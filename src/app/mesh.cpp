#include <mesh.h>

Mesh::Mesh(std::vector <Vertex> &vertices, std::vector <GLuint> &indices, std::vector <Texture> &texture) {
    this->vertices = vertices;
    this->indices = indices;
    this->texture = texture;
    
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

void Mesh::Draw(Shader *shader, Camera *camera) {
    shader->Use();

    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;
    unsigned int numNormal = 0;

    for (unsigned int i = 0; i < texture.size(); i++) {
        std::string num;
        std::string name;

        if (texture[i].type == DIFFUSE) {
            name = "diffuse";
            num = std::to_string(numDiffuse++);
        } 
        else if (texture[i].type == SPECULAR) {
            name = "specular";
            num = std::to_string(numSpecular++);
        }
        else if (texture[i].type == NORMAL) {
            name = "normal";
            num = std::to_string(numNormal++);
        }

        texture[i].assignToShader(shader, (name + num).c_str());
        texture[i].Bind();
    }
    
    camera->applyProjection(shader, "camMatrix");
    
    vao->Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    vao->Unbind();

    for (unsigned int i = 0; i < texture.size(); i++) texture[i].Unbind();
}