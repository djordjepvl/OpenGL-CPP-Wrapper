#include <camera.h>

#include <input.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 direction) {
    this->position = position;
    this->direction = direction;
    view = glm::lookAt(position, position + direction, up);
    matrix = projection * view;
}

void Camera::update(float deltaTime) {
    yaw += (float)Input::deltaMouseX * sensitivity;
    pitch += (float)Input::deltaMouseY * sensitivity;

    float pitchCap = 80.0f;
    if (pitch > pitchCap)  pitch = pitchCap;
    if (pitch < -pitchCap) pitch = -pitchCap;

    glm::vec3 newDir;
    newDir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newDir.y = sin(glm::radians(pitch));
    newDir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

direction = glm::normalize(newDir);
    
    direction = glm::normalize(newDir);

    float velocity = speed * deltaTime;

    glm::vec3 flatDirection = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));

    if (Input::isKeyDown(GLFW_KEY_W))
        position += flatDirection * velocity;
    if (Input::isKeyDown(GLFW_KEY_S))
        position -= flatDirection * velocity;

    if (Input::isKeyDown(GLFW_KEY_A))
        position -= glm::normalize(glm::cross(flatDirection, up)) * velocity;
    if (Input::isKeyDown(GLFW_KEY_D))
        position += glm::normalize(glm::cross(flatDirection, up)) * velocity;


    if (Input::isKeyDown(GLFW_KEY_SPACE))
        position += up * velocity;

    if (Input::isKeyDown(GLFW_KEY_LEFT_SHIFT))
        position -= up * velocity;
    

    view = glm::lookAt(position, position + direction, up);
    matrix = projection * view;
}
void Camera::setProjection(float FOVdeg, float nearPlane, float farPlane) {
    projection = glm::perspective(glm::radians(FOVdeg), (float)((float)WINDOW_WIDTH/(float)WINDOW_HEIGHT), nearPlane, farPlane);
    matrix = projection * view;
}
