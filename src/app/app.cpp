#include <app.h>

#include <shader.h>
#include <VBO.h>
#include <VAO.h>
#include <EBO.h>

// Vertices coordinates
GLfloat vertices[] =
{
    -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
    0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
    0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
    -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
    0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
    0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
};

// Indices for vertices order
GLuint indices[] =
{
    0, 3, 5, // Lower left triangle
    3, 2, 4, // Upper triangle
    5, 4, 1 // Lower right triangle
};

Shader *shader;
VBO *vbo;
VAO *vao;
EBO *ebo;

void App::init() {

    shader = new Shader(SHADER_PATH + "default.vert", SHADER_PATH + "default.frag");

    vao = new VAO();
    vao->Bind();

    vbo = new VBO(vertices, sizeof(vertices));
    ebo = new EBO(indices, sizeof(indices));
    
    vao->LinkVBO(vbo, 0);
    vao->Unbind();

    vbo->Unbind();
    ebo->Unbind();
}

void App::input(int key, int action) {
    
}

void App::update() {
    
}

void App::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    shader->Use();
    vao->Bind();
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
}

void App::close() {
    delete shader;

    delete vao;
    delete vbo;
    delete ebo;
}
