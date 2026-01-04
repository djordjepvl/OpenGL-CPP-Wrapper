#pragma once

#include <config.h>
#include <shader.h>

class Camera {
    public:
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        float speed = 10.0f;
        float sensitivity = 0.1f;
        
        float yaw = -90.0f;
        float pitch = 0.0f;

        Camera(glm::vec3 position, glm::vec3 direction);

        void update(float deltaTime);
        void setProjection(float FOVdeg, float nearPlane, float farPlane);
        glm::mat4 getCamMatrix();
};