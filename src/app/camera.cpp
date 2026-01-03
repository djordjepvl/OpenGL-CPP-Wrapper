#include <camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 direction) {
    this->position = position;
    this->direction = direction;
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::update() {
    
}
void Camera::setProjection(float FOVdeg, float nearPlane, float farPlane) {
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);
    view = glm::lookAt(position, position + direction, up);
    projection = glm::perspective(glm::radians(FOVdeg), (float)((float)WINDOW_WIDTH/(float)WINDOW_HEIGHT), nearPlane, farPlane);
}
void Camera::applyProjection(Shader *shader, const char* uniform) {
    glUniformMatrix4fv(glGetUniformLocation(shader->id, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

