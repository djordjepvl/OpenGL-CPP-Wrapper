#include <shader.h>

#include <fileIO.h>

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string &vertexFile, const std::string &fragmentFile) {
    std::string vertexCodeStr = fileToString(vertexFile);
    std::string fragmentCodeStr = fileToString(fragmentFile);

    if (vertexCodeStr == "") vertexCodeStr = fileToString(SHADER_PATH + "default.vert");
    if (fragmentCodeStr == "") fragmentCodeStr = fileToString(SHADER_PATH + "default.frag");

    const char* vertexCode = vertexCodeStr.c_str();
    const char* fragmentCode = fragmentCodeStr.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, NULL);
    glCompileShader(vertexShader);
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Vertex Shader Error: " << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Fragment Shader Error: " << infoLog << std::endl;
    }

    id  = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(id);
}

void Shader::Use() {
    glUseProgram(id);
}

void Shader::setMat4(const char* uniform, glm::mat4 matrix) {
    Use();
    glUniformMatrix4fv(glGetUniformLocation(id, uniform), 1, GL_FALSE, glm::value_ptr(matrix));
}
