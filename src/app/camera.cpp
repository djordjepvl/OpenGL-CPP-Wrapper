#include <camera.h>

#include <input.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 direction) {
    this->position = position;
    this->direction = direction;
    view = glm::lookAt(position, position + direction, up);
}

void Camera::update(float deltaTime) {
    yaw += (float)Input::deltaMouseX * sensitivity;
    pitch += (float)Input::deltaMouseY * sensitivity;

    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 newDir;
    newDir.x = cos(glm::radians(yaw));
    newDir.y = sin(glm::radians(pitch));
    newDir.z = sin(glm::radians(yaw));
    
    direction = glm::normalize(newDir);

    float velocity = speed * deltaTime;

    if (Input::isKeyDown(GLFW_KEY_W))
        position += direction * velocity;
    if (Input::isKeyDown(GLFW_KEY_S))
        position -= direction * velocity;

    if (Input::isKeyDown(GLFW_KEY_A))
        position -= glm::normalize(glm::cross(direction, up)) * velocity;
    if (Input::isKeyDown(GLFW_KEY_D))
        position += glm::normalize(glm::cross(direction, up)) * velocity;


    if (Input::isKeyDown(GLFW_KEY_SPACE))
        position += up * velocity;

    if (Input::isKeyDown(GLFW_KEY_LEFT_SHIFT))
        position -= up * velocity;
    

    view = glm::lookAt(position, position + direction, up);
}
void Camera::setProjection(float FOVdeg, float nearPlane, float farPlane) {
    projection = glm::perspective(glm::radians(FOVdeg), (float)((float)WINDOW_WIDTH/(float)WINDOW_HEIGHT), nearPlane, farPlane);
}
void Camera::applyProjection(Shader *shader, const char* uniform) {
    glUniformMatrix4fv(glGetUniformLocation(shader->id, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

