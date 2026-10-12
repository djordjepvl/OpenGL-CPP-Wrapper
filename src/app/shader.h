#pragma once

#include <config.h>

#include <camera.h>

class Shader
{
    public:
        GLuint id;
        Shader(const std::string &vertexFile, const std::string &fragmentFile);
        ~Shader();
        void Use();
        void setMat4(const std::string& uniform, glm::mat4 matrix);
        void setVec3(const std::string& name, const glm::vec3& value);
        void setVec4(const std::string& name, const glm::vec4& value);
        void setFloat(const std::string& name, const GLfloat value);
        void setInt(const std::string& name, const GLuint value);
};
