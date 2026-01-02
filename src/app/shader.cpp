#include <shader.h>

#include <io.h>

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
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
    glCompileShader(fragmentShader);

    id  = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Use() {
    glUseProgram(id);
}

void Shader::Delete() {
    glDeleteProgram(id);
}