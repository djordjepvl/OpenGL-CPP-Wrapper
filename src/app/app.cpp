#include <app.h>

#include <input.h>

#include <shader.h>
#include <texture.h>
#include <VBO.h>
#include <VAO.h>
#include <EBO.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float offsetX = 0.0f;
GLfloat vertices[] = {
    // Coordinates           // Colors           // UVs
    // Front face (Z+)
    -0.5f, -0.5f,  0.5f,     1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // 0
     0.5f, -0.5f,  0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // 1
     0.5f,  0.5f,  0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // 2
    -0.5f,  0.5f,  0.5f,     1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // 3

    // Back face (Z-)
    -0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // 4
     0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // 5
     0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // 6
    -0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // 7

    // Left face (X-)
    -0.5f,  0.5f,  0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // 8
    -0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // 9
    -0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // 10
    -0.5f, -0.5f,  0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // 11

    // Right face (X+)
     0.5f,  0.5f,  0.5f,     1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // 12
     0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // 13
     0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // 14
     0.5f, -0.5f,  0.5f,     1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // 15

    // Top face (Y+)
    -0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // 16
     0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // 17
     0.5f,  0.5f,  0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // 18
    -0.5f,  0.5f,  0.5f,     1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // 19

    // Bottom face (Y-)
    -0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // 20
     0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // 21
     0.5f, -0.5f,  0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // 22
    -0.5f, -0.5f,  0.5f,     1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // 23
};

GLuint indices[] = {
    0, 1, 2,  2, 3, 0,       // Front
    4, 5, 6,  6, 7, 4,       // Back
    8, 9, 10, 10, 11, 8,     // Left
    12, 13, 14, 14, 15, 12,  // Right
    16, 17, 18, 18, 19, 16,  // Top
    20, 21, 22, 22, 23, 20   // Bottom
};
Shader *shader;
VBO *vbo;
VAO *vao;
EBO *ebo;

glm::mat4 model;
glm::mat4 view;
glm::mat4 proj;

Texture *texture;



void App::init() {
    glEnable(GL_DEPTH_TEST);

    shader = new Shader(SHADER_PATH + "default.vert", SHADER_PATH + "default.frag");

    texture = new Texture(TEXTURE_PATH + "paulie.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    texture->Bind();

    vao = new VAO();
    vao->Bind();

    vbo = new VBO(vertices, sizeof(vertices));
    ebo = new EBO(indices, sizeof(indices));
    
    vao->LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    vao->LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao->LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    vao->Unbind();

    vbo->Unbind();
    ebo->Unbind();

    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    proj = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, -1.0f, -5.0f));
    proj = glm::perspective(glm::radians(45.0f), (float)((float)WINDOW_WIDTH/(float)WINDOW_HEIGHT), 0.1f, 100.0f);

    
}

void App::update(float deltaTime, float currentTime) {
    if (Input::isKeyDown(GLFW_KEY_RIGHT)) offsetX += deltaTime;
    if (Input::isKeyDown(GLFW_KEY_LEFT)) offsetX -= deltaTime;

    model = glm::rotate(model, glm::radians(offsetX), glm::vec3(0.0f, 1.0f, 0.0f));
}

void App::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->Use();

    GLint modelLoc = glGetUniformLocation(shader->id, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    GLint viewLoc = glGetUniformLocation(shader->id, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    GLint projLoc = glGetUniformLocation(shader->id, "proj");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

    texture->texUnit(shader, "tex0", 0);
    vao->Bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void App::close() {
    delete shader;
    delete texture;
    delete vao;
    delete vbo;
    delete ebo;
}
