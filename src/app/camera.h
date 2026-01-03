#pragma once

#include <config.h>
#include <shader.h>

class Camera {
    public:
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        float speed = 0.1f;
        float sensitivity = 100.0f;

        Camera(glm::vec3 position, glm::vec3 direction);

        void update();
        void setProjection(float FOVdeg, float nearPlane, float farPlane);
        void applyProjection(Shader *shader, const char* uniform);
    private:
        glm::mat4 view;
        glm::mat4 projection;
};