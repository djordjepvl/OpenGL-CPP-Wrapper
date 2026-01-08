#pragma once

#include <config.h>

#include <camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShadowMap {
    public:
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 matrix = glm::mat4(1.0f);

        unsigned int fbo;          // The Framebuffer object
        unsigned int depthMap;     // The actual Texture ID
        unsigned int resolution;   // e.g., 2048

        float orthoSize = 20.0f;
        float nearPlane = 1.0f;
        float farPlane = 100.0f;

        void init(int resolution);
        void update(glm::vec3 sunDir, Camera *camera);
        void bind();
        void unbind();
};