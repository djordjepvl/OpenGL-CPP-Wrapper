#pragma once

#include <config.h>

#include <mesh.h>
#include <texture.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Object3D {
    public:
        glm::vec3 position = glm::vec3(0.0f);
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f);

        std::vector<std::shared_ptr<Mesh>> meshes;

        Object3D(std::vector<std::shared_ptr<Mesh>> &meshes);
        static std::shared_ptr<Object3D> LoadGLB(
            const std::string& path, 
            glm::vec3 pos = {0.0f, 0.0f, 0.0f}, // Initializer list default
            float s = 1.0f, 
            glm::vec3 rot = {0.0f, 0.0f, 0.0f}
        );

        void Draw(Shader *shader, Camera *camera);
        glm::mat4 getModelMatrix();

        inline void Move(float x, float y, float z) { position += glm::vec3(x,y,z); }
        inline void Move(glm::vec3 vec) { position += vec; }
        inline void Scale (float x, float y, float z) { scale *= glm::vec3(x,y,z); }
        inline void Scale (glm::vec3 vec) { scale *= vec; }
        inline void Rotate(float x, float y, float z) { rotation = glm::quat(glm::radians(glm::vec3(x,y,z))) * rotation; }
        inline void Rotate(glm::vec3 vec) { rotation = glm::quat(glm::radians(vec)) * rotation; }
        
        inline void SetPosition(float x, float y, float z) { position = glm::vec3(x,y,z); }
        inline void SetPosition(glm::vec3 vec) { position = vec; }
        inline void SetScale(float x, float y, float z) { scale = glm::vec3(x,y,z); }
        inline void SetScale(float s) { scale = glm::vec3(s,s,s); }
        inline void SetScale(glm::vec3 vec) { scale = vec; }
        inline void SetRotation(float pitch, float yaw, float roll) {
            glm::quat qX = glm::angleAxis(glm::radians(pitch), glm::vec3(1, 0, 0));
            glm::quat qY = glm::angleAxis(glm::radians(yaw),   glm::vec3(0, 1, 0));
            glm::quat qZ = glm::angleAxis(glm::radians(roll),  glm::vec3(0, 0, 1));
            this->rotation = qY * qX * qZ; 
        }
        inline void SetRotation(glm::vec3 vec) {
            glm::quat qX = glm::angleAxis(glm::radians(vec.x), glm::vec3(1, 0, 0));
            glm::quat qY = glm::angleAxis(glm::radians(vec.y),   glm::vec3(0, 1, 0));
            glm::quat qZ = glm::angleAxis(glm::radians(vec.z),  glm::vec3(0, 0, 1));
            this->rotation = qY * qX * qZ; 
        }

        inline glm::vec3 GetRotationEuler() {
            glm::vec3 radians = glm::eulerAngles(rotation);
            return glm::degrees(radians);
        }
        inline void SetRotationX(float pitch) {
            glm::vec3 euler = GetRotationEuler();
            SetRotation(pitch, euler.y, euler.z);
        }
        inline void SetRotationY(float yaw) {
            glm::vec3 euler = GetRotationEuler();
            SetRotation(euler.x, yaw, euler.z);
        }
        inline void SetRotationZ(float roll) {
            glm::vec3 euler = GetRotationEuler();
            SetRotation(euler.x, euler.y, roll);
        }
};