#include <VBO.h>



VBO::VBO(std::vector<Vertex> &vertices) {

    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}
VBO::~VBO() {
    glDeleteBuffers(1, &id);
}

void VBO::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}
void VBO::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}