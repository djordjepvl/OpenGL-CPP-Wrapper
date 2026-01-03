#include <app.h>

#include <input.h>

#include <shader.h>
#include <VBO.h>
#include <VAO.h>
#include <EBO.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float offsetX = 0.0f;
GLfloat vertices[] = {
    -0.5f, -0.5f,  0.5f, // 0
     0.5f, -0.5f,  0.5f, // 1
     0.5f,  0.5f,  0.5f, // 2
    -0.5f,  0.5f,  0.5f, // 3
    -0.5f, -0.5f, -0.5f, // 4
     0.5f, -0.5f, -0.5f, // 5
     0.5f,  0.5f, -0.5f, // 6
    -0.5f,  0.5f, -0.5f  // 7
};

GLuint indices[] = {
    0, 1, 2, 2, 3, 0, // Front
    1, 5, 6, 6, 2, 1, // Right
    7, 6, 5, 5, 4, 7, // Back
    4, 0, 3, 3, 7, 4, // Left
    3, 2, 6, 6, 7, 3, // Top
    4, 5, 1, 1, 0, 4  // Bottom
};

Shader *shader;
VBO *vbo;
VAO *vao;
EBO *ebo;

glm::mat4 model;
glm::mat4 view;
glm::mat4 proj;

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
    glClear(GL_COLOR_BUFFER_BIT);
    shader->Use();
    
    GLint uniLoc = glGetUniformLocation(shader->id, "offset");

    GLint modelLoc = glGetUniformLocation(shader->id, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    GLint viewLoc = glGetUniformLocation(shader->id, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    GLint projLoc = glGetUniformLocation(shader->id, "proj");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

    glUniform3f(uniLoc, offsetX, 0.0f, 0.0f);

    vao->Bind();
    glDrawElements(GL_TRIANGLES, 32, GL_UNSIGNED_INT, 0);
}

void App::close() {
    delete shader;

    delete vao;
    delete vbo;
    delete ebo;
}
