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
    view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
    proj = glm::perspective(glm::radians(45.0f), (float)(WINDOW_WIDTH/WINDOW_HEIGHT), 0.1f, 100.0f);
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
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
}

void App::close() {
    delete shader;

    delete vao;
    delete vbo;
    delete ebo;
}
