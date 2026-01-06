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
        static std::shared_ptr<Object3D> LoadGLB(const std::string& path);

        void Draw(Shader *shader, Camera *camera);
        glm::mat4 getModelMatrix();

        inline void Move(float x, float y, float z) { position += glm::vec3(x,y,z); }
        inline void Move(glm::vec3 vec) { position += vec; }
        inline void Scale (float x, float y, float z) { scale *= glm::vec3(x,y,z); }
        inline void Scale (glm::vec3 vec) { scale *= vec; }
        inline void Rotate(float x, float y, float z) { rotation = rotation * glm::quat(glm::radians(glm::vec3(x, y, z))); }
        inline void Rotate(glm::vec3 vec) { rotation = rotation * glm::quat(glm::radians(vec));; }
        
        inline void SetPosition(float x, float y, float z) { position = glm::vec3(x,y,z); }
        inline void SetPosition(glm::vec3 vec) { position = vec; }
        inline void SetScale(float x, float y, float z) { scale = glm::vec3(x,y,z); }
        inline void SetScale(glm::vec3 vec) { scale = vec; }
        inline void SetRotation(float pitch, float yaw, float roll) { rotation = glm::quat(glm::vec3(glm::radians(pitch), glm::radians(yaw), glm::radians(roll))); }
        inline void SetRotation(glm::vec3 vec) { rotation = glm::quat(glm::vec3(glm::radians(vec.x), glm::radians(vec.y), glm::radians(vec.z))); }

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