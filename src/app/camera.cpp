#include <camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 direction) {
    this->position = position;
    this->direction = direction;
    view = glm::lookAt(position, position + direction, up);
}

void Camera::update(float deltaTime) {
    position.z += deltaTime;
    view = glm::lookAt(position, position + direction, up);
}
void Camera::setProjection(float FOVdeg, float nearPlane, float farPlane) {
    projection = glm::perspective(glm::radians(FOVdeg), (float)((float)WINDOW_WIDTH/(float)WINDOW_HEIGHT), nearPlane, farPlane);
}
void Camera::applyProjection(Shader *shader, const char* uniform) {
    glUniformMatrix4fv(glGetUniformLocation(shader->id, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

