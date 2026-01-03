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

    // 2. Rebuild the direction vector using trigonometry
    glm::vec3 newDir;
    newDir.x = cos(glm::radians(yaw));
    newDir.y = 0.0f; // Keep level for now
    newDir.z = sin(glm::radians(yaw));
    
    direction = glm::normalize(newDir); // Ensure length is exactly 1.0

    // 3. Debug the angle and the resulting vector
    std::cout << "Yaw: " << yaw << " | DirX: " << direction.x << " DirZ: " << direction.z << std::endl;
    

    view = glm::lookAt(position, position + direction, up);
}
void Camera::setProjection(float FOVdeg, float nearPlane, float farPlane) {
    projection = glm::perspective(glm::radians(FOVdeg), (float)((float)WINDOW_WIDTH/(float)WINDOW_HEIGHT), nearPlane, farPlane);
}
void Camera::applyProjection(Shader *shader, const char* uniform) {
    glUniformMatrix4fv(glGetUniformLocation(shader->id, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

